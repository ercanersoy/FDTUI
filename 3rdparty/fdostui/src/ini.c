/*
 INI.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "ini.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#if defined(__DOS__) || defined(__NT__) && !defined(__NTCRUSTY__)
#include <sys\types.h>
#include <sys\stat.h>
#include <direct.h>
#include <io.h>
#include <malloc.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#endif
#include <fcntl.h>

struct line
{
    char                                m_buf[INI_VALUE_MAX_LEN+1];
    size_t                              m_lineno;
};

static int
line_read(
    int *const                          o_eof,
    struct line *const                  io_line,
    int                                 io_fd)
{
    long                                l_bytes;
    char                                l_chr;
    int                                 l_exit;
    char *                              l_ptr;
    size_t                              l_size;

    *o_eof= 0;
    memset(&(*io_line).m_buf, 0, sizeof((*io_line).m_buf));
    l_exit= 0;
    l_ptr= &(*io_line).m_buf[0];
    l_size= 0;

    do
    {

        if (INI_VALUE_MAX_LEN <= l_size)
        {
            fprintf(
                stderr, 
                "INI(READLINE) maximum line capacity reached line=%lu\n",
                (unsigned long)(*io_line).m_lineno);
            l_exit= E2BIG;
            break;
        }

        l_bytes= read(io_fd, &l_chr, 1);

        if (0 == l_bytes)
        {
            *o_eof= 1;
            break;
        }

        if (-1 == l_bytes)
        {
            l_exit= errno;
            fprintf(
                stderr, 
                "INI(READLINE) read error line=%lu:"
                "error[%d]='%s'\n",
                (unsigned long)(*io_line).m_lineno,
                l_exit,
                strerror(l_exit));
            break;
        }

        if ('\r' == l_chr)
        {
            continue;
        }

        if ('\n' == l_chr)
        {
            (*io_line).m_lineno++;
            break;
        }

        *l_ptr++= l_chr;
        l_size++;

    }while(1);

    *l_ptr= 0;

    return l_exit;
}

static int
is_white(
    char const                          i_chr)
{
    int                                 l_is_white;

    switch(i_chr)
    {
        case 0:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 32:
        case 127:
            l_is_white= 1;
            break;
        default:
            l_is_white= 0;
            break;
    }

    return l_is_white;
}

static void
trim_left(
    char *                              i_str)
{
    char *                              l_head;
    int                                 l_is_white;
    char *                              l_tail;

    l_tail= i_str;

    for(l_head= i_str; *l_head; l_head++)
    {
        l_is_white= is_white(*l_head);

        if (0 == l_is_white)
        {
            break;
        }
    }

    do
    {

        if (l_head == l_tail)
        {
            break;
        }

        while(*l_head)
        {
            *l_tail++= *l_head++;
        }

        *l_tail= 0;

    }while(0);

    return;
}

static void
trim_right(
    char *                              i_str)
{
    int                                 l_is_white;
    char *                              l_tail;

    for (l_tail= i_str; *l_tail; l_tail++);

    do
    {

        if (i_str == l_tail)
        {
            break;
        }

        l_tail--;
        l_is_white= is_white(*l_tail);

        if (0 == l_is_white)
        {
            break;
        }

        *l_tail= 0;

    }while(1);

    return;
}

static void
ini_tuple_remove_cb(
    void *                              i_value)
{
    struct ini_tuple *                  l_tuple;

    l_tuple= (struct ini_tuple *)i_value;
    free((*l_tuple).m_name);
    free((*l_tuple).m_value);
    free(l_tuple);

    return;
}

static void
ini_section_remove_cb(
    void *                              i_value)
{
    struct ini_section *                l_section;

    l_section= (struct ini_section *)i_value;
    free((*l_section).m_name);
    hash_discharge(&(*l_section).m_hash);
    free(l_section);

    return;
}

static int
ini_section_new(
    struct ini_section **               o_section,
    struct ini *const                   io_ini,
    char *                              io_line)
{
    int                                 l_exit;
    int                                 l_found;
    size_t                              l_length;
    char *                              l_ptr;
    struct ini_section *                l_section;

    *o_section= 0;
    l_exit= 0;
    l_ptr= io_line;

    do
    {

        *l_ptr= ' ';
        trim_left(l_ptr);
        l_found= 0;

        for(; *l_ptr; l_ptr++)
        {
            if (']' == *l_ptr)
            {
                *l_ptr++= 0;
                l_found= 1;
                break;
            }
        }

        trim_right(io_line);

        if (0 == l_found || *l_ptr || 0 == *io_line)
        {
            break;
        }

        for(l_ptr= io_line; *l_ptr; l_ptr++)
        {
            *l_ptr= (char)tolower((int)(*l_ptr));
        }

        l_section= hash_lookup(&l_found, &(*io_ini).m_hash, io_line);

        if (l_found)
        {
            *o_section= l_section;
            break;
        }

        l_section= (struct ini_section*)calloc(1, sizeof(*l_section));

        if (0 == l_section)
        {
            fprintf(
                stderr, 
                "INI(SECTION) Unable to allocate section:"
                "error[%d]='%s'\n",
                errno,
                strerror(errno));
            l_exit= errno;
            break;
        }

        hash_assign(
            &(*l_section).m_hash,
            INI_TUPLE_BUCKETS,
            ini_tuple_remove_cb);

        l_length= 1 + strlen(io_line);
        (*l_section).m_name= malloc(l_length);

        if (0 == (*l_section).m_name)
        {
            fprintf(
                stderr, 
                "INI(SECTION) Unable to allocate key:"
                "error[%d]='%s'\n",
                errno,
                strerror(errno));
            l_exit= errno;
            break;
        }

        memcpy((*l_section).m_name, io_line, l_length);

        l_exit= hash_insert(&(*io_ini).m_hash, (*l_section).m_name, l_section);

        if (l_exit)
        {
            ini_section_remove_cb(l_section);
            fprintf(
                stderr, 
                "INI(SECTION) Unable to insert hash:"
                "error[%d]='%s'\n",
                l_exit,
                strerror(l_exit));
            break;
        }

        *o_section= l_section;
        l_section= 0;

    }while(0);

    if (l_section)
    {
        ini_section_remove_cb(l_section);
    }

    return l_exit;
}

static int
ini_tuple_new(
    struct ini_tuple **                 o_tuple,
    struct ini_section *const           io_section,
    char *                              io_line)
{
    int                                 l_exit;
    int                                 l_found;
    size_t                              l_length;
    char *                              l_tail;
    struct ini_tuple *                  l_tuple;
    char *                              l_value;
    char *                              l_value_new;

    *o_tuple= 0;
    l_exit= 0;
    l_value=0;
    l_value_new= 0;

    for(l_tail= io_line; *l_tail; l_tail++)
    {
        if ('=' == *l_tail)
        {
            *l_tail++= 0;
            trim_left(l_tail);
            trim_right(l_tail);
            l_value= l_tail;
            break;
        }
    }

    trim_right(io_line);

    do
    {

        if (0 == *io_line)
        {
            break;
        }

        if (l_value)
        {
            l_length= 1 + strlen(l_value);
            l_value_new= (char*)malloc(l_length);

            if (0 == l_value_new)
            {
                fprintf(
                    stderr, 
                    "INI(TUPLE) Unable to allocate value:"
                    "error[%d]='%s'\n",
                    errno,
                    strerror(errno));
                l_exit= errno;
                break;
            }

            memcpy(l_value_new, l_value, l_length);
        }

        for(l_tail= io_line; *l_tail; l_tail++)
        {
            *l_tail= (char)tolower((int)(*l_tail));
        }

        l_tuple= hash_lookup(&l_found, &(*io_section).m_hash, io_line);

        if (l_found)
        {
            *o_tuple= l_tuple;
            free((*l_tuple).m_value);
            (*l_tuple).m_value= l_value_new;
            break;
        }

        if (0 == l_value_new)
        {
            break;
        }

        l_tuple= (struct ini_tuple*)calloc(1, sizeof(*l_tuple));

        if (0 == l_tuple)
        {
            free(l_value_new);
            fprintf(
                stderr, 
                "INI(TUPLE) Unable to allocate tuple:"
                "error[%d]='%s'\n",
                errno,
                strerror(errno));
            l_exit= errno;
            break;
        }

        l_length= 1 + strlen(io_line);
        (*l_tuple).m_name= (char*)malloc(l_length);

        if (0 == (*l_tuple).m_name)
        {
            free(l_value_new);
            free(l_tuple);
            fprintf(
                stderr, 
                "INI(TUPLE) Unable to allocate key:"
                "error[%d]='%s'\n",
                errno,
                strerror(errno));
            l_exit= errno;
            break;
        }

        memcpy((*l_tuple).m_name, io_line, l_length);
        (*l_tuple).m_value= l_value_new;

        l_exit= hash_insert(&(*io_section).m_hash, (*l_tuple).m_name, l_tuple);
        
        if (l_exit)
        {
            ini_tuple_remove_cb(l_tuple);
            fprintf(
                stderr, 
                "INI(TUPLE) Unable to insert hash:"
                "error[%d]='%s'\n",
                l_exit,
                strerror(l_exit));
            break;
        }

        *o_tuple= l_tuple;

    }while(0);

    return l_exit;
}

extern int
ini_assign(
    struct ini*const                    o_ini)
{
    int                                 l_exit;

    memset(o_ini, 0, sizeof(*o_ini));

    l_exit= hash_assign(
        &(*o_ini).m_hash,
        INI_SECTION_BUCKETS,
        ini_section_remove_cb);

    return l_exit;
}

extern void
ini_discharge(
    struct ini*const                    io_ini)
{

    hash_discharge(&(*io_ini).m_hash);
    memset(io_ini, 0,sizeof(*io_ini));

    return;
}

extern char const *
ini_lookup(
    int*const                           o_found,
    struct ini *const                   i_ini,
    char const *                        i_section,
    char const *                        i_key)
{
    int                                 l_found;
    char const *                        l_value;
    struct ini_section *                l_section;
    struct ini_tuple *                  l_tuple;

    *o_found= 0;
    l_value= 0;

    l_section= hash_lookup(&l_found, &(*i_ini).m_hash, i_section);

    if (l_section)
    {
        l_tuple= hash_lookup(&l_found, &(*l_section).m_hash, i_key);

        if (l_tuple)
        {
            l_value= (*l_tuple).m_value;
            *o_found= 1;
        }

    }

    return l_value;
}

extern int
ini_parse(
    struct ini*const                    io_ini,
    char const*                         i_path,
    enum ini_options const              i_options)
{
    int                                 l_eof;
    int                                 l_exit;
    int                                 l_fd;
    struct line *                       l_line;
    struct ini_section *                l_section;
    struct ini_tuple *                  l_tuple;

    (void)i_options; /* currently not used */
    l_line= 0;

    do
    {

        l_fd= open(i_path, O_RDONLY);

        if (-1 == l_fd)
        {
            fprintf(
                stderr, 
                "INI(LOAD) Unable to open '%s':"
                "error[%d]='%s'\n",
                i_path,
                errno,
                strerror(errno));
            l_exit= errno;
            break;
        }

        l_line= (struct line*)calloc(1, sizeof(*l_line));

        if (0 == l_line)
        {
            l_exit= errno;
            break;
        }

        memcpy((*l_line).m_buf, "[defaults]\0", 11); 
        l_exit= ini_section_new(&l_section, io_ini, (*l_line).m_buf);

        if (l_exit)
        {
            break;
        }

        do
        {

            l_exit= line_read(&l_eof, l_line, l_fd);

            if (l_exit || l_eof)
            {
                break;
            }

            trim_left((*l_line).m_buf);
            trim_right((*l_line).m_buf);

            if (0 == (*l_line).m_buf[0])
            {
                continue;
            }

            if (';' == (*l_line).m_buf[0] || '#' == (*l_line).m_buf[0])
            {
                continue;
            }

            if ('[' == (*l_line).m_buf[0])
            {
                l_exit= ini_section_new(&l_section, io_ini, (*l_line).m_buf);

                if (l_exit)
                {
                    break;
                }

                if (0 == l_section)
                {
                    fprintf(
                    stderr, 
                    "INI(LOAD) malformed section, discarding line=%lu\n",
                    (unsigned long)(*l_line).m_lineno);
                }
                continue;
            }

            if (0 == l_section)
            {
                fprintf(
                    stderr, 
                    "INI(LOAD) section not found, discarding line=%lu\n",
                    (unsigned long)(*l_line).m_lineno);
                continue;
            }

            l_exit= ini_tuple_new(&l_tuple, l_section, (*l_line).m_buf);

            if (l_exit)
            {
                break;
            }

        }while(1);

    }while(0);

    free(l_line);

    if (-1 != l_fd)
    {
        close(l_fd);
    }

    return l_exit;
}

