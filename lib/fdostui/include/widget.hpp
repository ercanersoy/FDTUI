/*
 WIDGET.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __widget_hpp__

#include "box.h"
#include "event.h"
#include "cursor.h"
#include <stddef.h>

class group;

class widget
{

public:

    enum damage
    {
        DAMAGE_NONE                     = 0,
        DAMAGE_ALL                      = (1<<0),
        DAMAGE_LABEL                    = (1<<1),
        DAMAGE_VALUE                    = (1<<2),
        DAMAGE_SCROLL                   = (1<<3),
        DAMAGE_MOVED                    = (1<<4),
        DAMAGE_RESERVED1                = (1<<5),
        DAMAGE_RESERVED2                = (1<<6),
        DAMAGE_RESERVED3                = (1<<7)
    };

    widget(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~widget();

    virtual void
    set_damage(
        unsigned int const              i_damage=DAMAGE_NONE);

    virtual bool
    close(
        bool const                      i_shutting_down);

    bool
    contains(
        int const                       i_pos_x,
        int const                       i_pos_y) const;

    bool
    contains(
        struct box const&               i_box) const;
    
    virtual void
    draw() const = 0;

    virtual enum event_response
    event_key_default(
        enum scancodes const            i_scan);
    
    virtual enum event_response
    event_key(
        struct event_key const&         i_event);
    
    virtual enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    virtual void
    focus_enter();

    virtual void
    focus_leave();
    
    void
    get_box(
        struct box&                     o_box) const;

    bool
    get_can_focus() const;
    
    bool
    get_enabled() const;

    bool
    get_has_focus() const;

    group*const
    get_parent() const;

    void*
    get_user_data() const;
    
    bool
    get_visible() const;

    int
    get_pos_x() const;

    int
    get_pos_y() const;

    unsigned int
    get_len_x() const;

    unsigned int
    get_len_y() const;
    
    virtual void
    move(
        int const                       i_pos_x,
        int const                       i_pos_y);
    
    virtual void
    resize(
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);

    void
    set_can_close(
        bool const                      i_can_close);
    
    void
    set_disabled();
    
    void
    set_enabled();
    
    void
    set_hidden();

    virtual void
    set_parent(
        group*const                     i_parent);

    void
    set_user_data(
        void*                           io_user_data);
    
    void
    set_visible();

    void
    translate_screen(
        int*const                       o_pos_x,
        int*const                       o_pos_y) const;

protected:

    unsigned int                        m_damage;

    enum flags
    {
        VISIBLE=                        (1 << 0),
        ENABLED=                        (1 << 1),
        CAN_FOCUS=                      (1 << 2),
        HAS_FOCUS=                      (1 << 3),
        CAN_CLOSE=                      (1 << 4)
    };

    unsigned int                        m_flags;
    struct box                          m_box;
    group*                              m_parent;
    void*                               m_user_data;

private:

    widget();
    
    widget(
    const widget&);

    widget&
    operator=(widget const&);

};

inline bool
widget::contains(
    int const                           i_pos_x,
    int const                           i_pos_y) const
{
    bool                                l_hit;

    l_hit= static_cast<bool>(box_contains(i_pos_x, i_pos_y, &m_box));

    return l_hit;
}

inline bool
widget::contains(
    struct box const&                   i_box) const
{
    bool                                l_hit;

    l_hit= static_cast<bool>(box_contains_box(&m_box, &i_box));

    return l_hit;
}

inline void
widget::move(
    int const                           i_pos_x,
    int const                           i_pos_y)
{
    m_box.m_pos_x= i_pos_x;
    m_box.m_pos_y= i_pos_y;
    return;
}

inline void
widget::resize(
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y)
{
    m_box.m_len_x= i_len_x;
    m_box.m_len_y= i_len_y;
    return;
}

inline void
widget::set_disabled()
{
    m_flags&= ~widget::ENABLED;
    return;
}

inline void
widget::focus_enter()
{
    m_flags|= widget::HAS_FOCUS;
#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        int                             l_cur_x;
        int                             l_cur_y;
        translate_screen(&l_cur_x, &l_cur_y);
        ::move(l_cur_y, l_cur_x);
    }
#endif
    return;
}

inline void
widget::focus_leave()
{
    m_flags&= ~widget::HAS_FOCUS;
    return;
}

inline void
widget::get_box(
    struct box&                         o_box) const
{
    o_box= m_box;
    return;
}

inline bool
widget::get_can_focus() const
{
    return ((m_flags & widget::CAN_FOCUS) && (m_flags & widget::ENABLED));
}

inline bool
widget::get_enabled() const
{
    return (m_flags & widget::ENABLED);
}

inline bool
widget::get_has_focus() const
{
    return (m_flags & widget::HAS_FOCUS);
}

inline group*const
widget::get_parent() const
{
    return m_parent;
}

inline void*
widget::get_user_data() const
{
    return m_user_data;
}

inline bool
widget::get_visible() const
{
    return (m_flags & widget::VISIBLE);
}

inline int
widget::get_pos_x() const
{
    return m_box.m_pos_x;
}

inline int
widget::get_pos_y() const
{
    return m_box.m_pos_y;
}

inline unsigned int
widget::get_len_x() const
{
    return m_box.m_len_x;
}

inline unsigned int
widget::get_len_y() const
{
    return m_box.m_len_y;
}

inline void
widget::set_can_close(
    bool const                          i_can_close)
{
    if (i_can_close)
    {
        m_flags|= widget::CAN_CLOSE;
    }
    else
    {
        m_flags&= ~widget::CAN_CLOSE;
    }
    return;
}

inline void
widget::set_enabled()
{
    m_flags|= widget::ENABLED;
    return;
}

inline void
widget::set_hidden()
{
    m_flags&= ~widget::VISIBLE;
    return;
}

inline void
widget::set_parent(
    group*const                         i_parent)
{
    m_parent= i_parent;
}

inline void
widget::set_user_data(
    void*                               io_user_data)
{
    m_user_data= io_user_data;
    return;
}

inline void
widget::set_visible()
{
    m_flags|= widget::VISIBLE;
    return;
}

inline enum event_response
widget::event_key_default(
    enum scancodes const                i_scan)
{
    return RESPONSE_NONE;
}

inline enum event_response
widget::event_key(
    struct event_key const&             i_event)
{
    return RESPONSE_NONE;
}
    
inline enum event_response
widget::event_mouse(
    struct event_mouse const&           i_event)
{
    return RESPONSE_NONE;
}

inline void
widget::set_damage(
    unsigned int const                  i_damage)
{
    m_damage= i_damage;
}

#define __widget_hpp__
#endif
