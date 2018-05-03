/*
 MENU.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __menu_hpp__

#include "window.hpp"
#include "menuitem.h"

class menu : public window
{

public:

    enum event_response_extended
    {
        RESPONSE_LEFT                  = -20,
        RESPONSE_RIGHT                 = -21
    };

    menu(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~menu();

    virtual void
    draw() const;

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);
    
    virtual enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    void
    set_menu(
        struct menuitem*                i_menu);

    void
    set_response_extended(
        bool const                      i_turnon);

#if defined(__CURSES__)
    virtual void
    focus_enter();
#endif

protected:

    bool                                m_response_extended;
    unsigned int                        m_selected;
    unsigned int                        m_offset;
    int                                 m_orig_pos_y;
    struct array                        m_array;
    struct menuitem*                    m_menu;

private:

    menu();

    menu(
        const menu&);

    menu&
    operator=(menu const&);

    virtual void
    move(
        int const                       i_pos_x,
        int const                       i_pos_y);
    
    virtual void
    resize(
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);

    void
    draw_line(
        int const                       i_pos_y,
        int const                       i_erase) const;

    void
    draw_item(
        int const                       i_pos_y,
        int const                       i_selected,
        struct menuitem const*const     i_menu) const;

    void
    draw_items() const;

    void
    preflight();

    enum event_response
    event_key_ascii(
        unsigned char const             i_ascii);

    enum event_response
    event_key_scan(
        enum scancodes const            i_scan);

    void
    select(
        unsigned int const              i_slot);

    void
    select_next();

    void
    select_previous();

    enum event_response
    submenu(
        struct menuitem *               io_menu,
        unsigned int const              i_pos_x,
        unsigned int const              i_pos_y);

    int
    event_in_parent(
        int const                       i_pos_x,
        int const                       i_pos_y);
};

inline void
menu::move(
    int const                           i_pos_x,
    int const                           i_pos_y)
{
    return;
}

inline void
menu::resize(
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y)
{
    return;
}

inline void
menu::set_menu(
    struct menuitem*                    i_menu)
{

    m_menu= i_menu;
    preflight();

    return;
}

inline void
menu::set_response_extended(
    bool const                          i_turnon)
{

    m_response_extended= i_turnon;

    return;
}

#if defined(__CURSES__)
inline void
menu::focus_enter()
{
    window::focus_enter();
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        ::move(m_box.m_pos_y, m_box.m_pos_x);
    }
}
#endif

#define __menu_hpp__
#endif
