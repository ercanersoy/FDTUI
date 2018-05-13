/*
 GAPBUF.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
/*
 gapbuf_move is based on moveGap in NEdit textBuf.c and 
 FLTK Fl_Text_Buffer.cpp.

 gapbuf_text is based on BufAsString in NEdit textbuf.c

 Copyright (C) 1999 Mark Edel
 Copyright 2001-2010 by Bill Spitzak and others.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 USA.
*/
#include "gapbuf.h"
#include <string.h>
#include <stdlib.h>
#if defined(__BORLANDC__)
#define SIZE_MAX ((size_t)-1) /* 4294967295U 32-bit */
#else
#include <stdint.h>
#endif
#if defined(__WATCOMC__) 
#include <malloc.h>
#endif

#if defined(__FLAT__) || defined(__LARGE__)
#   define __malloc                     malloc
#   define __free                       free
#   define __realloc                    realloc
#   define __memmove                    memmove
#   define __memcpy                     memcpy
#else
#   define __malloc                     _fmalloc
#   define __realloc                    _frealloc
#   define __free                       _ffree
#   define __memmove                    _fmemmove
#   define __memcpy                     _fmemcpy
#endif

extern void
gapbuf_assign(
    struct gapbuf*const                 o_gap)
{

    memset(o_gap, 0, sizeof(*o_gap));

    return;
}

extern void
gapbuf_discharge(
    struct gapbuf*const                 io_gap)
{

    __free((*io_gap).m_block);
    memset(io_gap, 0, sizeof(*io_gap));

    return;
}

#define GAPBUF_GROW_SIZE                512

static int
gapbuf_grow(
    struct gapbuf*const                 io_gap)
{
    unsigned char __FAR*                l_block;
    int                                 l_exit;
    size_t                              l_gapsize;
    unsigned char __FAR*                l_movefrom;
    size_t                              l_movesize;
    unsigned char __FAR*                l_moveto;
    size_t                              l_size;

    l_exit= 0;

    do
    {

        if ((SIZE_MAX - GAPBUF_GROW_SIZE) <= (*io_gap).m_block_size)
        {
            l_exit= -1;
            break;
        }

        l_size= GAPBUF_GROW_SIZE + (*io_gap).m_block_size;

        l_block= (unsigned char __FAR*)__realloc((*io_gap).m_block, l_size);

        if (0 == l_block)
        {
            l_exit= -1;
            break;
        }

        l_movefrom= &l_block[(*io_gap).m_end];
        l_moveto= &l_block[(*io_gap).m_start];
        l_movesize= (*io_gap).m_block_size - (*io_gap).m_end;
        __memcpy(l_moveto, l_movefrom, l_movesize);

        l_gapsize= (*io_gap).m_end - (*io_gap).m_start;
        (*io_gap).m_block= l_block;
        (*io_gap).m_block_size= l_size;
        (*io_gap).m_start= l_size - l_gapsize - GAPBUF_GROW_SIZE;
        (*io_gap).m_end= l_size;

    }while(0);

    return l_exit;
}

static void
gapbuf_move(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset)
{
    size_t                              l_gapsize;
    int                                 l_moved;
    unsigned char __FAR*                l_movefrom;
    size_t                              l_movesize;
    unsigned char __FAR*                l_moveto;

    do
    {

        if (i_offset == (*io_gap).m_start)
        {
            break;
        }

        if (i_offset > (*io_gap).m_start)
        {
            l_movefrom= &(*io_gap).m_block[(*io_gap).m_end];
            l_moveto= &(*io_gap).m_block[(*io_gap).m_start];
            l_movesize= i_offset - (*io_gap).m_start;
            __memmove(l_moveto, l_movefrom, l_movesize);
        }
        else
        {
            l_gapsize= (*io_gap).m_end - (*io_gap).m_start;
            l_movefrom= &(*io_gap).m_block[i_offset];
            l_moveto= &(*io_gap).m_block[i_offset + l_gapsize];
            l_movesize= (*io_gap).m_start - i_offset;
            __memmove(l_moveto, l_movefrom, l_movesize);
        }

        l_moved= i_offset - (*io_gap).m_start;
        (*io_gap).m_end+= l_moved;
        (*io_gap).m_start+= l_moved;

    }while(0);

    return;
}

extern int
gapbuf_backspace(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset)
{
    int                                 l_moved;

    l_moved= 0;

    do
    {

        if (0 == i_offset)
        {
            break;
        }

        if (i_offset != (*io_gap).m_start)
        {
            gapbuf_move(io_gap, i_offset); 
        }

        (*io_gap).m_start--;
        l_moved= 1;

    }while(0);

    return l_moved;
}

extern int
gapbuf_insert(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset,
    unsigned char const                 i_char)
{
    int                                 l_exit;

    l_exit= 0;

    do
    {

        if ((*io_gap).m_start == (*io_gap).m_end)
        {

            l_exit= gapbuf_grow(io_gap);

            if (l_exit)
            {
                break;
            }

        }

        if (i_offset != (*io_gap).m_start)
        {
            gapbuf_move(io_gap, i_offset); 
        }

        (*io_gap).m_block[(*io_gap).m_start]= i_char;
        (*io_gap).m_start++;

    }while(0);

    return l_exit;
}

