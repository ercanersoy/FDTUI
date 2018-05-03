/*
 MATRIX.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __matrix_h__

struct matrix
{
    int                           	m_translate_x;
    int                           	m_translate_y;
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct matrix                    _matrix;

extern void
matrix_cleanup();

extern void
matrix_push();

extern void
matrix_pop();

extern void
matrix_translate(
    int const                     	i_translate_x,
    int const                     	i_translate_y);

extern void
matrix_transform(
    int*const                     	o_pos_x,
    int*const                     	o_pos_y);

#ifdef __cplusplus
}
#endif

#define __matrix_h__
#endif
