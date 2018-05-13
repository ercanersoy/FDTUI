/*
 SCROLLER.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __scroller_hpp__

#include "group.hpp"
#include "scrllbar.hpp"

class scroller : public group
{

public:

    scroller(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~scroller();

    virtual int
    add(
        widget*const                    i_widget);

    virtual void
    draw() const;

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);
    
    virtual enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    int
    get_scroll_pos_x() const;

    int
    get_scroll_pos_y() const;

    virtual void
    set_parent(
        group*const                    i_parent);

protected:

    scrollbar                           m_horizontal;
    scrollbar                           m_vertical;
    int                                 m_scroll_pos_x;
    int                                 m_scroll_pos_y;

    void
    set_scroll_bounds();

private:

    scroller();

    scroller(
        const scroller&);

    scroller&
    operator=(scroller const&);

    static void
    horizontal_cb(
        widget const*                   i_widget,
        void*                           io_user_data);

    static void
    vertical_cb(
        widget const*                   i_widget,
        void*                           io_user_data);

public:

    void
    scroll_internal();

};

inline int
scroller::add(
    widget*const                    i_widget)
{
    int                             l_exit;

    l_exit= group::add(i_widget);

    if (0 == l_exit)
    {
        set_scroll_bounds();
    }

    return l_exit;
}

inline void
scroller::set_parent(
    group*const                         i_parent)
{
    if (this != i_parent)
    {
        group::set_parent(i_parent);
        m_horizontal.set_parent(i_parent);
        m_vertical.set_parent(i_parent);
    }
}

inline int
scroller::get_scroll_pos_x() const
{
    return m_scroll_pos_x;
}

inline int
scroller::get_scroll_pos_y() const
{
    return m_scroll_pos_y;
}

#define __scroller_hpp__
#endif
