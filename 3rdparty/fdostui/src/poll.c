/*
 POLL.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "poll.h"
#include "mouse.h"
#include "keyboard.h"
#include <string.h>

extern void
poll_yield();

#if defined(__NT__) || defined(__CURSES__)
#define poll_yield()
#endif

#if defined(__DJGPP__)
#include <dpmi.h>
#define poll_yield() __dpmi_yield()
#endif

#if defined(__DOS__)
#pragma aux poll_yield= \
    "mov ax, 1680h" \
    "int 2fh" \
    modify [AX]
#endif

static struct event_key                 _poll_key_unget= {0,0};
static unsigned int                     _poll_key_unget_count= 0;

extern void
poll_keyboard_unget(
    struct event_key const*const        i_event)
{
    _poll_key_unget= (*i_event);
    _poll_key_unget_count= 1;
    return;
}

static int
poll_keyboard(
    struct event_key*const              o_event)
{
    unsigned char                       l_ascii;
    int                                 l_exit;
    enum scancodes                      l_scan;

    do
    {

        if (_poll_key_unget_count)
        {
            (*o_event)= _poll_key_unget;
            _poll_key_unget_count= 0;
            l_exit= 1;
            break;
        }

        l_exit= keyboard_query();

        if (0 == l_exit)
        {
            break;
        }

        keyboard_read((unsigned char*)&l_scan, &l_ascii);
        (*o_event).m_ascii= l_ascii;
        (*o_event).m_scan= l_scan;

    }while(0);

    return l_exit;
}

static int
poll_mouse(
    struct event_mouse *const           o_event)
{
    static struct mouse_event           _prev= {0,0,MOUSE_BUTTON_NONE};
    int                                 l_exit;
    struct mouse_event                  l_event;

    do
    {
        l_exit= mouse_get_event(&l_event);

        if (0 == l_exit)
        {
            break;
        }

        (*o_event).m_pos_x= (l_event.m_curs_col / 8);
        (*o_event).m_pos_y= (l_event.m_curs_row / 8);

        if (l_event.m_btn_state)
        {
            if (_prev.m_btn_state)
            {
                (*o_event).m_state= MOUSE_BUTTON_DRAGGING;
            }
            else
            {
                (*o_event).m_state= MOUSE_BUTTON_CLICKED;
            }
        }
        else 
        {
            if (_prev.m_btn_state)
            {
                (*o_event).m_state= MOUSE_BUTTON_RELEASED;
            }
            else
            {
                (*o_event).m_state= MOUSE_BUTTON_NONE;
            }
        }

        _prev= l_event;

    }while(0);

    return l_exit;
}

extern int
poll_block(
    struct poll_event *const         	o_event)
{
    int                                 l_exit;

    memset(o_event, 0, sizeof(*o_event));

    do
    {

        l_exit= poll_keyboard(&(*o_event).m_record.m_key);

        if (l_exit)
        {
            (*o_event).m_type= EVENT_KEY;
            break;
        }

        l_exit= poll_mouse(&(*o_event).m_record.m_mouse);

        if (l_exit)
        {
            (*o_event).m_type= EVENT_MOUSE;
            break;
        }

        poll_yield();

    }while(1);

    return l_exit;
}
