/*
 DRAW.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "draw.h"
#include "matrix.h"
#include "clip.h"

extern void
draw_fill_area(
    int const                     	i_col,
    int const                     	i_row,
    unsigned int const            	i_columns,
    unsigned int const            	i_rows,
    screen_char_t const                 i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    struct box                          l_area;
    struct box                          l_clip;

    l_area.m_pos_x= i_col;
    l_area.m_pos_y= i_row;
    l_area.m_len_x= i_columns;
    l_area.m_len_y= i_rows;
    matrix_transform(&l_area.m_pos_x, &l_area.m_pos_y);
    clip_get(&l_clip);
    clip(&l_area, &l_clip);

    do
    {
        if (0 == l_area.m_len_x || 0 == l_area.m_len_y)
        {
            break;
        }

        screen_fill_area(
            l_area.m_pos_x,
            l_area.m_pos_y,
            l_area.m_len_x,
            l_area.m_len_y,
            i_char,
            i_fcolor,
            i_bcolor);

    }while(0);

    return;
}

extern void
draw_putc(
    int const                     	i_col,
    int const                     	i_row,
    screen_char_t const                 i_char,
    unsigned int const            	i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    struct box                          l_area;
    struct box                          l_clip;

    l_area.m_pos_x= i_col;
    l_area.m_pos_y= i_row;
    l_area.m_len_x= i_repeat_count;
    l_area.m_len_y= 1;
    matrix_transform(&l_area.m_pos_x, &l_area.m_pos_y);
    clip_get(&l_clip);
    clip(&l_area, &l_clip);

    do
    {

        if (0 == l_area.m_len_x || 0 == l_area.m_len_y)
        {
            break;
        }

        screen_putc(
            l_area.m_pos_x,
            l_area.m_pos_y,
            i_char,
            l_area.m_len_x,
            i_fcolor,
            i_bcolor);

    }while(0);

    return;
}

extern void
draw_putvc(
    int const                     	i_col,
    int const                     	i_row,
    screen_char_t const                 i_char,
    unsigned int const            	i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    struct box                          l_area;
    struct box                          l_clip;

    l_area.m_pos_x= i_col;
    l_area.m_pos_y= i_row;
    l_area.m_len_x= 1;
    l_area.m_len_y= i_repeat_count;
    matrix_transform(&l_area.m_pos_x, &l_area.m_pos_y);
    clip_get(&l_clip);
    clip(&l_area, &l_clip);

    do
    {

        if (0 == l_area.m_len_x || 0 == l_area.m_len_y)
        {
            break;
        }

        screen_putvc(
            l_area.m_pos_x,
            l_area.m_pos_y,
            i_char,
            l_area.m_len_y,
            i_fcolor,
            i_bcolor);

    }while(0);

    return;
}

extern void
draw_puts(
    int const                     	i_col,
    int const                     	i_row,
    unsigned char const*                i_string,
    unsigned int const            	i_length,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    struct box                          l_area;
    struct box                          l_clip;
    unsigned int                  	l_offset;
    int                                 l_pos_x;

    l_area.m_pos_x= i_col;
    l_area.m_pos_y= i_row;
    l_area.m_len_x= i_length;
    l_area.m_len_y= 1;
    matrix_transform(&l_area.m_pos_x, &l_area.m_pos_y);
    l_pos_x= l_area.m_pos_x;
    clip_get(&l_clip);
    clip(&l_area, &l_clip);

    do
    {

        if (0 == l_area.m_len_x || 0 == l_area.m_len_y)
        {
            break;
        }

        l_offset= l_area.m_pos_x - l_pos_x;

        screen_puts(
            l_area.m_pos_x,
            l_area.m_pos_y,
            i_string+l_offset,
            l_area.m_len_x,
            i_fcolor,
            i_bcolor);

    }while(0);

    return;
}

extern void
draw_frame(
    struct box const*const              i_box,
    struct skin_frame const*const       i_skin,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    unsigned int                  	l_right;
    unsigned int                  	l_bottom;

    l_right= (*i_box).m_pos_x + (*i_box).m_len_x - 1;
    l_bottom= (*i_box).m_pos_y + (*i_box).m_len_y - 1;

    draw_putc(
        (*i_box).m_pos_x,
        (*i_box).m_pos_y,
        (*i_skin).m_hline,
        (*i_box).m_len_x,
        i_fcolor,
        i_bcolor);
        
    draw_putc(
        (*i_box).m_pos_x,
        l_bottom,
        (*i_skin).m_hline,
        (*i_box).m_len_x,
        i_fcolor,
        i_bcolor);
        
    draw_putvc(
        (*i_box).m_pos_x,
        (*i_box).m_pos_y,
        (*i_skin).m_vline,
        (*i_box).m_len_y,
        i_fcolor,
        i_bcolor);
        
    draw_putc(
        (*i_box).m_pos_x,
        (*i_box).m_pos_y,
        (*i_skin).m_top_left,
        1,
        i_fcolor,
        i_bcolor);
        
    draw_putc(
        (*i_box).m_pos_x,
        l_bottom,
        (*i_skin).m_bottom_left,
        1,
        i_fcolor,
        i_bcolor);
        
    draw_putvc(
        l_right,
        (*i_box).m_pos_y,
        (*i_skin).m_vline,
        (*i_box).m_len_y,
        i_fcolor,
        i_bcolor);
        
    draw_putc(
        l_right,
        (*i_box).m_pos_y,
        (*i_skin).m_top_right,
        1,
        i_fcolor,
        i_bcolor);
        
    draw_putc(
        l_right,
        l_bottom,
        (*i_skin).m_bottom_right,
        1,
        i_fcolor,
        i_bcolor);
        
    return;
}
