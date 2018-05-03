/* SCRLLBAR.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "scrllbar.hpp"
#include "draw.h"

scrollbar::scrollbar(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    slider(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_pagesize(1),
    m_dragging(false)
{

    set_step(1);

    return;
}
    
scrollbar::~scrollbar()
{
    return;
}

void
scrollbar::draw() const
{
    struct box                          l_box;
    int                                 l_pos_x;
    int                                 l_pos_y;

    l_box= m_box;

    if (HORIZONTAL == m_orientation)
    {
        l_box.m_pos_x++;
        l_box.m_len_x--;
        l_box.m_len_x--;
    }
    else
    {
        l_box.m_pos_y++;
        l_box.m_len_y--;
        l_box.m_len_y--;
    }

    slider::draw(l_box);

    if (HORIZONTAL == m_orientation)
    {
        l_pos_y= m_box.m_pos_y + (m_box.m_len_y / 2);

        draw_putc(
            m_box.m_pos_x,
            l_pos_y,
            _skin_buttons.m_left,
            1,
            _skin_slider.m_fcolor,
            _skin_slider.m_bcolor);

        draw_putc(
            m_box.m_pos_x+m_box.m_len_x-1,
            l_pos_y,
            _skin_buttons.m_right,
            1,
            _skin_slider.m_fcolor,
            _skin_slider.m_bcolor);
    }
    else
    {
        l_pos_x= m_box.m_pos_x + (m_box.m_len_x / 2);

        draw_putc(
            l_pos_x,
            m_box.m_pos_y,
            _skin_buttons.m_up,
            1,
            _skin_slider.m_fcolor,
            _skin_slider.m_bcolor);

        draw_putc(
            l_pos_x,
            m_box.m_pos_y+m_box.m_len_y-1,
            _skin_buttons.m_down,
            1,
            _skin_slider.m_fcolor,
            _skin_slider.m_bcolor);
    }

    return;
}

enum scrollbar::where
scrollbar::event_where_horizontal(
    struct event_mouse const&           i_event) const
{
    int                                 l_pos_thumb;
    int                                 l_pos_y;
    enum where                          l_where;

    l_where= WHERE_OUTOFBOUNDS;

    do
    {

        l_pos_y= m_box.m_pos_y + (m_box.m_len_y / 2);

        if (static_cast<int>(i_event.m_pos_y) != static_cast<int>(l_pos_y))
        {
            break;
        }

        if (static_cast<int>(i_event.m_pos_x) == static_cast<int>(m_box.m_pos_x))
        {
            l_where= WHERE_UPARROW;
            break;
        }

        if (i_event.m_pos_x == m_box.m_pos_x + m_box.m_len_x - 1)
        {
            l_where= WHERE_DOWNARROW;
            break;
        }

        l_pos_thumb= value2screen(m_value, m_box.m_len_x - 2);
        l_pos_thumb+= m_box.m_pos_x + 1;

        if (static_cast<int>(i_event.m_pos_x) == static_cast<int>(l_pos_thumb))
        {
            l_where= WHERE_THUMB;
            break;
        }

        if (static_cast<int>(i_event.m_pos_x) < static_cast<int>(l_pos_thumb))
        {
            l_where= WHERE_LEFTOFTHUMB;
            break;
        }

        l_where= WHERE_RIGHTOFTHUMB;

    }while(0);

    return l_where;
}

enum scrollbar::where
scrollbar::event_where_vertical(
    struct event_mouse const&           i_event) const
{
    int                                 l_pos_thumb;
    int                                 l_pos_x;
    enum where                          l_where;

    l_where= WHERE_OUTOFBOUNDS;

    do
    {

        l_pos_x= m_box.m_pos_x + (m_box.m_len_x / 2);

        if (static_cast<int>(i_event.m_pos_x) != static_cast<int>(l_pos_x))
        {
            break;
        }

        if (static_cast<int>(i_event.m_pos_y) == static_cast<int>(m_box.m_pos_y))
        {
            l_where= WHERE_UPARROW;
            break;
        }

        if (i_event.m_pos_y == m_box.m_pos_y + m_box.m_len_y - 1)
        {
            l_where= WHERE_DOWNARROW;
            break;
        }

        l_pos_thumb= value2screen(m_value, m_box.m_len_y - 2);
        l_pos_thumb+= m_box.m_pos_y + 1;

        if (static_cast<int>(i_event.m_pos_y) == static_cast<int>(l_pos_thumb))
        {
            l_where= WHERE_THUMB;
            break;
        }

        if (static_cast<int>(i_event.m_pos_y) < static_cast<int>(l_pos_thumb))
        {
            l_where= WHERE_LEFTOFTHUMB;
            break;
        }

        l_where= WHERE_RIGHTOFTHUMB;

    }while(0);

    return l_where;
}

enum scrollbar::where
scrollbar::event_where(
    struct event_mouse const&           i_event) const
{
    enum where                          l_where;

    if (HORIZONTAL == m_orientation)
    {
        l_where= event_where_horizontal(i_event);
    }
    else
    {
        l_where= event_where_vertical(i_event);
    }

    return l_where;
}

enum event_response
scrollbar::event_mouse(
    struct event_mouse const&           i_event)
{
    int                                 l_amount;
    struct box                          l_box;
    bool                                l_flip;
    enum event_response                 l_response;
    enum where                          l_where;

    l_response= RESPONSE_NONE;

    do
    {

        if (MOUSE_BUTTON_RELEASED == i_event.m_state)
        {
            m_dragging= false;
            break;
        }

        if (MOUSE_BUTTON_DRAGGING == i_event.m_state)
        {
            if (false == m_dragging)
            {
                break;
            }

            l_box= m_box;

            if (HORIZONTAL == m_orientation)
            {
               l_box.m_pos_x++;
               l_box.m_len_x--;
               l_box.m_len_x--;
            }
            else
            {
               l_box.m_pos_y++;
               l_box.m_len_y--;
               l_box.m_len_y--;
            }

            l_response= slider::event_mouse(i_event, l_box);
            break;
        }

        if (MOUSE_BUTTON_CLICKED != i_event.m_state)
        {
            break;
        }

        l_where= event_where(i_event);

        if (WHERE_OUTOFBOUNDS == l_where)
        {
            break;
        }

        if (WHERE_THUMB == l_where)
        {
            m_dragging= true;
            break;
        }

        l_flip= (m_maximum < m_minimum);

        switch(l_where)
        {
            case WHERE_UPARROW:
                l_amount= (!l_flip) ? -1 : 1;
                break;
            case WHERE_DOWNARROW:
                l_amount= (!l_flip) ? 1 : -1;
                break;
            case WHERE_LEFTOFTHUMB:
                l_amount= (!l_flip) ? -m_pagesize : m_pagesize;
                break;
            case WHERE_RIGHTOFTHUMB:
                l_amount= (!l_flip) ? m_pagesize : -m_pagesize;
                break;
        }

        handle_push();
        handle_drag(this, clamp(increment(m_value, l_amount)));
        handle_release(this);
        l_response= RESPONSE_HANDLED;

    }while(0);

    return l_response;
}

enum event_response
scrollbar::event_key(
    struct event_key const&             i_event)
{
    int                                 l_amount;
    bool                                l_flip;
    enum event_response                 l_response;

    do
    {

        l_flip= (m_maximum < m_minimum);

        if (SCAN_PGUP == i_event.m_scan)
        {
            l_amount= (!l_flip) ? -m_pagesize : m_pagesize;
            handle_push();
            handle_drag(this, clamp(increment(m_value, l_amount)));
            handle_release(this);
            l_response= RESPONSE_HANDLED;
            break;
        }

        if (SCAN_PGDN == i_event.m_scan)
        {
            l_amount= (!l_flip) ? m_pagesize : -m_pagesize;
            handle_push();
            handle_drag(this, clamp(increment(m_value, l_amount)));
            handle_release(this);
            l_response= RESPONSE_HANDLED;
            break;
        }

        if (SCAN_HOME == i_event.m_scan)
        {
            handle_push();
            handle_drag(this, (!l_flip) ? m_minimum : m_maximum);
            handle_release(this);
            l_response= RESPONSE_HANDLED;
            break;
        }

        if (SCAN_END == i_event.m_scan)
        {
            handle_push();
            handle_drag(this, (!l_flip) ? m_maximum : m_minimum);
            handle_release(this);
            l_response= RESPONSE_HANDLED;
            break;
        }

        l_response= slider::event_key(i_event);

    }while(0);

    return l_response;
}
