/*
 ARRAY.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __array_h__

#include <stddef.h>

struct array
{
    size_t                              m_slots;
    size_t                              m_taken;
    size_t                              m_growby;
    void **                             m_base;
};

#ifdef __cplusplus
extern "C" {
#endif

extern void
array_assign(
    struct array *const                 o_arr,
    size_t const                        i_growby);

extern void
array_discharge(
    struct array *const                 io_arr);

extern void *
array_pop(
    struct array *const                 io_arr);

extern int
array_push(
    struct array *const                 io_arr,
    void *                              io_object);

extern void *
array_remove(
    struct array *const                 io_arr,
    size_t const                        i_index);

#ifdef __cplusplus
}
#endif

#define __array_h__
#endif 
