/*
 LISBBOX.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "listbox.hpp"
#include "skin.h"
#include "draw.h"
#include "clip.h"
#include "matrix.h"
#include "wm.hpp"
#include "mouse.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

struct listbox_item
{
    unsigned char*                      m_label;
    bool                                m_selected;
};

void
listbox::vertical_cb(
    widget const*                       /*i_widget*/,
    void*                               io_user_data)
{
    listbox*                            l_listbox;

    l_listbox= reinterpret_cast<listbox*>(io_user_data);
    (*l_listbox).scroll_internal();

    return;
}

listbox::listbox(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_multiline(false),
    m_line(0),
    m_array(),
    m_vertical(i_pos_x+i_len_x-1, i_pos_y+1, 1, i_len_y-2),
    m_label(0),
    m_response(RESPONSE_HANDLED),
    m_signal_selected(0)
{
#define GROWBY 30
    array_assign(&m_array, GROWBY);
    m_vertical.set_debounce(true);
    m_vertical.set_orientation(slider::VERTICAL);
    m_vertical.set_range(0, 0);
    m_vertical.set_user_data(static_cast<void*>(this));
    m_vertical.set_signal_value(vertical_cb);
    m_vertical.set_hidden();
}
    
listbox::~listbox()
{

    remove_all();
    array_discharge(&m_array);
    free(m_label);

    return;
}

void
listbox::move(
    int const                           i_pos_x,
    int const                           i_pos_y)
{

    widget::move(i_pos_x, i_pos_y);
    m_vertical.move(m_box.m_pos_x + m_box.m_len_x - 1, i_pos_y + 1);

    return;
}

void
listbox::resize(
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y)
{

    widget::resize(i_len_x, i_len_y);
    m_vertical.resize(1, m_box.m_len_y - 2);
    vertical_configure();

    return;
}

int
listbox::add(
    unsigned char const*                i_label)
{
    int                                 l_exit;
    struct listbox_item*                l_item;
    size_t                              l_len;

    l_exit= -1;

    do
    {

        if (0 == i_label || 0 == i_label[0])
        {
            break;
        }

        l_item= reinterpret_cast<struct listbox_item*>(malloc(sizeof(*l_item)));

        if (0 == l_item)
        {
            break;
        }

        (*l_item).m_selected= 0;
        l_len= 1+strlen(reinterpret_cast<char const*>(i_label));
        (*l_item).m_label= reinterpret_cast<unsigned char*>(malloc(l_len));

        if (0 == (*l_item).m_label)
        {
            free(l_item);
            break;
        }

        memcpy((*l_item).m_label, i_label, l_len);

        l_exit= array_push(&m_array, reinterpret_cast<void*>(l_item));

        if (l_exit)
        {
            free((*l_item).m_label);
            free(l_item);
        }

    }while(0);

    vertical_configure();

    return l_exit;
}

void
listbox::unselect_all()
{
    struct listbox_item *               l_item;
    size_t                              l_slot;

    for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
    {
        l_item= reinterpret_cast<struct listbox_item*>(m_array.m_base[l_slot]);
        (*l_item).m_selected= false;
    }

    m_line= 0;

    return;
}

void
listbox::set_selected(
    size_t const                        i_slot,
    bool const                          i_selected)
{
    struct listbox_item *               l_item;

    if (false == m_multiline)
    {
        unselect_all();
    }

    if (m_array.m_taken > i_slot)
    {
        l_item= reinterpret_cast<struct listbox_item*>(m_array.m_base[i_slot]);
        (*l_item).m_selected= i_selected;
    }

    return;
}

void
listbox::select(
    size_t const                        i_slot)
{
    struct listbox_item *               l_item;
    size_t                              l_slot;
    bool                                l_value;

    do
    {

        if (m_array.m_taken <= i_slot)
        {
            break;
        }

        if (false == m_multiline)
        {
            for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
            {
                if (l_slot != i_slot)
                {
                    l_item= reinterpret_cast<struct listbox_item*>(
                        m_array.m_base[l_slot]);
                    (*l_item).m_selected= false;
                }
            }
        }
    
        l_item= reinterpret_cast<struct listbox_item*>(m_array.m_base[i_slot]);
        l_value= (*l_item).m_selected;
    
        if (false == m_multiline)
        {
            (*l_item).m_selected= true;
        }
        else
        {
            (*l_item).m_selected= !(*l_item).m_selected;
        }
    
        set_damage(DAMAGE_VALUE);
        wm_draw_widget(this);
    
        if (l_value != (*l_item).m_selected)
        {
            emit_selected();
        }

    }while(0);

    return;
}

void
listbox::draw() const
{
#if defined(__CURSES__)
    attr_t                              l_attr;
#endif
    enum background                     l_bcolor;
    enum foreground                     l_fcolor;
    struct listbox_item const*          l_item;
    size_t                              l_length;
    size_t                              l_line;
    size_t                              l_slot;

    mouse_hide();
    screen_push(1);

    draw_fill_area(
        m_box.m_pos_x,
        m_box.m_pos_y,
        m_box.m_len_x,
        m_box.m_len_y,
        0,
        _skin_listbox.m_fcolor,
        _skin_listbox.m_bcolor);

    draw_frame(
        &m_box,
        &_skin_frame_single,
        _skin_listbox.m_fcolor,
        _skin_listbox.m_bcolor);

    if (m_label)
    {

        l_length= strlen(reinterpret_cast<char const*>(m_label));

        if (m_box.m_len_x <= l_length)
        {
            l_length= m_box.m_len_x - 2;
        }

        draw_puts(
            m_box.m_pos_x+1,
            m_box.m_pos_y,
            m_label,
            l_length,
            _skin_listbox.m_fcolor,
            _skin_listbox.m_bcolor);
    }

    l_line= 0;
    l_slot= static_cast<size_t>(m_vertical.get_value());

    do
    {

        if ((m_box.m_len_y - 2) <= l_line)
        {
            break;
        }

        if (m_array.m_taken <= l_slot)
        {
            break;
        }

        l_item= reinterpret_cast<struct listbox_item const*>(m_array.m_base[l_slot]);
        l_length= strlen(reinterpret_cast<char const*>((*l_item).m_label));

        if (m_box.m_len_x - 2 < l_length)
        {
            l_length= m_box.m_len_x - 2;
        }

#if defined(__CURSES__)
        l_attr= A_NORMAL;
#endif

        if (l_line != m_line)
        {
            if (!(*l_item).m_selected)
            {
                l_fcolor= _skin_listbox.m_fcolor;
                l_bcolor= _skin_listbox.m_bcolor;
            }
            else
            {
                l_fcolor= _skin_listbox.m_selected_fcolor;
                l_bcolor= _skin_listbox.m_selected_bcolor;
#if defined(__CURSES__)
                l_attr= A_UNDERLINE;
#endif
            }
        }
        else
        {
            l_fcolor= _skin_listbox.m_highlight_fcolor;
            l_bcolor= _skin_listbox.m_highlight_bcolor;
#if defined(__CURSES__)
            l_attr= A_REVERSE;
#endif
        }

#if defined(__CURSES__)
        if (!_video_has_color && A_NORMAL != l_attr)
        {
            attron(l_attr);	
        }
#endif

        draw_putc(
            m_box.m_pos_x + 1,
            m_box.m_pos_y + l_line + 1,
            0,
            m_box.m_len_x - 2,
            l_fcolor,
            l_bcolor);

        draw_puts(
            m_box.m_pos_x + 1,
            m_box.m_pos_y + l_line + 1,
            (*l_item).m_label,
            l_length,
            l_fcolor,
            l_bcolor);

#if defined(__CURSES__)
        if (!_video_has_color && A_NORMAL != l_attr)
        {
            attroff(l_attr);	
        }
#endif

        l_line++;
        l_slot++;

    }while(1);

    if (m_vertical.get_visible())
    {
        m_vertical.draw();
    }

    screen_pop();
    mouse_show();

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        int                             l_cur_x;
        int                             l_cur_y;
        translate_screen(&l_cur_x, &l_cur_y);
        l_cur_y+= m_line + 1;
        ::move(l_cur_y, l_cur_x);
    }
#endif

    return;
}

