/*
 GROUP.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __group_hpp__

#include "array.h"
#include "widget.hpp"

class group : public widget
{

public:

    group(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~group();

    virtual void
    set_damage(
        unsigned int const              i_damage=DAMAGE_NONE);
    
    virtual int
    add(
        widget*const                    i_widget);
    
    virtual void
    draw() const;

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
    set_focus(
        widget&                         i_widget);

protected:

    widget*                             m_focused;
    size_t                              m_slot;
    struct array                        m_array;

private:

    group();

    group(
        const group&);

    group&
    operator=(group const&);

    void
    focus_next();

    void
    focus_previous();

};

#define __group_hpp__
#endif
