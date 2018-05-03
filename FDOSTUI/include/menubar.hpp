/*
 MENUBAR.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __menubar_hpp__

#include "widget.hpp"
#include "menuitem.h"

class menubar : public widget
{

public:

    menubar(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~menubar();

    virtual void
    draw() const;

    enum event_response
    event_key_default(
       enum scancodes const             i_scan);

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);
    
    virtual enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    void
    set_menu(
        struct menuitembar const*       i_menu);

protected:

    struct item
    {
        struct menuitembar const*       m_menu;
        int                             m_pos_x;
        unsigned int                    m_len_x;
    };

    unsigned                            m_selected;
    struct item*                        m_item;
    size_t                              m_item_count;

private:

    menubar();

    menubar(
        const menubar&);

    menubar&
    operator=(menubar const&);

    void
    draw_item(
        struct item const&              i_item) const;

    void
    popup(
        struct item&                    i_item);

};

#define __menubarbar_hpp__
#endif