enum event_response
listbox::event_mouse(
    struct event_mouse const&           i_event)
{
    bool                                l_hit;
    size_t                              l_line;
    enum event_response                 l_response;
    size_t                              l_slot;

    l_response= RESPONSE_NONE;

    do
    {

        l_hit= m_vertical.contains(i_event.m_pos_x, i_event.m_pos_y);

        if (l_hit)
        {
            l_response= m_vertical.event_mouse(i_event);
            break;
        }

        if ((static_cast<int>(i_event.m_pos_y) <= static_cast<int>(m_box.m_pos_y)) ||
            (static_cast<int>(i_event.m_pos_y) >= static_cast<int>(m_box.m_pos_y + m_box.m_len_y - 1)))
        {
            break;
        }

        if (MOUSE_BUTTON_CLICKED != i_event.m_state)
        {
            break;
        }
        
        l_line= (i_event.m_pos_y - m_box.m_pos_y - 1);

        if (false == m_multiline || l_line == m_line)
        {
            m_line= l_line;
            l_slot= static_cast<size_t>(m_vertical.get_value());
            l_slot+= m_line;
            select(l_slot);
            l_response= m_response;
        }
        else
        {
            m_line= l_line;
            set_damage(DAMAGE_VALUE);
            wm_draw_widget(this);
        }

    }while(0);

