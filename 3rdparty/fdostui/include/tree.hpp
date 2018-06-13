/*
 TREE.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#ifndef __tree_hpp__

#include "treeitem.hpp"
#include "scrllbar.hpp"

class tree : public widget
{

public:

    typedef void (*signal_selected_t)(
        tree const*const                i_tree,
        void*                           io_user_data);

    tree(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);

    void
    attach(
        treeitem*const                  io_root);

    virtual void
    draw() const;
    
    virtual
    ~tree();

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);
    
    virtual enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    virtual void
    move(
        int const                       i_pos_x,
        int const                       i_pos_y);

    virtual void
    resize(
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);

protected:

    treeitem*                           m_root;
    treeitem*                           m_selected;
    signal_selected_t                   m_signal_selected;
    scrollbar                           m_horizontal;
    scrollbar                           m_vertical;

private:

    tree();

    tree(
        const tree&);

    tree&
    operator=(tree const&);

    int
    draw(
        treeitem*const                  io_item,
        int const                       i_pos_x,
        int const                       i_pos_y) const;

    void
    emit_selected();

    bool
    event_hit(
        treeitem*const                  io_item,
        unsigned int const              i_pos_y);

    void
    key_down();

    void
    key_up();

    void
    scrollbar_configure();

    void
    scrollbar_configure(
        unsigned int&                   io_height,
        unsigned int&                   io_width,
        treeitem const&                 i_item,
        unsigned int const              i_pos_y);
};

inline void
tree::attach(
    treeitem*const                      io_root)
{

    delete m_root;
    m_root= io_root;
    m_selected= io_root;

    if (m_root)
    {
        treeitem::set_links(m_root, 0, 0, 0);
    } 

    scrollbar_configure();

    return;
}

inline void
tree::emit_selected()
{

    if (m_signal_selected && m_selected)
    {
        (*m_signal_selected)(this, (*m_selected).m_user_data);
    }

    return;
}

#define __tree_hpp__
#endif
