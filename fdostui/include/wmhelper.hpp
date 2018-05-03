/*
 WMHELPER.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __wmhelper_hpp__

#include "screen.h"
#include "window.hpp"

class wmhelper
{

public:

    static unsigned int const           MIN_WIDTH;
    static unsigned int const           MIN_HEIGHT;

    wmhelper();

    virtual
    ~wmhelper();

    static void
    draw_decorate(
        struct box const&               i_box,
        enum window::attributes const   i_attr,
        unsigned char const*            i_title);

    static void
    draw_frame(
        struct box const&               i_box,
        struct skin_frame const&        i_skin,
        enum foreground const           i_fcolor,
        enum background const           i_bcolor);

    static void
    draw_title(
        unsigned char const*            i_title,
        struct box const&               i_box);

    enum event_response
    event_key(
        struct event_key const&         i_event);

    enum event_response
    event_key(
        window*const                    i_window,
        struct event_key const&         i_event);

    enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    enum event_response
    event_mouse(
        window*const                    i_window,
        struct event_mouse const&       i_event);

    void
    close();

    void
    maximize();

    void
    minimize();

    bool
    capture() const;

    void
    capture_resize();

    void
    capture_move();

protected:

    enum wm_event
    {
        EVENT_NONE,
/*      EVENT_SYSMENU,
        EVENT_CLOSE,
        EVENT_MINIMIZE,
        EVENT_MAXIMIZE, */
        EVENT_RESIZE,
        EVENT_MOVE
    };

    enum wm_capture
    {
        CAPTURE_NONE,
        CAPTURE_MOUSE,
        CAPTURE_KEYBOARD
    };

    window*                             m_window;
    unsigned int                        m_mouse_x;
    unsigned int                        m_mouse_y;
    enum wm_event                       m_event;
    enum wm_capture                     m_capture;

private:

    wmhelper(
        const wmhelper&);

    wmhelper&
    operator=(wmhelper const&);

    void
    event_system_menu();

};

inline bool
wmhelper::capture() const
{
    return m_capture;
}

inline void
wmhelper::capture_resize()
{
    m_capture= CAPTURE_KEYBOARD;
    m_event= EVENT_RESIZE;
    return;
}

inline void
wmhelper::capture_move()
{
    m_capture= CAPTURE_KEYBOARD;
    m_event= EVENT_MOVE;
}

#define __wmhelper_hpp__
#endif