extern int
gapbuf_delete(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset)
{
    int                                 l_moved;

    l_moved= 0;

    gapbuf_move(io_gap, i_offset);

    if ((*io_gap).m_end < (*io_gap).m_block_size)
    {
        (*io_gap).m_end++;
        l_moved= 1;
    }

    return l_moved;
}

extern unsigned char __FAR*
gapbuf_char_next(
    size_t*                             io_offset,
    struct gapbuf const*const           i_gap)
{
    unsigned char __FAR*                l_ptr;

    l_ptr= gapbuf_ptr(i_gap, 1+(*io_offset));

    if (l_ptr)
    {
        (*io_offset)++;
    }

    return l_ptr;
}

extern unsigned char __FAR*
gapbuf_char_prev(
    size_t*                             io_offset,
    struct gapbuf const*const           i_gap)
{
    unsigned char __FAR*                l_ptr;

    l_ptr= 0;

    if (*io_offset)
    {
        (*io_offset)--;
        l_ptr= gapbuf_ptr(i_gap, (*io_offset));
    }

    return l_ptr;
}

extern unsigned char __FAR*
gapbuf_line_next(
    size_t*                             io_offset,
    struct gapbuf const*const           i_gap)
{
    unsigned char __FAR*                l_ptr;

    l_ptr= gapbuf_ptr(i_gap, (*io_offset));

    do
    {

        if (0 == l_ptr)
        {
            break;
        }

        if ('\n' == (*l_ptr))
        {
            l_ptr= gapbuf_char_next(io_offset, i_gap);
            break;
        }

        l_ptr= gapbuf_char_next(io_offset, i_gap);

    }while(1);

    return l_ptr;
}

extern unsigned char __FAR*
gapbuf_line_prev(
    size_t*                             io_offset,
    struct gapbuf const*const           i_gap)
{
    unsigned char __FAR*                l_ptr;

    do
    {

        l_ptr= gapbuf_char_prev(io_offset, i_gap);

        if (0 == l_ptr)
        {
            break;
        }

        if ('\n' == (*l_ptr))
        {
            break;
        }

    }while(1);

    return l_ptr;
}

extern int
gapbuf_overwrite(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset,
    unsigned char const                 i_char)
{
    int                                 l_exit;
    size_t                              l_offset;

    l_exit= 0;
    l_offset= i_offset;

    do
    {

        if ((*io_gap).m_start == (*io_gap).m_end)
        {
            l_exit= gapbuf_grow(io_gap);

            if (l_exit)
            {
                break;
            }
        }

        if (l_offset >= (*io_gap).m_start)
        {
            l_offset= (*io_gap).m_end + (l_offset - (*io_gap).m_start);
            if (l_offset >= (*io_gap).m_block_size)
            {
                if (l_offset != (*io_gap).m_start)
                {
                    gapbuf_move(io_gap, i_offset);
                }
                l_offset= i_offset;
                (*io_gap).m_start++;
            }
        }

        (*io_gap).m_block[l_offset]= i_char;

    }while(0);

    return l_exit;
}

extern unsigned char const __FAR*
gapbuf_text(
    struct gapbuf *const                io_gap)
{
    size_t                              l_len_left;
    size_t                              l_len_right;
    size_t                              l_offset;
    unsigned char const __FAR*          l_ptr;
    size_t                              l_size;

    l_ptr= 0;
    l_size= (*io_gap).m_block_size - ((*io_gap).m_end - (*io_gap).m_start);

    do
    {

        if (0 == l_size)
        {
            break;
        }

        l_len_left= (*io_gap).m_start;
        l_len_right= (l_size - l_len_left);
        l_offset= (l_len_left < l_len_right) ? 0 : l_size;
        gapbuf_move(io_gap, l_offset);
        l_offset= ((0 == l_len_left) ? (*io_gap).m_end : 0);
        l_ptr= &(*io_gap).m_block[l_offset];

    }while(0);

    return l_ptr;
}

extern unsigned char __FAR*
gapbuf_ptr(
    struct gapbuf const*const           i_gap,
    size_t const                        i_offset)
{
    size_t                              l_size;
    unsigned char __FAR*                l_ptr;

    l_size= (*i_gap).m_block_size - ((*i_gap).m_end - (*i_gap).m_start);
    l_ptr= 0;

    do
    {

        if (i_offset >= l_size)
        {
            break;
        }

        if (i_offset < (*i_gap).m_start)
        {
            l_ptr= (*i_gap).m_block + i_offset; 
            break;
        }

        l_ptr= (*i_gap).m_block + (*i_gap).m_end + 
            (i_offset - (*i_gap).m_start);

    }while(0);

    return l_ptr;
}
