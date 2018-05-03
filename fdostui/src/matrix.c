/*
 MATRIX.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "matrix.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

struct matrix                           _matrix= {0,0};
static struct matrix*                   _array= 0;
static size_t                           _array_slots= 0;
static size_t                           _array_top= 0;

extern void
matrix_cleanup()
{
    free(_array);
    _array= 0;
    _array_slots= 0;
    _array_top= 0;
    memset(&_matrix, 0, sizeof(_matrix));
}

extern void
matrix_push()
{
    struct matrix*                      l_new;
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
            l_size= l_slots * sizeof(struct matrix);
            l_new= (struct matrix*)realloc(_array, l_size);
            if (0 == l_new)
            {
                break;
            }
            _array= l_new;
            _array_slots= l_slots;
        }

        _array[_array_top]= _matrix;
        _array_top++;

    }while(0);

    return;
}

extern void
matrix_pop()
{
    if (_array_top)
    {
        _array_top--;
        _matrix= _array[_array_top];
    }
    return;
}

extern void
matrix_translate(
    int const                           i_translate_x,
    int const                           i_translate_y)
{
    _matrix.m_translate_x+= i_translate_x;
    _matrix.m_translate_y+= i_translate_y;
    return;
}

extern void
matrix_transform(
    int*const                           o_pos_x,
    int*const                           o_pos_y)
{
    (*o_pos_x)+= _matrix.m_translate_x;
    (*o_pos_y)+= _matrix.m_translate_y;
    return;
}
