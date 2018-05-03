/*
 MENU.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "menu.hpp"
#include "mouse.h"
#include "wm.hpp"
#include "wmhelper.hpp"
#include "block.h"
#include "skin.h"
#include "screen.h"
#include <ctype.h>

menu::menu(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    window(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_response_extended(false),
    m_selected(0),
    m_offset(0),
    m_orig_pos_y(0),
    m_array(),
    m_menu(0)
{
    array_assign(&m_array, 1);
    return;
}
    
menu::~menu()
{
    array_discharge(&m_array);
    return;
}

void
menu::draw_line(
    int const                           i_pos_y,
    int const                           i_erase) const
{

    if (0 == i_erase)
    {

        screen_putc(
            m_box.m_pos_x,
            i_pos_y,
            _skin_frame_single.m_middle_left,
            1,
            _skin_menu.m_window.m_fcolor,
            _skin_menu.m_window.m_bcolor);
    
        screen_putc(
            m_box.m_pos_x+1,
            i_pos_y,
            _skin_frame_single.m_hline,
            m_box.m_len_x-2,
            _skin_menu.m_window.m_fcolor,
            _skin_menu.m_window.m_bcolor);
    
        screen_putc(
            m_box.m_pos_x+m_box.m_len_x-1,
            i_pos_y,
            _skin_frame_single.m_middle_right,
            1,
            _skin_menu.m_window.m_fcolor,
            _skin_menu.m_window.m_bcolor);

    }
    else
    {
        screen_putc(
            m_box.m_pos_x,
            i_pos_y,
            _skin_frame_single.m_vline,
            1,
            _skin_menu.m_window.m_fcolor,
            _skin_menu.m_window.m_bcolor);
    
        screen_putc(
            m_box.m_pos_x+1,
            i_pos_y,
            0,
            m_box.m_len_x-2,
            _skin_menu.m_window.m_fcolor,
            _skin_menu.m_window.m_bcolor);
    
        screen_putc(
            m_box.m_pos_x+m_box.m_len_x-1,
            i_pos_y,
            _skin_frame_single.m_vline,
            1,
            _skin_menu.m_window.m_fcolor,
            _skin_menu.m_window.m_bcolor);
    }

    return;
}

void
menu::draw_item(
    int const                           i_pos_y,
    int const                           i_selected,
    struct menuitem const*const         i_menu) const
{
#if defined(__CURSES__)
    attr_t                              l_attr;
#endif
    enum background                     l_bcolor;
    unsigned char                       l_buf[76];
    unsigned char                       l_char;
    int                                 l_cols;
    enum foreground                     l_fcolor;
    size_t                              l_len;
    size_t                              l_len2;
    int                                 l_offset;

    do
    {

        if (0 == i_menu)
        {
            draw_line(i_pos_y, 0);
            break;
        }

        if (m_box.m_len_x < 6)
        {
            break;
        }
    
        memset(l_buf, ' ', sizeof(l_buf));
    
        l_len= strlen(reinterpret_cast<char const*>((*i_menu).m_label));
    
        if (sizeof(l_buf) <= l_len)
        {
            l_len= sizeof(l_buf) - 1;
        }
    
        l_cols= m_box.m_len_x - 4;
    
        if (static_cast<int>(l_cols) < static_cast<int>(l_len))
        {
            l_len= l_cols;
        }
    
        if (0 == (MENUITEM_CHECKBOX & (*i_menu).m_flags))
        {
            l_offset= 0;
        }
        else
        {
            l_buf[0]= '[';
            if (MENUITEM_VALUE & (*i_menu).m_flags)
            {
                l_buf[1]= _skin_buttons.m_checked;
            }
            l_buf[2]= ']';
            l_offset= 4;
            if (static_cast<int>(l_cols) < static_cast<int>(l_offset + l_len))
            {
                l_len= l_cols - l_offset;
            }
        }

        if (sizeof(l_buf) <= (l_offset + l_len))
        {
            l_len= sizeof(l_buf) - l_offset - 1;
        }
    
        memcpy(l_buf+l_offset, (*i_menu).m_label, l_len);
    
        if (MENUITEM_SUBMENU & (*i_menu).m_flags)
        {
            l_buf[l_cols-1]= _skin_buttons.m_submenu;
        }
        else if ((*i_menu).m_accel_label)
        {
            l_len2= strlen(reinterpret_cast<char const*>((*i_menu).m_accel_label));
            l_offset= l_cols-l_len2;
    
            if (0 < l_offset)
            {
                memcpy(l_buf+l_offset, (*i_menu).m_accel_label, l_len2);
            }
        }

#if defined(__CURSES__)
        l_attr= A_NORMAL;
#endif
    
        if (0 == i_selected)
        {
            if (0 == (MENUITEM_DISABLED & (*i_menu).m_flags))
            {
                l_fcolor= _skin_menu.m_window.m_text_fcolor;
                l_bcolor= _skin_menu.m_window.m_text_bcolor;
            }
            else
            {
                l_fcolor= _skin_menu.m_disabled_fcolor;
                l_bcolor= _skin_menu.m_disabled_bcolor;
#if defined(__CURSES__)
                l_attr= A_BOLD;
#endif
            }
        }
        else
        {
            l_bcolor= _skin_menu.m_selected_bcolor;
            if (0 == (MENUITEM_DISABLED & (*i_menu).m_flags))
            {
                l_fcolor= _skin_menu.m_selected_fcolor;
#if defined(__CURSES__)
                l_attr= A_REVERSE;
#endif
            }
            else
            {
                l_fcolor= _skin_menu.m_disabled_fcolor;
#if defined(__CURSES__)
                l_attr= A_BOLD;
#endif
            }
        }

#if defined(__CURSES__)
        if (!_video_has_color && A_NORMAL != l_attr)
        {
            attron(l_attr);	
        }
#endif
    
        screen_puts(
            m_box.m_pos_x+2,
            i_pos_y,
            l_buf,
            l_cols,
            l_fcolor,
            l_bcolor);

#if defined(__CURSES__)
        if (!_video_has_color && A_NORMAL != l_attr)
        {
            attroff(l_attr);	
        }
#endif
    
        if (MENUITEM_MNEMONIC_NONE != (*i_menu).m_mnemonic_offset)
        {
            l_fcolor= _skin_menu.m_hotkey_fcolor;
            if (0 == i_selected)
            {
                l_bcolor= _skin_menu.m_hotkey_bcolor;
            }
            else
            {
                l_bcolor= _skin_menu.m_selected_bcolor;
            }

            l_char= (*i_menu).m_label[(*i_menu).m_mnemonic_offset];

#if defined(__CURSES__)
            if (!_video_has_color)
            {
                attron(A_UNDERLINE);
            }
#endif

            screen_putc( 
                2+m_box.m_pos_x+l_offset+(*i_menu).m_mnemonic_offset,
                i_pos_y,
                l_char,
                1,
                l_fcolor,
                l_bcolor);

#if defined(__CURSES__)
            if (!_video_has_color)
            {
                attroff(A_UNDERLINE);
            }
#endif
        }

    }while(0);

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        ::move(i_pos_y, m_box.m_pos_x+2);
    }
#endif

    return;
}

void
menu::draw_items() const
{
    struct menuitem *                   l_menu;
    int                                 l_pos_y;
    unsigned int                        l_slot;
    unsigned int                        l_row_last;

    if (m_box.m_len_x < 6)
    {
        return;
    }

    l_slot= m_offset;
    l_pos_y= m_box.m_pos_y + 1;
    l_row_last= m_box.m_pos_y + m_box.m_len_y - 1;

    do
    {

        if (m_array.m_taken <= l_slot)
        {
            break;
        }

        if (static_cast<int>(l_row_last) <= static_cast<int>(l_pos_y))
        {
            break;
        }

        l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[l_slot]);

        if (l_menu)
        {
            draw_item(l_pos_y, (l_slot == m_selected), l_menu);
        }
        else
        {
            draw_line(l_pos_y, 0);
        }

        l_slot++;
        l_pos_y++;

    }while(1);

    return;
}
    
void
menu::draw() const
{

    mouse_hide();
    screen_push(1);

    screen_fill_area(
        m_box.m_pos_x,
        m_box.m_pos_y,
        m_box.m_len_x,
        m_box.m_len_y,
        0,
        _skin_menu.m_window.m_fcolor,
        _skin_menu.m_window.m_bcolor);

    wmhelper::draw_frame(
        m_box,
        _skin_frame_single,
        _skin_menu.m_window.m_fcolor,
        _skin_menu.m_window.m_bcolor);

    draw_items();

    screen_pop();
    mouse_show();

    return;
}

int
menu::event_in_parent(
    int const                           i_pos_x,
    int const                           i_pos_y)
{
    int                                 l_hit;
    widget*                             l_parent;

    l_hit= 0;

    l_parent= get_parent();

    do
    {

        if (0 == l_parent)
        {
            break;
        }

        l_hit= (*l_parent).contains(i_pos_x, i_pos_y);

        if (l_hit)
        {
            break;
        }

        l_parent= (*l_parent).get_parent();

    }while(1);

    return l_hit;
}

enum event_response
menu::event_key_ascii(
    unsigned char const                 i_ascii)
{
    enum event_response                 l_response;
    struct menuitem *                   l_menu;
    unsigned char                       l_char;
    unsigned int                        l_slot;

    l_response= RESPONSE_NONE;

    do
    {

        l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[m_selected]);

        if (l_menu && ASCII_ENTER == i_ascii)
        {

            if (MENUITEM_DISABLED & (*l_menu).m_flags)
            {
                break;
            }

            if (MENUITEM_CHECKBOX & (*l_menu).m_flags)
            {
                (*l_menu).m_flags^= MENUITEM_VALUE;
            }

            if (MENUITEM_SUBMENU & (*l_menu).m_flags)
            {
                l_response= submenu(
                    l_menu,
                    (m_box.m_pos_x+m_box.m_len_x),
                    (m_box.m_pos_y+m_selected-m_offset));
                break;
            }

            menuitem_emit(l_menu);
            l_response= RESPONSE_ACCEPT;
            break;
        }

        l_slot= 0;

        do
        {

            if (m_array.m_taken <= l_slot)
            {
                break;
            }

            l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[l_slot]);

            if (l_menu && 
                (0 == (MENUITEM_DISABLED & (*l_menu).m_flags)) &&
                (MENUITEM_MNEMONIC_NONE != (*l_menu).m_mnemonic_offset))
            {
                l_char= (*l_menu).m_label[(*l_menu).m_mnemonic_offset];
                if (toupper(l_char) == toupper(i_ascii))
                {
                    if (MENUITEM_SUBMENU & (*l_menu).m_flags)
                    {
                        select(l_slot);
                        l_response= submenu(
                            l_menu,
                            (m_box.m_pos_x+m_box.m_len_x),
                            (m_box.m_pos_y+m_selected-m_offset));
                        break;
                    }
                    menuitem_emit(l_menu);
                    l_response= RESPONSE_ACCEPT;
                    break;
                }
            }

            l_slot++;

        }while(1);

    }while(0);

    return l_response;
}

enum event_response
menu::event_key_scan(
    enum scancodes const                i_scan)
{
    enum event_response                 l_response;
    struct menuitem *                   l_menu;

    l_response= RESPONSE_NONE;

    do
    {
        if (0 == i_scan)
        {
            break;
        }

        if (SCAN_UP == i_scan)
        {
            select_previous();
            l_response= RESPONSE_HANDLED;
            break;
        }

        if (SCAN_DOWN == i_scan)
        {
            select_next();
            l_response= RESPONSE_HANDLED;
            break;
        }

        l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[m_selected]);

        if (SCAN_RIGHT == i_scan)
        {
            if (l_menu && 
                (MENUITEM_SUBMENU & (*l_menu).m_flags) &&
                (0 == (MENUITEM_DISABLED & (*l_menu).m_flags)))
            {
                l_response= submenu(
                    l_menu,
                    (m_box.m_pos_x+m_box.m_len_x),
                    (m_box.m_pos_y+m_selected));
            }
            else if (m_response_extended)
            {
                l_response= static_cast<enum event_response>(RESPONSE_RIGHT);
            }
            break;
        }

        if (SCAN_LEFT == i_scan)
        {
            if (get_parent())
            {
                l_response= RESPONSE_CANCEL;
            }
            else if (m_response_extended)
            {
                l_response= static_cast<enum event_response>(RESPONSE_LEFT);
            }
            break;
        }

        l_menu= menuitem_find_scancode(m_menu, i_scan);

        if (l_menu)
        {
            l_response= RESPONSE_ACCEPT;
            menuitem_emit(l_menu);
            break;
        }

    }while(0);

    return l_response;
}

enum event_response
menu::event_key(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;

    if (i_event.m_ascii)
    {
        l_response= event_key_ascii(i_event.m_ascii);
    }
    else
    {
        l_response= event_key_scan(i_event.m_scan);

    }

    return l_response;
}
    
enum event_response
menu::event_mouse(
    struct event_mouse const&           i_event)
{
    int                                 l_hit;
    struct menuitem *                   l_menu;
    enum event_response                 l_response;
    unsigned int                        l_slot;
    unsigned int                        l_visible_rows;
    int                                 l_pos_y;

    l_response= RESPONSE_NONE;

    do
    {

        if (MOUSE_BUTTON_RELEASED == i_event.m_state)
        {
            break;
        }

        l_hit= contains(i_event.m_pos_x, i_event.m_pos_y);

        if (0 == l_hit)
        {

            if (get_parent())
            {
                if (static_cast<int>(i_event.m_pos_y) != static_cast<int>(m_orig_pos_y))
                {
                    l_hit= event_in_parent(i_event.m_pos_x, i_event.m_pos_y);
                    if (l_hit)
                    {
                        l_response= RESPONSE_CANCEL;
                        break;
                    }
                }
            }

            if (MOUSE_BUTTON_CLICKED == i_event.m_state)
            {
                l_response= RESPONSE_CLOSE;
            }
            break;
        }

        if (0 == m_array.m_taken)
        {
            break;
        }

        if (static_cast<int>(i_event.m_pos_y) == static_cast<int>(m_box.m_pos_y))
        {
            if (m_offset)
            {
                m_offset--;
                m_selected= m_offset;
                draw();
            }
            break;
        }

        if (i_event.m_pos_y == m_box.m_pos_y + m_box.m_len_y - 1)
        {
            l_visible_rows= (m_box.m_len_y - 2);
            if (m_array.m_taken > l_visible_rows)
            {
                if (m_offset + l_visible_rows < m_array.m_taken)
                {
                    m_selected= m_offset + l_visible_rows;
                    m_offset++;
                    draw();
                }
            }
            break;
        }

        l_slot= i_event.m_pos_y - m_box.m_pos_y - 1;
        l_slot+= m_offset;

        if (m_array.m_taken <= l_slot)
        {
            break;
        }

        if (m_selected != l_slot)
        {
            mouse_hide();
            l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[m_selected]);
            if (l_menu)
            {
                l_pos_y= 1 + m_box.m_pos_y + m_selected - m_offset;
                draw_item(l_pos_y, 0, l_menu);
            }
            l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[l_slot]);
            if (l_menu)
            {
                draw_item(i_event.m_pos_y, 1, l_menu);
            }
            m_selected= l_slot;
            mouse_show();
        }

        if (i_event.m_state)
        {

            l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[l_slot]);

            if ((0 == l_menu) || (MENUITEM_DISABLED & (*l_menu).m_flags))
            {
                break;
            }

            if (MENUITEM_CHECKBOX & (*l_menu).m_flags)
            {
                (*l_menu).m_flags^= MENUITEM_VALUE;
            }

            if (MENUITEM_SUBMENU & (*l_menu).m_flags)
            {
                l_response= submenu(
                    l_menu,
                    (m_box.m_pos_x+m_box.m_len_x),
                    i_event.m_pos_y-1);
                break;
            }

            menuitem_emit(l_menu);
            l_response= RESPONSE_ACCEPT;
        }

    }while(0);

    return l_response;
}

void
menu::select(
    unsigned int const                  i_slot)
{
    unsigned int                        l_slot;
    unsigned int                        l_visible_rows;
    int                                 l_pos_y;
    struct menuitem*                    l_menu;

    l_slot= (m_array.m_taken > i_slot) ? i_slot : m_array.m_taken-1;

    do
    {

        if (m_selected == l_slot)
        {
            break;
        }

        l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[m_selected]);
        if (l_menu)
        {
            mouse_hide();
            l_pos_y= 1 + m_box.m_pos_y + m_selected - m_offset;
            draw_item(l_pos_y, 0, l_menu);
            mouse_show();
        }

        l_visible_rows= (m_box.m_len_y - 2);

        if (l_slot >= m_offset &&
            l_slot < m_offset + l_visible_rows)
        {
            m_selected= l_slot;
            l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[l_slot]);
            if (l_menu)
            {
                l_pos_y= 1 + m_box.m_pos_y + m_selected - m_offset;
                draw_item(l_pos_y, 1, l_menu);
            }
            break;
        }

        //_mjo
        // m_offset= l_visible_rows * (l_slot / l_visible_rows);

        if (l_slot < m_offset)
        {
            do
            {
                m_offset--;
                if (l_slot >= m_offset)
                {
                    break;
                }
            }while(1);
        }
        else
        {
            do
            {
                m_offset++;
                if (l_slot < m_offset + l_visible_rows)
                {
                    break;
                }
            }while(1);
        }

        m_selected= l_slot;
        draw();

    }while(0);


    return;
}

void
menu::select_next()
{
    struct menuitem*                    l_menu;
    unsigned int                        l_slot;

    l_slot= 1+m_selected;

    do
    {

        if (m_array.m_taken <= l_slot)
        {
            break;
        }

        l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[l_slot]);

        if (l_menu)
        {
            select(l_slot);
            break;
        }

        l_slot++;

    }while(1);

    return;
}

void
menu::select_previous()
{
    struct menuitem*                    l_menu;
    unsigned int                        l_slot;

    l_slot= m_selected;

    do
    {

        if (0 == l_slot)
        {
            break;
        }

        l_slot--;

        l_menu= reinterpret_cast<struct menuitem*>(m_array.m_base[l_slot]);

        if (l_menu)
        {
            select(l_slot);
            break;
        }

    }while(1);

    return;
}

void
menu::preflight()
{
    unsigned int                        l_rows;
    unsigned int                        l_cols;
    unsigned int                        l_sum;

    m_orig_pos_y= m_box.m_pos_y+1;

    menuitem_count(&l_rows, &l_cols, m_menu);
    m_array.m_growby= l_rows;
    menuitem_flatten(&m_array, m_menu);

    m_box.m_len_x= 4 + l_cols;
    m_box.m_len_y= 2 + l_rows;

    l_sum= m_box.m_pos_x + m_box.m_len_x;

    if (_video_cols < l_sum)
    {
        m_box.m_pos_x-= (l_sum - _video_cols);
        if (0 > m_box.m_pos_x)
        {
            m_box.m_pos_x= 0;
            m_box.m_len_x= _video_cols;
        }
    }

    l_sum= m_box.m_pos_y + m_box.m_len_y;

    if (_video_rows < l_sum)
    {
        m_box.m_pos_y-= (l_sum - _video_rows);
        if (0 > m_box.m_pos_y)
        {
            m_box.m_pos_y= 0;
            m_box.m_len_y= _video_rows;
        }
    }

    return;
}

enum event_response
menu::submenu(
    struct menuitem *                   io_menu,
    unsigned int const                  i_pos_x,
    unsigned int const                  i_pos_y)
{
    void __FAR*                         l_backing_store;
    struct box                          l_box;
    menu*                               l_menu;
    enum event_response                 l_response;
    struct menuitem*                    l_submenu;

    l_submenu= &io_menu[1];

    l_menu= new menu(i_pos_x, i_pos_y, _video_cols, _video_rows);
    (*l_menu).set_parent(this);
    (*l_menu).set_response_extended(m_response_extended);
    (*l_menu).set_menu(l_submenu);
    (*l_menu).get_box(l_box);

    mouse_hide();
    l_backing_store= block_read(&l_box);
    (*l_menu).draw();
    mouse_show();

    l_response= wm_run_modal(*l_menu);

    mouse_hide();
    block_write(l_backing_store, &l_box);
    mouse_show();

    block_free(l_backing_store);
    delete l_menu;

    if (RESPONSE_CANCEL == l_response)
    {
        l_response= RESPONSE_NONE;
    }

#if defined(__CURSES__)
    (*this).focus_enter();
#endif

    return l_response;
}

