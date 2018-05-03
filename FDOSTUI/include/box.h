/*
 BOX.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __box_h__

struct box
{
    int                           	m_pos_x;
    int                           	m_pos_y;
    unsigned int                  	m_len_x;
    unsigned int                  	m_len_y;
};

#ifdef __cplusplus
extern "C" {
#endif

extern int
box_contains(
    int const                     	i_pos_x,
    int const                     	i_pos_y,
    struct box const*                   i_box);

extern int
box_contains_box(
    struct box const*                   i_box1,
    struct box const*                   i_box2);

extern int
box_equal(
    struct box const*                   i_box1,
    struct box const*                   i_box2);

#ifdef __cplusplus
}
#endif

#define __box_h__
#endif
