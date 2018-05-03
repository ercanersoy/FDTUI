/*
 ARRAY.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "array.h"
#include <string.h>
#include <stdlib.h>
#if defined(__WATOMC__)
#include <malloc.h>
#endif

extern void
array_assign(
    struct array *const                 o_arr,
    size_t const                        i_growby)
{

    memset(o_arr, 0, sizeof(*o_arr));
    (*o_arr).m_growby= i_growby;

    return;
}

extern void
array_discharge(
    struct array *const                io_arr)
{

    free((*io_arr).m_base);
    memset(io_arr, 0, sizeof(*io_arr));

    return;
}

extern int
array_push(
    struct array *const                 io_arr,
    void *                              io_object)
{
    void *                              l_block;
    int                                 l_exit;
    size_t                              l_size;

    l_exit= 0;

    do
    {

        if ((*io_arr).m_taken >= (*io_arr).m_slots)
        {
            if (0 == (*io_arr).m_growby)
            {
                l_exit= -1;
                break;
            }

            l_size= (*io_arr).m_slots;
            l_size+= (*io_arr).m_growby;
            l_size*= sizeof(char *);
            l_block= realloc((*io_arr).m_base, l_size);
            if (0 == l_block)
            {
                l_exit= -1;
                break;
            }
            (*io_arr).m_slots+= (*io_arr).m_growby;
            (*io_arr).m_base= l_block;
        }

        (*io_arr).m_base[(*io_arr).m_taken]= io_object;
        (*io_arr).m_taken++;

    }while(0);

    return l_exit;
}

extern void *
array_pop(
    struct array *const                io_arr)
{
    void *                             l_object;

    l_object= 0;

    if ((*io_arr).m_taken)
    {
        (*io_arr).m_taken--;
        l_object= (*io_arr).m_base[(*io_arr).m_taken];
    }

    return l_object;
}

extern void *
array_remove(
    struct array *const                 io_arr,
    size_t const                        i_index)
{
    void *                              l_object;
    size_t                              l_head;

    l_object= 0;

    do
    {

        if ((*io_arr).m_taken <= i_index)
        {
            break;
        }

        l_object= (*io_arr).m_base[i_index];
        (*io_arr).m_taken--;

        for (l_head= i_index; (*io_arr).m_taken > l_head; l_head++)
        {
            (*io_arr).m_base[l_head]= (*io_arr).m_base[1 + l_head];
        }

    }while(0);

    return l_object;
}
