/*
 HASH.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "hash.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

static struct bucket *
bucket_new(
    char const *                        i_key,
    void *                              i_value)
{
    struct bucket *                     l_bucket;
    size_t                              l_length;

    l_bucket= (struct bucket*)calloc(1, sizeof(*l_bucket));

    do
    {

        if (0 == l_bucket)
        {
            break;
        }

        l_length= 1 + strlen(i_key);
        (*l_bucket).m_key= (char*)malloc(l_length);

        if ((*l_bucket).m_key)
        {
            memcpy((*l_bucket).m_key, i_key, l_length);
            (*l_bucket).m_value= i_value;
            break;
        }

        free(l_bucket);
        l_bucket= 0;

    }while(0);

    return l_bucket;
}

static int
bucket_insert(
    struct bucket *                     io_bucket,
    char const *                        i_key,
    void *                              i_value,
    hash_remove_cb                      i_remove)
{
    struct bucket *                     l_bucket;
    int                                 l_exit;
    struct bucket *                     l_new;
    int                                 l_rc;

    l_exit= ENOMEM;
    l_bucket= io_bucket;

    do
    {

        l_rc= strcmp(i_key, (*l_bucket).m_key);

        if (0 == l_rc)
        {
            if (i_remove)
            {
                (*i_remove)((*l_bucket).m_value);
            }            
            (*l_bucket).m_value= i_value;
            l_exit= 0;
            break;
        }

        if (0 == (*l_bucket).m_next)
        {
            l_new= bucket_new(i_key, i_value);
            if (l_new)
            {
                (*l_bucket).m_next= l_new;
                l_exit= 0;
            }
            break;
        }

        l_bucket= (*l_bucket).m_next;

    }while(1);

    return l_exit;
}

static void
bucket_delete(
    struct bucket *                     io_bucket,
    hash_remove_cb                      i_remove)
{
    struct bucket *                     l_next;
    struct bucket *                     l_tail;

    l_tail= io_bucket;

    do
    {

        if (0 == l_tail)
        {
            break;
        }

        l_next= (*l_tail).m_next;

        if (i_remove)
        {
            (*i_remove)((*l_tail).m_value);
        }
        
        free((*l_tail).m_key);
        free(l_tail);

        l_tail= l_next;

    }while(1);

    return;
}

/*
    djb2 hash algorithim. 
    By Daniel Bernstein posted to comp.lang.c.
    described at http://www.cse.yorku.ca/~oz/hash.html
    (public domain)
*/
static unsigned long
hash_djb2(
    char const *                        i_str,
    size_t const                        i_buckets)
{
    unsigned long                       l_hash;
    char const *                        l_ptr;

    for (l_hash= 5381, l_ptr= i_str; *l_ptr; l_ptr++)
    {
        l_hash= (l_hash << 5) + l_hash + (*l_ptr);
    }

    l_hash%= i_buckets;

    return l_hash;
}

extern int
hash_assign(
    struct hash*const                   o_hash,
    size_t const                        i_buckets,
    hash_remove_cb                      i_remove)
{
    int                                 l_exit;

    l_exit= ENOMEM;
    memset(o_hash, 0, sizeof(*o_hash));

    do
    {

        if (0 == i_buckets)
        {
            break;
        }

        (*o_hash).m_bucket= (struct bucket**)calloc(i_buckets, sizeof(struct bucket *));

        if ((*o_hash).m_bucket)
        {
            (*o_hash).m_slots= i_buckets;
            (*o_hash).m_remove= i_remove;
            l_exit= 0;
        }

    }while(0);

    return l_exit;
}

extern void
hash_discharge(
    struct hash*const                   io_hash)
{
    size_t                              l_slot;
    struct bucket *                     l_bucket;

    for (l_slot= 0; l_slot < (*io_hash).m_slots; l_slot++)
    {
        l_bucket= (*io_hash).m_bucket[l_slot];
        if (l_bucket)
        {
            bucket_delete(l_bucket, (*io_hash).m_remove);
        }
    }

    free((*io_hash).m_bucket);
    memset(io_hash, 0, sizeof((*io_hash)));

    return;
}

extern int
hash_insert(
    struct hash*const                   io_hash,
    char const *                        i_key,
    void *                              i_value)
{
    int                                 l_exit;
    struct bucket *                     l_bucket;
    size_t                              l_slot;

    l_exit= ENOMEM;

    do
    {

        if (0 == (*io_hash).m_slots)
        {
            break;
        }

        if (0 == i_key)
        {
            l_exit= EINVAL;
            break;
        }

        l_slot= hash_djb2(i_key, (*io_hash).m_slots);
        l_bucket= (*io_hash).m_bucket[l_slot];

        if (l_bucket)
        {
            l_exit= bucket_insert(
                l_bucket,
                i_key,
                i_value,
                (*io_hash).m_remove);
        }
        else
        {
            l_bucket= bucket_new(i_key, i_value);
            if (l_bucket)
            {
                (*io_hash).m_bucket[l_slot]= l_bucket;
                l_exit= 0;
            }
        }

    }while(0);

    return l_exit;
}

extern void *
hash_lookup(
    int *const                          o_found,
    struct hash*const                   io_hash,
    char const *                        i_key)
{
    struct bucket *                     l_bucket;
    int                                 l_rc;
    size_t                              l_slot;
    void *                              l_value;

    (*o_found)= 0;
    l_value= 0;

    do
    {

        if (0 == (*io_hash).m_slots)
        {
            break;
        }

        if (0 == i_key)
        {
            break;
        }

        l_slot= hash_djb2(i_key, (*io_hash).m_slots);
        l_bucket= (*io_hash).m_bucket[l_slot];

        do
        {

            if (0 == l_bucket)
            {
                break;
            }

            l_rc= strcmp(i_key, (*l_bucket).m_key);

            if (0 == l_rc)
            {
                (*o_found)= 1;
                l_value= (*l_bucket).m_value;
                break;
            }

            l_bucket= (*l_bucket).m_next;

        }while(1);

    }while(0);

    return l_value;
}

extern int
hash_remove(
    struct hash*const                   io_hash,
    char const *                        i_key)
{
    struct bucket *                     l_bucket;
    int                                 l_exit;
    struct bucket *                     l_prev;
    int                                 l_rc;
    size_t                              l_slot;

    l_exit= ENOENT;

    do
    {

        if (0 == (*io_hash).m_slots)
        {
            break;
        }

        if (0 == i_key)
        {
            break;
        }

        l_slot= hash_djb2(i_key, (*io_hash).m_slots);
        l_bucket= (*io_hash).m_bucket[l_slot];
        l_prev= 0;

        do
        {

            if (0 == l_bucket)
            {
                break;
            }

            l_rc= strcmp(i_key, (*l_bucket).m_key);

            if (0 == l_rc)
            {
                if (l_prev)
                {
                    (*l_prev).m_next= (*l_bucket).m_next;
                }
                else
                {
                    (*io_hash).m_bucket[l_slot]= (*l_bucket).m_next;
                }

                free((*l_bucket).m_key);

                if ((*io_hash).m_remove)
                {
                    (*(*io_hash).m_remove)((*l_bucket).m_value);
                }            

                free(l_bucket);
                
                l_exit= 0;
                break;
            }

            l_prev= l_bucket;
            l_bucket= (*l_bucket).m_next;

        }while(1);

    }while(0);

    return l_exit;
}
