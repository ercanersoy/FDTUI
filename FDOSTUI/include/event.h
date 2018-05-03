/*
 EVENT.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __event_h__

#include "keyboard.h"

enum event_response
{
    RESPONSE_CANCEL=                    -4,
    RESPONSE_ACCEPT=                    -3,
    RESPONSE_CLOSE=                     -2,
    RESPONSE_HANDLED=                   -1,
    RESPONSE_NONE=                      0
};

struct event_key
{
    unsigned char                       m_ascii;
    enum scancodes                      m_scan;
};

enum mouse_state
{
    MOUSE_BUTTON_NONE=                  0,
    MOUSE_BUTTON_CLICKED,
    MOUSE_BUTTON_DRAGGING,
    MOUSE_BUTTON_RELEASED
};

struct event_mouse
{
    unsigned int                        m_pos_x;
    unsigned int                        m_pos_y;
    enum mouse_state                    m_state;
};

struct event_resize
{
    unsigned int                        m_len_x;
    unsigned int                        m_len_y;
};

#define __event_h__
#endif
