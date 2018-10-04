/*
 SCROLLER.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "scroller.hpp"
#include "wm.hpp"
#include "skin.h"
#include "draw.h"
#include "matrix.h"

void
scroller::horizontal_cb(
    widget const*                       /*i_widget*/,
    void*                               io_user_data)
{
    scroller*                           l_scroller;

    l_scroller= reinterpret_cast<scroller*>(io_user_data);
    (*l_scroller).scroll_internal();

    return;
}

void
scroller::vertical_cb(
    widget const*                       /*i_widget*/,
    void*                               io_user_data)
{
    scroller*                           l_scroller;

    l_scroller= reinterpret_cast<scroller*>(io_user_data);
    (*l_scroller).scroll_internal();

    return;
}

scroller::scroller(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    group(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_horizontal(i_pos_x, i_pos_y+i_len_y-1, i_len_x-1, 1),
    m_vertical(i_pos_x+i_len_x-1, i_pos_y, 1, i_len_y-1),
    m_scroll_pos_x(0),
    m_scroll_pos_y(0)
{
    m_horizontal.set_debounce(true);
    m_horizontal.set_orientation(slider::HORIZONTAL);
    m_horizontal.set_range(0, 0);
    m_horizontal.set_user_data(static_cast<void*>(this));
    m_horizontal.set_signal_value(horizontal_cb);
    m_vertical.set_debounce(true);
    m_vertical.set_orientation(slider::VERTICAL);
    m_vertical.set_range(0, 0);
    m_vertical.set_user_data(static_cast<void*>(this));
    m_vertical.set_signal_value(vertical_cb);
    return;
}

scroller::~scroller()
{
    return;
}

void
scroller::draw() const
{

    screen_push(1);

    draw_fill_area(
        m_box.m_pos_x,
        m_box.m_pos_y,
        m_box.m_len_x,
        m_box.m_len_y,
        0,
        _skin_window.m_fcolor,
        _skin_window.m_bcolor);

    group::draw();

    m_horizontal.draw();
    m_vertical.draw();

    screen_pop();

    return;
}

enum event_response
scroller::event_key(
    struct event_key const&             i_event)
{
    struct box                          l_box;
    widget const*                       l_focused;
    enum event_response                 l_response;
    bool                                l_scroll;

    l_focused= m_focused;
    l_response= group::event_key(i_event);
    l_scroll= false;

    do
    {

        if (l_focused == m_focused)
        {
            break;
        }

        (*m_focused).get_box(l_box);

        if (static_cast<unsigned int>(l_box.m_pos_x) > (m_box.m_len_x - 2))
        {
            int                         l_scroll_x;
            l_scroll_x= static_cast<int>(m_horizontal.get_value());
            m_horizontal.set_value(m_horizontal.clamp(
                static_cast<double>(l_box.m_pos_x+l_scroll_x)));
            l_scroll= true;
        }

        if (static_cast<unsigned int>(l_box.m_pos_y) >= (m_box.m_len_y - 2))
        {
            int                         l_scroll_y;
            l_scroll_y= static_cast<int>(m_vertical.get_value());
            m_vertical.set_value(m_vertical.clamp(
                static_cast<double>(l_box.m_pos_y+l_scroll_y)));
            l_scroll= true;
        }


    }while(0);

    if (l_scroll)
    {
        scroll_internal();
    }

    return l_response;
}

enum event_response
scroller::event_mouse(
    struct event_mouse const&           i_event)
{
    int                                 l_hit;
    enum event_response                 l_response;

    do
    {

        l_hit= m_horizontal.contains(i_event.m_pos_x, i_event.m_pos_y);

        if (l_hit)
        {
            l_response= m_horizontal.event_mouse(i_event);
            break;
        }

        l_hit= m_vertical.contains(i_event.m_pos_x, i_event.m_pos_y);

        if (l_hit)
        {
            l_response= m_vertical.event_mouse(i_event);
            break;
        }

        l_response= group::event_mouse(i_event);

    }while(0);

    return l_response;
}

void
scroller::scroll_internal()
{
    int                                 l_pos_x;
    int                                 l_pos_y;
    int                                 l_delta_x;
    int                                 l_delta_y;
    size_t                              l_slot;
    widget*                             l_widget;
#if defined(__CURSES__)
    widget*                             l_widget_focused= 0;
#endif

    l_pos_x= static_cast<int>(m_horizontal.get_value());
    l_pos_y= static_cast<int>(m_vertical.get_value());

    l_delta_x= m_scroll_pos_x - l_pos_x;
    l_delta_y= m_scroll_pos_y - l_pos_y;

    do
    {

        if (0 == l_delta_x && 0 == l_delta_y)
        {
            break;
        }

        m_scroll_pos_x= l_pos_x;
        m_scroll_pos_y= l_pos_y;

        for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
        {
            l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
            l_pos_x= (*l_widget).get_pos_x() + l_delta_x;
            l_pos_y= (*l_widget).get_pos_y() + l_delta_y;
            (*l_widget).move(l_pos_x, l_pos_y);
#if defined(__CURSES__)
            if ((*l_widget).get_has_focus())
            {
                l_widget_focused= l_widget;
            }
#endif
        }

        set_damage(DAMAGE_SCROLL);
        wm_draw_widget(this);

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        int                             l_cur_x;
        int                             l_cur_y;
        if (l_widget_focused)
        {
            (*l_widget_focused).translate_screen(&l_cur_x, &l_cur_y);
        }
        else
        {
            translate_screen(&l_cur_x, &l_cur_y);
        }
        ::move(l_cur_y, l_cur_x);
    }
#endif

    }while(0);

    return;
}

void
scroller::set_scroll_bounds()
{
    struct box                          l_box;
    int                                 l_max_x;
    int                                 l_max_y;
    int                                 l_min_x;
    int                                 l_min_y;
    int                                 l_top;
    size_t                              l_slot;
    widget const*                       l_widget;

    l_min_x= 0;
    l_max_x= 0;
    l_min_y= 0;
    l_max_y= 0;

    for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
    {
        l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
        (*l_widget).get_box(l_box);

        if (l_min_x > l_box.m_pos_x)
        {
            l_min_x= l_box.m_pos_x;
        }

        l_top= l_box.m_pos_x + l_box.m_len_x;
        if (l_max_x < l_top)
        {
            l_max_x= l_top;
        }

        if (l_min_y > l_box.m_pos_y)
        {
            l_min_y= l_box.m_pos_y;
        }

        l_top= l_box.m_pos_y + l_box.m_len_y;
        if (l_max_y < l_top)
        {
            l_max_y= l_top;
        }
    }

    l_max_x-= (m_box.m_len_x - 2);

    if (0 > l_max_x)
    {
        l_max_x= 0;
    }

    m_horizontal.set_range(l_min_x, l_max_x);

    l_max_y-= (m_box.m_len_y - 2);

    if (0 > l_max_y)
    {
        l_max_y= 0;
    }

    m_vertical.set_range(l_max_y, l_min_y);

    return;
}
