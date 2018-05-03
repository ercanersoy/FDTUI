/*
 BOX.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "box.h"

extern int
box_contains(
    int const                           i_pos_x,
    int const                           i_pos_y,
    struct box const*                   i_box)
{
    int                                 l_hit;
    int                                 l_right;
    int                                 l_bottom;

    l_hit= 0;

    do
    {

        if (0 == (*i_box).m_len_x)
        {
            break;
        }

        if (0 == (*i_box).m_len_y)
        {
            break;
        }

        if (i_pos_x < (*i_box).m_pos_x)
        {
            break;
        }

        if (i_pos_y < (*i_box).m_pos_y)
        {
            break;
        }

        l_right= ((*i_box).m_pos_x + (*i_box).m_len_x) - 1;

        if (i_pos_x > l_right)
        {
            break;
        }

        l_bottom= ((*i_box).m_pos_y + (*i_box).m_len_y) - 1;

        if (i_pos_y > l_bottom)
        {
            break;
        }

        l_hit= 1;

    }while(0);

    return l_hit;
}

extern int
box_contains_box(
    struct box const*                   i_box1,
    struct box const*                   i_box2)
{
    int                                 l_hit;

    l_hit= (
        ((*i_box2).m_pos_x >= (*i_box1).m_pos_x) &&
        ((*i_box2).m_pos_y >= (*i_box1).m_pos_y) &&
        ((int)((*i_box2).m_pos_x+(*i_box2).m_len_x) <=
        (int)((*i_box1).m_pos_x+(*i_box1).m_len_x)) &&
        ((int)((*i_box2).m_pos_y+(*i_box2).m_len_y) <=
        (int)((*i_box1).m_pos_y+(*i_box1).m_len_y))
            );

    return l_hit;
}

extern int
box_equal(
    struct box const*                   i_box1,
    struct box const*                   i_box2)
{
    int                                 l_equal;

    l_equal= (
        ((*i_box1).m_pos_x == (*i_box2).m_pos_x) &&
        ((*i_box1).m_pos_y == (*i_box2).m_pos_y) &&
        ((*i_box1).m_len_x == (*i_box2).m_len_x) &&
        ((*i_box1).m_len_y == (*i_box2).m_len_y)
            );

    return l_equal;
}
