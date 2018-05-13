/*
 BUTTON.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __button_hpp__

#include "draw.h"
#include "widget.hpp"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

class button : public widget
{

public:

    typedef void (*signal_clicked_t)(
        button const*                   i_button,
        void*                           io_user_data);

    enum style
    {
        STYLE_FLAT                      = (1 << 0),
        STYLE_SHADOW                    = (1 << 1),
        STYLE_BOX                       = (1 << 2)
    };

    button(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~button();

    virtual void
    draw() const;

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
    set_response(
        int const                       i_response);

    void
    set_style(
        unsigned int const              i_style);

    void
    set_text(
        unsigned char const*            i_str);

    void
    set_signal_clicked(
        signal_clicked_t                i_signal);

protected:

    enum type
    {
        TYPE_BUTTON,
        TYPE_CHECK,
        TYPE_RADIO
    };

    bool                                m_state;
    enum type                           m_type;
    unsigned int                        m_style;
    unsigned char*                      m_label;
    enum event_response                 m_response;
    signal_clicked_t                    m_signal_clicked;

    virtual void
    toggled();

private:

    button();
    
    button(
    const button&);

    button&
    operator=(button const&);

    void
    emit_clicked();

};

inline void
button::set_style(
    unsigned int const          i_style)
{
    m_style= i_style;
    return;
}

inline void
button::set_text(
    unsigned char const*                i_str)
{

    free(m_label);
    m_label= 0;

    if (i_str && i_str[0])
    {
        size_t                          l_len;
        l_len= 1+strlen(reinterpret_cast<char const*>(i_str));
        m_label= reinterpret_cast<unsigned char*>(malloc(l_len));
        if (m_label)
        {
            memcpy(m_label, i_str, l_len);
        }
    }

    return;
}

inline void
button::set_response(
    int const                           i_response)
{
    m_response= static_cast<enum event_response>(i_response);
    return;
}

inline void
button::set_signal_clicked(
    signal_clicked_t                    i_signal)
{
    m_signal_clicked= i_signal;
}

inline  void
button::emit_clicked()
{

    if (m_signal_clicked)
    {
        (*m_signal_clicked)(this, m_user_data);
    }

    return;
}

inline void
button::toggled()
{
    return;
}

#define __button_hpp__
#endif
