/*
 WM.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "wm.hpp"
#include "wmhelper.hpp"
#include "hw.h"
#include "mouse.h"
#include "cursor.h"
#include "event.h"
#include "array.h"
#include "screen.h"
#include "clip.h"
#include "matrix.h"
#include "screen.h"
#include "poll.h"
#include "skin.h"
#include "fdtuicfg.h"
#if defined(__DOS__) || defined(__NT__) && !defined(__NTCRUSTY__)
#include <sys\types.h>
#include <dos.h>
#include <direct.h>
#else
#include <limits.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#if !defined(PATH_MAX)
#define PATH_MAX                        4096
#endif

#define ARRAY_GROWBY                    30
wmhelper                                _wm_helper;
static window*                          _wm_grab;
static window*                          _wm_focused;
static struct array                     _wm_array;
static enum wm_error                    _wm_quit_flag;
bool                                    _wm_esc_flag= false;
#if (__WATCOMC__)
static unsigned int                     _saved_drive;
#endif
static char                             _saved_directory[PATH_MAX+1];

#ifdef __DOS__

int __far _int24h_handler(
    unsigned int                        i_device_error,
    unsigned int                        i_error_code,
    unsigned __far *                    i_device_header)
{
    return _HARDERR_FAIL;
}

#endif

extern enum wm_error
wm_init()
{
    enum wm_error                       l_exit;
    int                                 l_rc;

    l_exit= ERROR_NONE;
    _wm_quit_flag= ERROR_NONE;
    _wm_grab= reinterpret_cast<window*>(0);
    _wm_focused= reinterpret_cast<window*>(0);
    array_assign(&_wm_array, ARRAY_GROWBY);

#if defined(__WATCOMC__)
    _dos_getdrive(&_saved_drive);
    _getdcwd(_saved_drive, _saved_directory, sizeof(_saved_directory));
#elif defined(__NT__)
    GetCurrentDirectoryA(sizeof(_saved_directory), _saved_directory);
#else
    getcwd(_saved_directory, sizeof(_saved_directory));
#endif

    do
    {

        fdtuicfg_load();
        l_rc= screen_init();

        if (l_rc)
        {
            l_exit= ERROR_INIT_SCREEN;
            break;
        }

        skin_set_defaults();

        cursor_hide();

        screen_fill(
            _skin_desktop.m_fillc,
            _skin_desktop.m_fcolor, 
            _skin_desktop.m_bcolor);

        (void)mouse_init();

#if defined(__DOS__) || defined(__NT__) || defined(__DJGPP__)
        hw_detect_drives();
#endif

        mouse_show();

#ifdef __DOS__
        _harderr(_int24h_handler);
#endif

    }while(0);

    return l_exit;
}

extern void
wm_deinit()
{

    clip_cleanup();
    matrix_cleanup();
    array_discharge(&_wm_array);
    mouse_deinit();
    screen_deinit();

#if defined(__WATCOM_C__)
    unsigned                            l_total;
    _dos_setdrive(_saved_drive, &l_total);
    _chdir(_saved_directory);
#elif defined(__NT__)
    SetCurrentDirectoryA(_saved_directory);
#else
    chdir(_saved_directory);
#endif

    return;
}

extern void
wm_quit(
    enum wm_error const                 i_reason)
{
    _wm_quit_flag= i_reason;
    return;
}

extern void
wm_draw(
    window *const                       i_window)
{
    window *                            l_last;
    window *                            l_window;
    unsigned int                        l_slot;

    l_last= (i_window) ? i_window : _wm_focused;

    mouse_hide();
    screen_push(0);

    screen_fill(
        _skin_desktop.m_fillc,
        _skin_desktop.m_fcolor, 
        _skin_desktop.m_bcolor);

    for (l_slot= 0; _wm_array.m_taken > l_slot; l_slot++)
    {
        l_window= reinterpret_cast<window*>(_wm_array.m_base[l_slot]);
        if ((l_last != l_window) && (*l_window).get_visible())
        {
            (*l_window).set_damage(widget::DAMAGE_ALL);
            (*l_window).draw();
            (*l_window).set_damage();
        }
    }

    if (l_last)
    {
        if ((*l_last).get_visible())
        {
            (*l_last).draw();
        }
        (*l_last).set_damage();
    }

    screen_pop();
    mouse_show();

    return;
}

extern void
wm_draw_widget(
    widget *const                       i_widget)
{
    struct box                          l_box;
    struct box                          l_clip;
    group const*                        l_parent;

    do
    {

        if (false == (*i_widget).get_visible())
        {
            break;
        }

        l_parent= (*i_widget).get_parent();

        if (0 == l_parent)
        {
            (*i_widget).draw();
            break;
        }

        (*l_parent).get_box(l_clip);
        (*l_parent).translate_screen(&l_clip.m_pos_x, &l_clip.m_pos_y);
        l_clip.m_len_x-= 2;
        l_clip.m_len_y-= 2;

        matrix_push();

        do
        {

            matrix_translate((*l_parent).get_pos_x()+1, (*l_parent).get_pos_y()+1);

            l_parent= (*l_parent).get_parent();

            if (0 == l_parent)
            {
                break;
            }

            (*l_parent).get_box(l_box);
            (*l_parent).translate_screen(&l_box.m_pos_x, &l_box.m_pos_y);
            l_box.m_len_x-= 2;
            l_box.m_len_y-= 2;
            clip(&l_clip, &l_box);

        }while(1);


        l_clip.m_pos_x= 0;
        l_clip.m_pos_y= 0;
        matrix_transform(&l_clip.m_pos_x, &l_clip.m_pos_y);
        clip_push(&l_clip);

        (*i_widget).draw();

        clip_pop();
        matrix_pop();

        (*i_widget).set_damage();


    }while(0);

    return;
}

extern void
wm_register_window(
    window*const                        i_window)
{
    int                                 l_found;
    unsigned                            l_slot;
    window*                             l_window;

    l_found= 0;
    for (l_slot= 0; _wm_array.m_taken > l_slot; l_slot++)
    {
        l_window= (window*)_wm_array.m_base[l_slot];
        if (i_window == l_window)
        {
            l_found= 1;
            break;
        }
    }

    if (0 == l_found)
    {
        array_push(&_wm_array, i_window);
        if (_wm_focused)
        {
            (*_wm_focused).focus_leave();
        }
        _wm_focused= i_window;
        (*_wm_focused).focus_enter();
    }

    return;
}

static void
wm_window_next()
{
    unsigned                            l_slot;
    window*                             l_window;

    l_window= 0;

    do
    {

        if (0 == _wm_focused)
        {
            l_window= (window*)_wm_array.m_base[0];
            break;
        }

        for (l_slot= 0; _wm_array.m_taken > l_slot; l_slot++)
        {
            l_window= (window*)_wm_array.m_base[l_slot];
            if (_wm_focused == l_window)
            {
                if (_wm_array.m_taken > (l_slot+1))
                {
                    l_window= (window*)_wm_array.m_base[(l_slot+1)];
                }
                else
                {
                    l_window= (window*)_wm_array.m_base[0];
                }
                break;
            }
        }

    }while(0);

    if (l_window && _wm_focused != l_window)
    {
        (*_wm_focused).focus_leave();
        _wm_focused= l_window;
         wm_draw(_wm_focused);
        (*_wm_focused).focus_enter();
    }

    return;
}

static enum event_response
wm_event_keyboard(
    struct event_key const&             i_key)
{
    enum event_response                 l_response;

    do
    {

        if (_wm_helper.capture())
        {
            l_response= _wm_helper.event_key(i_key);
            break;
        }

        if (_wm_grab)
        {
            l_response= (*_wm_grab).event_key(i_key);
            break;
        }

        l_response= _wm_helper.event_key(_wm_focused, i_key);

        if (RESPONSE_NONE != l_response)
        {
            break;
        }

        if (SCAN_F6 == i_key.m_scan)
        {
            wm_window_next();
            l_response= RESPONSE_HANDLED;
            break;
        }

        l_response= (*_wm_focused).event_key(i_key);

        if (RESPONSE_NONE != l_response)
        {
            break;
        }

        if (SCAN_NONE != i_key.m_scan)
        {
            l_response= (*_wm_focused).event_key_default(i_key.m_scan);
        }

    }while(0);

    return l_response;
}

static enum event_response
wm_event_mouse(
    struct event_mouse const&           i_mouse)
{
    enum event_response                 l_response;
    int                                 l_hit;
    size_t                              l_slot;
    window*                             l_window;

    l_response= RESPONSE_NONE;

    do
    {

        if (_wm_helper.capture())
        {
            l_response= _wm_helper.event_mouse(i_mouse);
            break;
        }

        if (_wm_grab)
        {
            l_response= (*_wm_grab).event_mouse(i_mouse);
            break;
        }

        l_hit= (*_wm_focused).contains(i_mouse.m_pos_x, i_mouse.m_pos_y);

        if (l_hit)
        {
            l_response= _wm_helper.event_mouse(_wm_focused, i_mouse);
            if (RESPONSE_NONE != l_response)
            {
                break;
            }
            if (_wm_focused)
            {
                l_response= (*_wm_focused).event_mouse(i_mouse);
            }
            break;
        }

        if (0 == (MOUSE_BUTTON_CLICKED & i_mouse.m_state))
        {
            break;
        }

        for (l_slot= 0; _wm_array.m_taken > l_slot; l_slot++)
        {
            l_window= reinterpret_cast<window*>(_wm_array.m_base[l_slot]);
            if (l_window != _wm_focused)
            {
                l_hit= (*l_window).contains(i_mouse.m_pos_x, i_mouse.m_pos_y);
                if (l_hit && (*l_window).get_can_focus())
                {
                    (*_wm_focused).focus_leave();
                    _wm_focused= l_window;
                    wm_draw(l_window);
                    (*l_window).focus_enter();
                    l_response= _wm_helper.event_mouse(_wm_focused, i_mouse);
                    break;
                }
            }
        }

    }while(0);

    return l_response;
}

static void
wm_unregister_all()
{
    size_t                              l_slot;
    window*                             l_window;

    for (l_slot= 0; _wm_array.m_taken > l_slot; l_slot++)
    {
        l_window= reinterpret_cast<window*>(_wm_array.m_base[l_slot]);
        (void)(*l_window).close(true);
        delete l_window;
    }

    _wm_grab= 0;
    _wm_focused= 0;
    _wm_array.m_taken= 0;

    return;
}

static void
wm_focus_set()
{
    unsigned int                        l_slot;

    l_slot= _wm_array.m_taken;

    do
    {

	if (0 == l_slot)
	{
	    break;
	}

	l_slot--;
        _wm_focused= reinterpret_cast<window*>(_wm_array.m_base[l_slot]);

	if ((*_wm_focused).get_visible())
	{
	    (*_wm_focused).focus_enter();
	    break;
	}

    }while(1);

    return;
}

extern enum wm_error
wm_run()
{
    enum wm_error                       l_error;
    int                                 l_exit;
    struct poll_event                   l_event;

    l_error= ERROR_NONE;

    do
    {

        if (_wm_quit_flag)
        {
            break;
        }

	if (0 == _wm_focused || false == (*_wm_focused).get_visible())
	{

	    wm_focus_set();

	    if (0 == _wm_focused)
	    {
		break;
	    }

	}

        l_exit= poll_block(&l_event);

        if (0 == l_exit)
        {
            break;
        }

        if (EVENT_KEY == l_event.m_type)
        {
            if (_wm_esc_flag && ASCII_ESC == l_event.m_record.m_key.m_ascii)
            {
                break;
            }
            wm_event_keyboard(l_event.m_record.m_key);
        }
        else if (EVENT_MOUSE == l_event.m_type)
        {
            wm_event_mouse(l_event.m_record.m_mouse);
        }
#if !defined(__DOS__) && !defined(__DJGPP__)
        else if (EVENT_SCREEN_RESIZE == l_event.m_type)
        {
            screen_changed_size();
            wm_draw(_wm_focused);
        }
#endif

    }while(1);

    wm_unregister_all();

    return l_error;
}

extern enum event_response
wm_run_modal(
    widget&                             i_widget)
{
    struct poll_event                   l_event;
    int                                 l_exit;
    enum event_response                 l_response;

    i_widget.focus_enter();

    do
    {

        l_response= RESPONSE_NONE;

        l_exit= poll_block(&l_event);

        if (0 == l_exit)
        {
            break;
        }

        if (EVENT_KEY == l_event.m_type)
        {
            if (ASCII_ESC == l_event.m_record.m_key.m_ascii)
            {
                l_response= RESPONSE_CLOSE;
                break;
            }
            l_response= i_widget.event_key(l_event.m_record.m_key);
        }
        else if (EVENT_MOUSE == l_event.m_type)
        {
            l_response= i_widget.event_mouse(l_event.m_record.m_mouse);
        }
#if !defined(__DOS__) && !defined(__DJGPP__)
        else if (EVENT_SCREEN_RESIZE == l_event.m_type)
        {
            screen_changed_size();
            screen_push(1);
            if (_wm_focused)
            {
                wm_draw(_wm_focused);
            }
            wm_draw_widget(&i_widget);
            screen_pop();
        }
#endif

        if (l_exit)
        {
            if (
                RESPONSE_NONE != l_response &&
                RESPONSE_HANDLED != l_response)
            {
                break;
            }
        }

    }while(1);

    return l_response;
}

extern window*const
wm_grab(
    window*const                        i_window)
{
    window*                             l_prev;

    l_prev= _wm_grab;
    _wm_grab= i_window;

    return l_prev;
}

extern void
wm_unregister_window(
    window*const                        i_window)
{
    unsigned                            l_slot;
    window*                             l_window;

    if (_wm_grab == i_window)
    {
        _wm_grab= 0;
    }

    if (_wm_focused == i_window)
    {
        (*_wm_focused).focus_leave();
        _wm_focused= 0;
    }

    for (l_slot= 0; _wm_array.m_taken > l_slot; l_slot++)
    {
        l_window= (window*)_wm_array.m_base[l_slot];
        if (i_window == l_window)
        {
            array_remove(&_wm_array, l_slot);
            break;
        }
    }

    if (_wm_array.m_taken)
    {
        _wm_focused= reinterpret_cast<window*>
            (_wm_array.m_base[_wm_array.m_taken-1]);
         wm_draw(_wm_focused);
         (*_wm_focused).focus_enter();
    }
    else
    {
        _wm_quit_flag= ERROR_EMPTY_QUEUE;
    }

    return;
}
