/*
 WMHELPER.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "wmhelper.hpp"
#include "wm.hpp"
#include "wmmenu.hpp"
#include "menu.hpp"
#include "mouse.h"
#include "block.h"
#include "skin.h"
#include <string.h>

unsigned int const                      wmhelper::MIN_WIDTH= 6;
unsigned int const                      wmhelper::MIN_HEIGHT= 2;

wmhelper::wmhelper() :
    m_window(0),
    m_mouse_x(0),
    m_mouse_y(0),
    m_event(EVENT_NONE),
    m_capture(CAPTURE_NONE)
{
    return;
}

wmhelper::~wmhelper()
{
    return;
}

// fixme: use draw.c draw_frame?
void
wmhelper::draw_frame(
    struct box const&                   i_box,
    struct skin_frame const&            i_skin,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    int                                 l_right;
    int                                 l_bottom;

    l_right= i_box.m_pos_x + i_box.m_len_x - 1;
    l_bottom= i_box.m_pos_y + i_box.m_len_y - 1;

    screen_putc(
        i_box.m_pos_x,
        i_box.m_pos_y,
        i_skin.m_hline,
        i_box.m_len_x,
        i_fcolor,
        i_bcolor);

    screen_putc(
        i_box.m_pos_x,
        l_bottom,
        i_skin.m_hline,
        i_box.m_len_x,
        i_fcolor,
        i_bcolor);

    screen_putvc(
        i_box.m_pos_x,
        i_box.m_pos_y,
        i_skin.m_vline,
        i_box.m_len_y,
        i_fcolor,
        i_bcolor);

    screen_putvc(
        l_right,
        i_box.m_pos_y,
        i_skin.m_vline,
        i_box.m_len_y,
        i_fcolor,
        i_bcolor);

    screen_putc(
        i_box.m_pos_x,
        i_box.m_pos_y,
        i_skin.m_top_left,
        1,
        i_fcolor,
        i_bcolor);

    screen_putc(
        l_right,
        i_box.m_pos_y,
        i_skin.m_top_right,
        1,
        i_fcolor,
        i_bcolor);

    screen_putc(
        i_box.m_pos_x,
        l_bottom,
        i_skin.m_bottom_left,
        1,
        i_fcolor,
        i_bcolor);

    screen_putc(
        l_right,
        l_bottom,
        i_skin.m_bottom_right,
        1,
        i_fcolor,
        i_bcolor);

    return;
}

void
wmhelper::draw_title(
    unsigned char const*                i_title,
    struct box const&                   i_box)
{
#define MAX_TITLE                       40
    unsigned char                       l_buf[40+3];
    size_t                              l_len;
    int                                 l_left;
    int                                 l_max;

    do
    {

        l_max= (i_box.m_len_x - (3+MIN_WIDTH));

        if (3 > l_max)
        {
            break;
        }

        l_buf[0]= _skin_frame_single.m_vline;

#if defined(__WATCOMC__) || defined(__BSD__)
        strlcpy(
            reinterpret_cast<char*>(l_buf+1),
            reinterpret_cast<char const*>(i_title),
            MAX_TITLE);
#else
        strncpy(
            reinterpret_cast<char*>(l_buf+1),
            reinterpret_cast<char const*>(i_title),
            MAX_TITLE-1);
         l_buf[MAX_TITLE]= 0;
#endif

        l_len= strlen(reinterpret_cast<char*>(l_buf));

        if (l_len > static_cast<size_t>(l_max))
        {
            l_len= l_max;
        }

        l_buf[l_len]= _skin_frame_single.m_vline;
        l_len++;
        l_buf[l_len]= 0;
        l_left= ((i_box.m_len_x / 2) - (l_len / 2));
        l_left+= i_box.m_pos_x;

        screen_puts(
            l_left,
            i_box.m_pos_y,
            l_buf,
            l_len,
            _skin_window.m_text_fcolor,
            _skin_window.m_text_bcolor);

    }while(0);

    return;
}

void
wmhelper::draw_decorate(
    struct box const&                   i_box,
    enum window::attributes const       i_attr,
    unsigned char const*                i_title)
{

    do
    {

        screen_fill_area(
            i_box.m_pos_x,
            i_box.m_pos_y,
            i_box.m_len_x,
            i_box.m_len_y,
            _skin_window.m_fillc,
            _skin_window.m_fcolor,
            _skin_window.m_bcolor);

        if (0 == (window::BORDER & i_attr))
        {
            break;
        }

        draw_frame(
            i_box,
            _skin_frame_single,
            _skin_window.m_fcolor,
            _skin_window.m_bcolor);

        if (window::SYSTEM & i_attr)
        {
            screen_putc(
                i_box.m_pos_x,
                i_box.m_pos_y,
                _skin_buttons.m_system,
                1,
                _skin_window.m_fcolor,
                _skin_window.m_bcolor);
        }

        if (window::MINMAX & i_attr)
        {
            screen_putc(
                i_box.m_pos_x+i_box.m_len_x-2,
                i_box.m_pos_y,
                _skin_buttons.m_maximize,
                1,
                _skin_window.m_fcolor,
                _skin_window.m_bcolor);
        
            screen_putc(
                i_box.m_pos_x+i_box.m_len_x-3,
                i_box.m_pos_y,
                _skin_buttons.m_minimize,
                1,
                _skin_window.m_fcolor,
                _skin_window.m_bcolor);
        }

        if (window::RESIZE & i_attr)
        {
            screen_putc(
                i_box.m_pos_x+i_box.m_len_x-1,
                i_box.m_pos_y+i_box.m_len_y-1,
                _skin_buttons.m_resize,
                1,
                _skin_window.m_fcolor,
                _skin_window.m_bcolor);
        }

        if (window::TITLE & i_attr)
        {
            if (i_title && i_title[0])
            {
                if (6 < i_box.m_len_x)
                {
                    draw_title(i_title, i_box);
                }
            }
        }

    }while(0);

    return;
}

enum event_response
wmhelper::event_mouse(
    window*const                        i_window,
    struct event_mouse const&           i_event)
{
    enum window::attributes             l_attr;
    struct box                          l_box;
    enum event_response                 l_response;
    unsigned int                        l_pos;
    int                                 l_right;

    m_window= i_window;
    m_event= EVENT_NONE;
    m_capture= CAPTURE_NONE;
    l_response= RESPONSE_NONE;

    do
    {

        if (0 == i_window)
        {
            break;
        }

        if (MOUSE_BUTTON_CLICKED != i_event.m_state)
        {
            break;
        }

        l_attr= (*i_window).get_attributes();

        if (0 == (window::BORDER & l_attr))
        {
            break;
        }

        (*i_window).get_box(l_box);

        if (window::SYSTEM & l_attr)
        {
            if ((static_cast<int>(i_event.m_pos_x) == static_cast<int>(l_box.m_pos_x)) &&
                (static_cast<int>(i_event.m_pos_y) == static_cast<int>(l_box.m_pos_y)))
            {
                event_system_menu();
                l_response= RESPONSE_HANDLED;
                break;
            }
        }

        if (window::MINMAX & l_attr)
        {

            l_pos= l_box.m_pos_x + l_box.m_len_x - 2;

            if ((static_cast<int>(i_event.m_pos_x) == static_cast<int>(l_pos)) &&
                (static_cast<int>(i_event.m_pos_y) == static_cast<int>(l_box.m_pos_y)))
            {
                maximize();
                m_capture= CAPTURE_NONE;
                m_event= EVENT_NONE;
                l_response= RESPONSE_HANDLED;
                break;
            }

            l_pos--;

            if ((i_event.m_pos_x == l_pos) &&
                ((int)i_event.m_pos_y == l_box.m_pos_y))
            {
                minimize();
                m_capture= CAPTURE_NONE;
                m_event= EVENT_NONE;
                l_response= RESPONSE_HANDLED;
                break;
            }
        }

        if (window::RESIZE & l_attr)
        {
            l_right=(l_box.m_pos_x + l_box.m_len_x) - 1;
            if ((int)i_event.m_pos_x == l_right)
            {
                l_right= (l_box.m_pos_y + l_box.m_len_y) - 1;
                if ((int)i_event.m_pos_y == l_right)
                {
                    m_mouse_x= i_event.m_pos_x;
                    m_mouse_y= i_event.m_pos_y;
                    m_event= EVENT_RESIZE;
                    m_capture= CAPTURE_MOUSE;
                    l_response= RESPONSE_HANDLED;
                    (*m_window).focus_leave(); 
                    break;
                }
            }
        }

        if (static_cast<int>(i_event.m_pos_y) == static_cast<int>(l_box.m_pos_y))
        {
            if (static_cast<int>(i_event.m_pos_x) >= static_cast<int>(l_box.m_pos_x))
            {
                l_right= (l_box.m_pos_x + l_box.m_len_x);
                if (static_cast<int>(i_event.m_pos_x) <= static_cast<int>(l_right))
                {
                    m_mouse_x= i_event.m_pos_x;
                    m_mouse_y= i_event.m_pos_y;
                    m_window= i_window;
                    m_event= EVENT_MOVE;
                    m_capture= CAPTURE_MOUSE;
                    l_response= RESPONSE_HANDLED;
                    (*m_window).focus_leave(); 
                    break;
                }
            }
        }

    }while(0);

    return l_response;
}

void
wmhelper::event_system_menu()
{
    struct box                          l_box;
    struct box                          l_new_box;
    struct box                          l_backing_box;
    menu*                               l_menu;
    void __FAR*                         l_backing_store;
    bool                                l_restore;

    (*m_window).focus_leave();
    (*m_window).get_box(l_box);
    l_menu= new menu(l_box.m_pos_x+1, l_box.m_pos_y+1, _video_cols, _video_rows);
    (*l_menu).set_menu(_system_menu_hook);
    (*l_menu).get_box(l_backing_box);

    mouse_hide();
    l_backing_store= block_read(&l_backing_box);
    (*l_menu).draw();
    mouse_show();
    wm_run_modal(*l_menu);
    delete l_menu;

    if (m_window && (*m_window).get_visible())
    {
        (*m_window).get_box(l_new_box);
        l_restore= box_equal(&l_new_box, &l_box);

        if (l_restore)
        {
            mouse_hide();
            block_write(l_backing_store, &l_backing_box);
            mouse_show();
        }
        (*m_window).focus_enter();
    }

    block_free(l_backing_store);

    return;
}

void
wmhelper::close()
{
    bool                                l_close;

    l_close= (*m_window).close(false);

    if (l_close)
    {
        wm_unregister_window(m_window);
        m_window= 0;
        m_event= EVENT_NONE;
        m_capture= CAPTURE_NONE;
    }

    return;
}

void
wmhelper::maximize()
{
    struct box                          l_box_before;
    struct box                          l_box_after;
    int                                 l_equal;

    (*m_window).get_box(l_box_before);
    (*m_window).move(0,0);
    (*m_window).resize(_video_cols, _video_rows);
    (*m_window).get_box(l_box_after);
    l_equal= box_equal(&l_box_before, &l_box_after);

    if (0 == l_equal)
    {
        mouse_hide();
        (*m_window).draw();
        mouse_show();
    }

    return;
}

void
wmhelper::minimize()
{
    struct box                          l_box_before;
    struct box                          l_box_after;
    int                                 l_equal;

    (*m_window).get_box(l_box_before);
    (*m_window).move(0, _video_rows - MIN_HEIGHT);
    (*m_window).resize(MIN_WIDTH, MIN_HEIGHT);
    (*m_window).get_box(l_box_after);
    l_equal= box_equal(&l_box_before, &l_box_after);

    if (0 == l_equal)
    {
        wm_draw(m_window);
    }

    return;
}

enum event_response
wmhelper::event_key(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;
    struct box                          l_box;
    struct box                          l_new_box;
    bool                                l_changed;

    l_response= RESPONSE_NONE;

    do
    {

        if (EVENT_NONE == m_event)
        {
            m_capture= CAPTURE_NONE;
            break;
        }

        if (ASCII_ENTER == i_event.m_ascii)
        {
            m_event= EVENT_NONE;
            m_capture= CAPTURE_NONE;
            l_response= RESPONSE_ACCEPT;
            (*m_window).focus_enter();
            break;
        }

        (*m_window).get_box(l_box);
        l_new_box= l_box;

        if (EVENT_MOVE == m_event)
        {

            l_changed= false;

            switch(i_event.m_scan)
            {
                case SCAN_UP:
                    if (l_new_box.m_pos_y)
                    {
                        l_new_box.m_pos_y--;
                        l_changed= true;
                    }
                    break;
                case SCAN_DOWN:
                    if (static_cast<int>(l_new_box.m_pos_y) < static_cast<int>((_video_rows-1)))
                    {
                        l_new_box.m_pos_y++;
                        l_changed= true;
                    }
                    break;
                case SCAN_LEFT:
                    if ((l_new_box.m_pos_x+l_new_box.m_len_x) > 1)
                    {
                        l_new_box.m_pos_x--;
                        l_changed= true;
                    }
                    break;
                case SCAN_RIGHT:
                    if ((int)(_video_cols-1) > l_new_box.m_pos_x)
                    {
                        l_new_box.m_pos_x++;
                        l_changed= true;
                    }
                    break;
                default:
                    break;
            }

            if (false == l_changed)
            {
                break;
            }

            (*m_window).move(l_new_box.m_pos_x, l_new_box.m_pos_y);
            (*m_window).set_damage(widget::DAMAGE_MOVED);
            wm_draw(m_window);

#if defined(__CURSES__)
            if (0 == (CURSOR_CAN_HIDE & _cursor_features))
            {
                if (0 > l_new_box.m_pos_x)
                {
                    l_new_box.m_pos_x+= l_new_box.m_len_x - 1;
                }
                ::move(l_new_box.m_pos_y, l_new_box.m_pos_x);
            }
#endif

            l_response= RESPONSE_HANDLED;
            break;
        }

        if (EVENT_RESIZE == m_event)
        {

            l_changed= false;

            switch(i_event.m_scan)
            {
                case SCAN_UP:
                    if (MIN_HEIGHT < l_new_box.m_len_y)
                    {
                        l_new_box.m_len_y--;
                        l_changed= true;
                    }
                    break;
                case SCAN_DOWN:
                    if (_video_rows > l_new_box.m_len_y)
                    {
                        l_new_box.m_len_y++;
                        l_changed= true;
                    }
                    break;
                case SCAN_LEFT:
                    if (MIN_WIDTH < l_new_box.m_len_x)
                    {
                        l_new_box.m_len_x--;
                        l_changed= true;
                    }
                    break;
                case SCAN_RIGHT:
                    if (_video_cols > l_new_box.m_len_x)
                    {
                        l_new_box.m_len_x++;
                        l_changed= true;
                    }
                    break;
                default:
                    break;
            }

            if (false == l_changed)
            {
                break;
            }

            (*m_window).resize(l_new_box.m_len_x, l_new_box.m_len_y);
            wm_draw(m_window);

#if defined(__CURSES__)
            if (0 == (CURSOR_CAN_HIDE & _cursor_features))
            {
                ::move(
                    l_new_box.m_pos_y + l_new_box.m_len_y - 1,
                    l_new_box.m_pos_x + l_new_box.m_len_x - 1);
            }
#endif

            l_response= RESPONSE_HANDLED;
            break;
        }

    }while(0);

    return l_response;
}

enum event_response
wmhelper::event_key(
    window*const                        i_window,
    struct event_key const&             i_event)
{
    enum window::attributes             l_attr;
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;
    l_attr= (*i_window).get_attributes();

    if (SCAN_ALT_PLUS == i_event.m_scan)
    {
        if (window::SYSTEM & l_attr)
        {
            m_window= i_window;
            event_system_menu();
            l_response= RESPONSE_HANDLED;
        }
    }

    return l_response;
}

enum event_response
wmhelper::event_mouse(
    struct event_mouse const&           i_event)
{
    struct box                          l_box;
    int                                 l_delta_x;
    int                                 l_delta_y;
    int                                 l_x;
    int                                 l_y;
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    do
    {

        if (CAPTURE_KEYBOARD == m_capture)
        {
            if (MOUSE_BUTTON_CLICKED == i_event.m_state)
            {
                m_capture= CAPTURE_NONE;
                m_event= EVENT_NONE;
                l_response= RESPONSE_ACCEPT;
                if (m_window)
                {
                    (*m_window).focus_enter();
                }
            }
            break;
        }

        if (MOUSE_BUTTON_DRAGGING != i_event.m_state)
        {
            m_event= EVENT_NONE;
            m_capture= CAPTURE_NONE;
            l_response= RESPONSE_ACCEPT;
            if (m_window)
            {
                (*m_window).focus_enter();
            }
            break;
        }

        l_response= RESPONSE_HANDLED;

        (*m_window).get_box(l_box);

        l_delta_x= i_event.m_pos_x - m_mouse_x;
        l_delta_y= i_event.m_pos_y - m_mouse_y;

        m_mouse_x= i_event.m_pos_x;
        m_mouse_y= i_event.m_pos_y;

        if (EVENT_MOVE == m_event)
        {
            l_x= l_box.m_pos_x + l_delta_x;

            if (0 > static_cast<int>(l_x + l_box.m_len_x))
            {
                l_x= 1 - l_box.m_len_x;
            }

            l_y= l_box.m_pos_y + l_delta_y;

            if (l_x != l_box.m_pos_x || l_y != l_box.m_pos_y)
            {
                (*m_window).move(l_x, l_y);
                (*m_window).set_damage(widget::DAMAGE_MOVED);
                wm_draw(m_window);
            }

            break;
        }

        if (EVENT_RESIZE == m_event)
        {
            l_x= l_box.m_len_x + l_delta_x;

            if (static_cast<int>(MIN_WIDTH) > static_cast<int>(l_x))
            {
                l_x= MIN_WIDTH;
            }
            else if (static_cast<int>(_video_cols) <= static_cast<int>(l_x))
            {
                l_x= _video_cols;
            }

            l_y= l_box.m_len_y + l_delta_y;

            if (static_cast<int>(_video_rows) <= static_cast<int>(l_box.m_pos_y + l_y))
            {
                l_y= _video_rows - l_box.m_pos_y;
            }

            if (static_cast<int>(MIN_HEIGHT) > static_cast<int>(l_y))
            {
                l_y= MIN_HEIGHT;
            }

            if (static_cast<int>(l_x) != static_cast<int>(l_box.m_len_x) || 
                static_cast<int>(l_y) != static_cast<int>(l_box.m_len_y))
            {
                (*m_window).resize(l_x, l_y);
                wm_draw(m_window);
            }
        }

    }while(0);

    return l_response;
}
