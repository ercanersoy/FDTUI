/*
 BLOCK.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "block.h"
#include "screen.h"
#include <stdlib.h>
#include <stddef.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

#if defined(__FLAT__) || defined(__LARGE__)
#   define __malloc                     malloc
#else
#   define __malloc                     _fmalloc
#endif

extern void __FAR*
block_read(
    struct box const*const              i_box)
{
    screen_block_t __FAR*               l_block;
    unsigned int                        l_len_x;
    screen_block_t __FAR*               l_next;
    int                                 l_pos_y;
    unsigned int                        l_row;
    size_t                              l_size;

    l_len_x= (*i_box).m_len_x;
#if defined(__CURSES__)
    l_len_x++;
#endif

    l_size= l_len_x * (*i_box).m_len_y * sizeof(screen_block_t);
#if defined(__CURSES__)
    l_size+= sizeof(screen_block_t);
#endif
    l_block= (screen_block_t __FAR*)__malloc(l_size);
    l_next= l_block;

    for (l_row= 0; (*i_box).m_len_y > l_row; l_row++)
    {
        l_pos_y= (*i_box).m_pos_y + l_row;

        if ((int)_video_rows <= l_pos_y)
        {
            break;
        }

        l_next= screen_read(
            l_next,
            l_len_x,
            (*i_box).m_pos_x,
            l_pos_y);
    }

    return (void __FAR*)l_block;
}

extern void
block_write(
    void const __FAR*                   i_block,
    struct box const*const              i_box)
{
    unsigned int                        l_len_x;
    screen_block_t const __FAR*         l_next;
    int                                 l_pos_y;
    unsigned int                        l_row;

    l_len_x= (*i_box).m_len_x;
#if defined(__CURSES__)
    l_len_x++;
#endif

    l_next= (screen_block_t __FAR*)i_block;

    for (l_row= 0; (*i_box).m_len_y > l_row; l_row++)
    {
        l_pos_y= (*i_box).m_pos_y + l_row;

        if ((int)_video_rows <= l_pos_y)
        {
            break;
        }

        l_next= screen_write(
            (*i_box).m_pos_x,
            l_pos_y,
            l_next,
            l_len_x);
    }

    return;
}
