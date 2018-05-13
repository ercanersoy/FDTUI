/*
 TREEITEM.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#ifndef __treeitem_hpp__

#include "array.h"
#include "box.h"

class treeitem
{

public:

    treeitem(
        unsigned char const*            i_str,
        void *const                     io_user_data= 0);

    bool
    add(
        unsigned char const*            i_str,
        void *const                     io_user_data= 0);

    bool
    add(
        treeitem*const                  io_item);

    treeitem*const
    next() const;

    treeitem*const
    prev() const;

    static void
    set_links(
        treeitem*const                  io_item,
        treeitem*const                  io_parent,
        treeitem*const                  io_prev,
        unsigned int const              i_level);

    virtual 
    ~treeitem();

protected:

    unsigned int                        m_open:1;
    unsigned char*                      m_text;
    unsigned int                        m_len;
    int                                 m_pos_y;
    void*                               m_user_data;
    struct array                        m_child;
    treeitem*                           m_parent;
    treeitem*                           m_next;
    treeitem*                           m_prev;
    unsigned int                        m_level;

private:

    friend class tree;

    treeitem();

    treeitem(
        const treeitem&);

    treeitem&
    operator=(treeitem const&);

};

#define __treeitem_hpp__
#endif
