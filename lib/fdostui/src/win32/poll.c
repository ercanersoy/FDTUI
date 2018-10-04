/*
 POLL.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "poll.h"
#include "mouse.h"

extern HANDLE                           _screen_stdin;

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

extern int
poll_block(
    struct poll_event *const            o_event)
{
    static struct event_mouse           _prev= {0,0,MOUSE_BUTTON_NONE};
    INPUT_RECORD                        l_event;
    int                                 l_exit;
    KEY_EVENT_RECORD const*             l_key;
    MOUSE_EVENT_RECORD const*           l_mouse;
    DWORD                               l_records;
    WINDOW_BUFFER_SIZE_RECORD const*    l_resize;

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

        ReadConsoleInputA(_screen_stdin, &l_event, 1, &l_records);

        if (0 == l_records)
        {
            l_exit= 0;
            break;
        }

        if (KEY_EVENT == l_event.EventType)
        {
            l_key= &l_event.Event.KeyEvent;
            if ((*l_key).bKeyDown)
            {
                (*o_event).m_type= EVENT_KEY;
                if ((LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED) & (*l_key).dwControlKeyState)
                {
                    if (0 == (*l_key).uChar.AsciiChar)
                    {
                        continue;
                    }
                    (*o_event).m_record.m_key.m_scan= (SCAN_ALT_MASK|(*l_key).uChar.AsciiChar);
                }
                else if ((SHIFT_PRESSED & (*l_key).dwControlKeyState) &&
                    (ASCII_TAB == (*l_key).uChar.AsciiChar))
                {
                    (*o_event).m_record.m_key.m_scan= SCAN_SHIFT_TAB;
                }
                else
                {
                    (*o_event).m_record.m_key.m_ascii= (*l_key).uChar.AsciiChar;
                    (*o_event).m_record.m_key.m_scan= (*l_key).wVirtualKeyCode;
                }
                l_exit= 1;
                break;
            }
        }
        else if (MOUSE_EVENT == l_event.EventType)
        {
            l_mouse= &l_event.Event.MouseEvent;
            (*o_event).m_type= EVENT_MOUSE;
            (*o_event).m_record.m_mouse.m_pos_x= (*l_mouse).dwMousePosition.X;
            (*o_event).m_record.m_mouse.m_pos_y= (*l_mouse).dwMousePosition.Y;
    
            if ((*l_mouse).dwButtonState)
            {
                if ((MOUSE_BUTTON_CLICKED == _prev.m_state) ||
                    (MOUSE_BUTTON_DRAGGING == _prev.m_state))
                {
                    (*o_event).m_record.m_mouse.m_state= MOUSE_BUTTON_DRAGGING;
                }
                else
                {
                    (*o_event).m_record.m_mouse.m_state= MOUSE_BUTTON_CLICKED;
                }
            }
            else
            {
                if (MOUSE_BUTTON_DRAGGING == _prev.m_state)
                {
                    (*o_event).m_record.m_mouse.m_state= MOUSE_BUTTON_RELEASED;
                }
                else
                {
                    (*o_event).m_record.m_mouse.m_state= MOUSE_BUTTON_NONE;
                }
            }

            _prev= (*o_event).m_record.m_mouse;
            l_exit= 1;
            break;
        }
        else if (WINDOW_BUFFER_SIZE_EVENT == l_event.EventType)
        {
            l_resize= &l_event.Event.WindowBufferSizeEvent;
            (*o_event).m_type= EVENT_SCREEN_RESIZE;
            (*o_event).m_record.m_resize.m_len_x= (*l_resize).dwSize.X;
            (*o_event).m_record.m_resize.m_len_y= (*l_resize).dwSize.Y;
            l_exit= 1;
            break;
        }

    } while(1);

    return l_exit;
}
