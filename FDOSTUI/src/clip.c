/*
 CLIP.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "clip.h"
#include "screen.h"
#include <stddef.h>
#include <stdlib.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

static struct box*                      _array= 0;
static size_t                           _array_slots= 0;
static size_t                           _array_top= 0;

extern void
clip_cleanup()
{
    free(_array);
    _array= 0;
    _array_slots= 0;
    _array_top= 0;
}

extern void
clip_get(
    struct box*const                    o_box)
{

    if (_array_top)
    {
        (*o_box)= _array[_array_top-1];
    }
    else
    {
        (*o_box).m_pos_x= 0;
        (*o_box).m_pos_y= 0;
        (*o_box).m_len_x= _video_cols;
        (*o_box).m_len_y= _video_rows;
    }

    return;
}

extern void
clip_push(
    struct box const*const              i_box)
{
    struct box*                         l_new;
    size_t                              l_size;
    size_t                              l_slots;

    do
    {

        if (_array_slots <= _array_top)
        {
            if (_array_slots)
            {
                l_slots= 2 * _array_slots;
            }
            else
            {
                l_slots= 20;
            }
            l_size= l_slots * sizeof(struct box);
            l_new= (struct box*)realloc(_array, l_size);
            if (0 == l_new)
            {
                break;
            }
            _array= l_new;
            _array_slots= l_slots;
        }

        _array[_array_top]= (*i_box);
        _array_top++;

    }while(0);

    return;
}

extern void
clip_pop()
{

    if (_array_top)
    {
        _array_top--;
    }

    return;
}

extern void
clip(
    struct box*const                    io_area,
    struct box const*const              i_clip)
{
    int                           	l_diff;
    int                           	l_value;
    int                           	l_value1;
    int                           	l_value2;

    if ((*i_clip).m_pos_x > (*io_area).m_pos_x)
    {
        l_diff= (*i_clip).m_pos_x - (*io_area).m_pos_x;
        (*io_area).m_pos_x+= l_diff;
        l_value= (int)(*io_area).m_len_x - l_diff;
        (*io_area).m_len_x= (0 <= l_value) ? l_value : 0;
    }

    if ((*i_clip).m_pos_y > (*io_area).m_pos_y)
    {
        l_diff= (*i_clip).m_pos_y - (*io_area).m_pos_y;
        (*io_area).m_pos_y+= l_diff;
        l_value= (int)(*io_area).m_len_y - l_diff;
        (*io_area).m_len_y= (0 <= l_value) ? l_value : 0;
    }

    if ((*io_area).m_len_x)
    {
        l_value1= ((*i_clip).m_pos_x + (*i_clip).m_len_x);
        l_value2= ((*io_area).m_pos_x + (*io_area).m_len_x);
    
        if (l_value1 < l_value2)
        {
            l_value= (int)(*io_area).m_len_x - (l_value2 - l_value1);
            (*io_area).m_len_x= (0 <= l_value) ? l_value : 0;
        }
    }

    if ((*io_area).m_len_y)
    {
        l_value1= ((*i_clip).m_pos_y + (*i_clip).m_len_y);
        l_value2= ((*io_area).m_pos_y + (*io_area).m_len_y);

        if (l_value1 < l_value2)
        {
            l_value= (int)(*io_area).m_len_y - (l_value2 - l_value1);
            (*io_area).m_len_y= (0 <= l_value) ? l_value : 0;
        }
    }

    return;
}