    return l_response;
}

enum event_response
listbox::event_key(
    struct event_key const&             i_event)
{
    bool                                l_damaged;
    size_t                              l_offset;
    enum event_response                 l_response;
    double                              l_value;

    l_damaged= false;
    l_value= m_vertical.get_value();
    l_offset= static_cast<size_t>(l_value);
    l_response= RESPONSE_NONE;

    do
    {

        if (ASCII_SPACE == i_event.m_ascii ||
            ASCII_ENTER == i_event.m_ascii)
        {
            select(m_line + l_offset);
	    l_damaged= false;
            l_response= m_response;
            break;
        }

        if (1 >= m_array.m_taken)
        {
            break;
        }

        if (SCAN_UP == i_event.m_scan)
        {
            if (m_line)
            {
                m_line--;
                l_damaged= true;
            }
            else if (l_offset)
            {
                l_value= m_vertical.increment(l_value, 1);
                l_value= m_vertical.clamp(l_value);
                l_damaged= m_vertical.set_value(l_value);
            }
            break;
        }

        if (SCAN_DOWN == i_event.m_scan)
        {
            if (m_line < m_box.m_len_y - 3)
            {
                if (m_array.m_taken - 1 > m_line + l_offset)
                {
                    m_line++;
                    l_damaged= true;
                }
            }
            else if (m_array.m_taken - 1 > m_line + l_offset)
            {
                l_value= m_vertical.increment(l_value, -1);
                l_value= m_vertical.clamp(l_value);
                l_damaged= m_vertical.set_value(l_value);
            }
            break;
        }

    }while(0);

    if (l_damaged)
    {
        if (false == m_multiline)
        {
            l_offset= static_cast<size_t>(m_vertical.get_value());
            select(m_line + l_offset);
        }
        else
        {
            set_damage(DAMAGE_VALUE);
            wm_draw_widget(this);
        }
    }

    return l_response;
}

void
listbox::scroll_internal()
{

    set_damage(DAMAGE_VALUE);
    wm_draw_widget(this);

    return;
}

unsigned char const*
listbox::get_item(
    size_t const                        i_slot) const
{
    unsigned char const*                l_label;
    struct listbox_item const*          l_item;

    l_label= 0;

    if (m_array.m_taken > i_slot)
    {
        l_item= reinterpret_cast<struct listbox_item const*>(
            m_array.m_base[i_slot]);
        l_label= (*l_item).m_label;
    }

    return l_label;
}

int
listbox::get_selected_first() const
{
    int                                 l_selected;
    size_t                              l_slot;
    struct listbox_item const*          l_item;

    l_selected= -1;

    for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
    {
        l_item= reinterpret_cast<struct listbox_item const*>(
            m_array.m_base[l_slot]);

        if ((*l_item).m_selected)
        {
            l_selected= static_cast<int>(l_slot);
            break;
        }

    }

    return l_selected;
}

int
listbox::get_selected_next(
    int const                           i_iter) const
{
    struct listbox_item const*          l_item;
    size_t                              l_slot;
    int                                 l_selected;

    l_selected= -1;

    do
    {
        if (0 > i_iter)
        {
            break;
        }

        l_slot= static_cast<size_t>(i_iter+1);

        for (; m_array.m_taken > l_slot; l_slot++)
        {
            l_item= reinterpret_cast<struct listbox_item const*>(
                m_array.m_base[l_slot]);
    
            if ((*l_item).m_selected)
            {
                l_selected= static_cast<int>(l_slot);
                break;
            }
        }

    }while(0);

    return l_selected;
}

void
listbox::remove(
    size_t const                        i_slot)
{
    struct listbox_item*                l_item;

    l_item= reinterpret_cast<struct listbox_item*>(
        array_remove(&m_array, i_slot));

    if (l_item)
    {
        free((*l_item).m_label);
        free(l_item);
        vertical_configure();
    }

    return;
}

void
listbox::remove_all()
{
    struct listbox_item*                l_item;

    do
    {

        l_item= reinterpret_cast<struct listbox_item*>(array_pop(&m_array));

        if (0 == l_item)
        {
            break;
        }

        free((*l_item).m_label);
        free(l_item);

    }while(1);

    m_line= 0;

    vertical_configure();

    return;
}

void
listbox::vertical_configure()
{
    int                                 l_max_y;

    l_max_y= m_array.m_taken - (m_box.m_len_y - 2);

    if (0 > l_max_y)
    {
        l_max_y= 0;
    }

    m_vertical.set_range(l_max_y, 0);
    m_vertical.set_value(0);

    if (m_array.m_taken > m_box.m_len_y - 2)
    {
        m_vertical.set_visible();
    }
    else
    {
        m_vertical.set_hidden();
    }

    return;
}
