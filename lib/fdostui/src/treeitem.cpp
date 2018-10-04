/*
 TREEITEM.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "treeitem.hpp"
#include <string.h>
#include <stdlib.h>

treeitem::treeitem(
    unsigned char const*                i_str,
    void *const                         io_user_data) :
    m_open(0),
    m_text(0),
    m_len(0),
    m_pos_y(0),
    m_user_data(io_user_data),
    m_child(),
    m_parent(0),
    m_next(0),
    m_prev(0),
    m_level(0)
{

    if (i_str && i_str[0])
    {
        m_len= strlen(reinterpret_cast<char const*>(i_str));
        m_text= reinterpret_cast<unsigned char*>(malloc(1+m_len));
        if (m_text)
        {
            memcpy(m_text, i_str, 1+m_len);
        }
    }

    array_assign(&m_child, 10);

    return;
}

treeitem::~treeitem()
{
    struct treeitem*                    l_item;

    do
    {

        l_item= reinterpret_cast<struct treeitem*>(array_pop(&m_child));

        if (0 == l_item)
        {
            break;
        }

        delete l_item;

    }while(1);

    free(m_text);
    array_discharge(&m_child);

    return;
}

bool
treeitem::add(
    unsigned char const*                i_str,
    void *const                         io_user_data)
{
    bool                                l_exit;
    treeitem*                           l_item;
    int                                 l_rc;

    l_exit= false;

    do
    {

        if (0 == i_str || 0 == i_str[0])
        {
            break;
        }

        l_item= new treeitem(i_str, io_user_data);

        if (0 == l_item)
        {
            break;
        }

        l_rc= array_push(&m_child, l_item);

        if (-1 == l_rc)
        {
            break;
        }

        l_exit= true;

    }while(0);

    return l_exit;
}

bool
treeitem::add(
    treeitem*const                      io_item)
{
    bool                                l_exit;
    int                                 l_rc;

    l_exit= false;

    do
    {

        if (0 == io_item)
        {
            break;
        }

        (*io_item).m_parent= this;
        l_rc= array_push(&m_child, io_item);

        if (-1 == l_rc)
        {
            break;
        }

        l_exit= true;

    }while(0);

    return l_exit;
}

treeitem*const
treeitem::next() const
{
    treeitem*                           l_next;
    treeitem*                           l_parent;

    l_next= 0;

    do
    {

        if (m_open)
        {
            if (m_child.m_taken)
            {
                l_next= reinterpret_cast<treeitem*>(m_child.m_base[0]);
                break;
            }
            break;
        }

        if (0 == m_parent)
        {
            break;
        }

        if (0 == m_next || m_level != (*m_next).m_level)
        {
            if (1 == m_level)
            {
                break;
            }

            l_parent= m_parent;

            do
            {
                l_next= (*l_parent).m_next;
                if (1 == (*l_parent).m_level)
                {
                    break;
                }
                l_parent= (*l_parent).m_parent;
                if (0 == l_parent)
                {
                    break;
                }
            }while(1);

            break;
        }

        l_next= m_next;

    }while(0);

    return l_next;
}

treeitem*const
treeitem::prev() const
{
    treeitem*                           l_prev;

    do
    {
        l_prev= m_prev;

        if (0 == l_prev)
        {
            break;
        }

        if (m_level != (*l_prev).m_level)
        {
            break;
        }

        do
        {

            if (0 == (*l_prev).m_open || 0 == (*l_prev).m_child.m_taken)
            {
                break;
            }

            l_prev= reinterpret_cast<treeitem*>((*l_prev).m_child.m_base[(*l_prev).m_child.m_taken-1]);

        }while(1);

    }while(0);

    return l_prev;
}

void
treeitem::set_links(
    treeitem*const                      io_item,
    treeitem*const                      io_parent,
    treeitem*const                      io_prev,
    unsigned int const                  i_level)
{
    treeitem*                           l_item;
    treeitem*                           l_prev;
    unsigned int                        l_slot;

    l_prev= 0;
    (*io_item).m_parent= io_parent;
    (*io_item).m_next= reinterpret_cast<treeitem*>(
        (((*io_item).m_child.m_taken) ? (*io_item).m_child.m_base[0] : 0));
    (*io_item).m_level= i_level;

    if ((*io_item).m_parent)
    {
        for (l_slot= 0; (*io_item).m_parent->m_child.m_taken > l_slot; l_slot++)
        {
            l_item= reinterpret_cast<treeitem*>(
                (*io_item).m_parent->m_child.m_base[l_slot]);
            if (l_item == io_item)
            {
                if (((*io_item).m_parent->m_child.m_taken -1) == l_slot)
                {
                    if (0 == (*io_item).m_parent->m_parent)
                    {
                        (*io_item).m_next= 0;
                    }
                }
                break;
            }
        }
    }

    (*io_item).m_prev= io_prev;

    if (0 == io_prev && io_parent)
    {
        (*io_item).m_prev= io_parent;
        l_prev= io_item;
    }

    for (l_slot=0; (*io_item).m_child.m_taken > l_slot; l_slot++)
    {
        l_item= reinterpret_cast<treeitem*>((*io_item).m_child.m_base[l_slot]);
        set_links(l_item, io_item, l_prev, (1+i_level));
        l_prev= l_item;
        if ((*io_item).m_child.m_taken > (1+l_slot))
        {
            (*l_item).m_next= reinterpret_cast<treeitem*>(
                (*io_item).m_child.m_base[1+l_slot]);
        }
    }

    return;
}
