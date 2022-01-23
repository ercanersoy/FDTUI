/*
vim:expandtab:softtabstop=4:tabstop=4:shiftwidth=4:nowrap:ruler
*/
/*
 POLL.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United State.
*/
#include "poll.h"
#include <curses.h>
#include <string.h>

enum mouse_status
{
    STATUS_CLEAR,
    STATUS_PUSHED,
    STATUS_STICKY
};

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

#if defined(__PDCURSES__) || defined(NCURSES_MOUSE_VERSION)
static void
poll_get_mouse(
    struct poll_event *const            o_event)
{
    static enum mouse_status            l_status= STATUS_CLEAR;
    MEVENT                              l_mouse;

#if defined(__PDCURSES__)
    nc_getmouse(&l_mouse);
#else
    getmouse(&l_mouse);
#endif

    (*o_event).m_type= EVENT_MOUSE;
    (*o_event).m_record.m_mouse.m_pos_x= l_mouse.x;
    (*o_event).m_record.m_mouse.m_pos_y= l_mouse.y;

    do
    {

#if defined(__PDCURSES__)
        if (BUTTON1_RELEASED & l_mouse.bstate || 
            BUTTON3_RELEASED & l_mouse.bstate)
#else
        if (BUTTON_RELEASE(l_mouse.bstate, 1) || 
            BUTTON_RELEASE(l_mouse.bstate, 3))
#endif
        {
            if (STATUS_PUSHED == l_status)
            {
                l_status= STATUS_STICKY;
                (*o_event).m_record.m_mouse.m_state= MOUSE_BUTTON_RELEASED;
                break;
            }
        }

#if defined(__PDCURSES__)
        if (BUTTON1_PRESSED & l_mouse.bstate || 
            BUTTON3_PRESSED & l_mouse.bstate)
#else
        if (BUTTON_PRESS(l_mouse.bstate, 1) || 
            BUTTON_PRESS(l_mouse.bstate, 3))
#endif
        {
            l_status= STATUS_PUSHED;
            (*o_event).m_record.m_mouse.m_state= MOUSE_BUTTON_CLICKED;
            break;
        }

        if (STATUS_PUSHED == l_status)
        {
            (*o_event).m_record.m_mouse.m_state= MOUSE_BUTTON_DRAGGING;
            break;
        }

        (*o_event).m_record.m_mouse.m_state= MOUSE_BUTTON_NONE;
        l_status= STATUS_CLEAR;

    }while(0);

    return;
}
#endif

static void
poll_get_key(
    struct poll_event *const            o_event,
    int const                           i_key)
{
#if !defined(__PDCURSES__)
    int                                 l_key2;
#endif

    do
    {

        (*o_event).m_type= EVENT_KEY;

        if (ASCII_ESC == i_key)
        {
#if !defined(__PDCURSES__)
            nodelay(stdscr, 1);
            l_key2= getch();
            nodelay(stdscr, 0);
            if (ERR == l_key2 || ASCII_ESC == l_key2)
            {
                (*o_event).m_record.m_key.m_ascii= ASCII_ESC;
                break;
            }
            else
            {
                (*o_event).m_record.m_key.m_scan= (SCAN_ESC_MASK|l_key2);
                break;
            }
#else
            (*o_event).m_record.m_key.m_ascii= ASCII_ESC;
            break;
#endif
        }

        if (KEY_ENTER == i_key)
        {
            (*o_event).m_record.m_key.m_ascii= ASCII_ENTER;
            break;
        }

        if (KEY_BACKSPACE == i_key)
        {
            (*o_event).m_record.m_key.m_ascii= ASCII_BS;
            break;
        }

        if (0x7f >= i_key)
        {
            switch(i_key)
            {
                case 0x7f:
                    (*o_event).m_record.m_key.m_ascii= ASCII_BS;
                    break;
                case 0x0a:
                    (*o_event).m_record.m_key.m_ascii= ASCII_ENTER;
                    break;
                default:
                    (*o_event).m_record.m_key.m_ascii= i_key;
                    break;
            }
            break;
        }

        (*o_event).m_record.m_key.m_scan= i_key;

    }while(0);

    return;
}

extern int
poll_block(
    struct poll_event *const            o_event)
{
    int                                 l_exit;
    int                                 l_key;

    l_exit= 0;

    do
    {

        memset(o_event, 0, sizeof(*o_event));

        if (_poll_key_unget_count)
        {
            (*o_event).m_type= EVENT_KEY;
            (*o_event).m_record.m_key= _poll_key_unget;
            _poll_key_unget_count= 0;
            l_exit= 1;
            break;
        }

        nodelay(stdscr, 0);
        l_key= getch();

        if (ERR == l_key)
        {
            l_exit= 0;
            break;
        }

#if defined(__PDCURSES__) || defined(NCURSES_MOUSE_VERSION)
        if (KEY_MOUSE == l_key)
        {
            poll_get_mouse(o_event);
            l_exit= 1;
            break;
        }
#endif

#if !defined(__DOS__)
        if (KEY_RESIZE == l_key)
        {
            (*o_event).m_type= EVENT_SCREEN_RESIZE;
            (*o_event).m_record.m_resize.m_len_x= COLS;
            (*o_event).m_record.m_resize.m_len_y= LINES;
            l_exit= 1;
            break;
        }
#endif

        poll_get_key(o_event, l_key);
        l_exit= 1;

    }while(0);

    return l_exit;
}
