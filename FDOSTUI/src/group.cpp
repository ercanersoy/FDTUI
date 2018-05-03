/*
 GROUP.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "group.hpp"
#include "skin.h"
#include "matrix.h"
#include "clip.h"
#include "draw.h"
#include "wm.hpp"

group::group(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_focused(0),
    m_slot(0),
    m_array()
{
#define GROWBY 30
    array_assign(&m_array, GROWBY);

    return;
}

group::~group()
{
    widget const*                       l_widget;

    do
    {
        l_widget= (widget*)array_pop(&m_array);

        if (0 == l_widget)
        {
            break;
        }

        delete l_widget;

    }while(1);

    array_discharge(&m_array);

    return;
}

void
group::set_damage(
    unsigned int const                  i_damage)
{
    size_t                              l_slot;
    widget *                            l_widget;

    for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
    {
        l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
        (*l_widget).set_damage(i_damage);
    }

    return;
}

int
group::add(
    widget*const                        i_widget)
{
    int                                 l_exit;
    bool                                l_found;
    size_t                              l_slot;
    widget const*                       l_widget;

    l_exit= -1;

    do
    {

        if (this == i_widget)
        {
            break;
        }

        l_found= false;

        for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
        {
            l_widget= reinterpret_cast<widget const*>(m_array.m_base[l_slot]);
            l_found= (l_widget == i_widget);
            if (l_found)
            {
                break;
            }
        }

        if (l_found)
        {
            l_exit= 0;
            break;
        }

        l_exit= array_push(&m_array, i_widget);

        if (0 == l_exit)
        {
            (*i_widget).set_parent(this);
        }

    }while(0);

    return l_exit;
}

void
group::draw() const
{
    struct box                          l_clip;
    struct box                          l_clip_cur;
    widget const*                       l_widget;
    size_t                              l_slot;

    screen_push(1);
    matrix_push();
    matrix_translate(m_box.m_pos_x+1, m_box.m_pos_y+1);

    l_clip.m_pos_x= 0;
    l_clip.m_pos_y= 0;
    l_clip.m_len_x= m_box.m_len_x - 2;
    l_clip.m_len_y= m_box.m_len_y - 2;
    matrix_transform(&l_clip.m_pos_x, &l_clip.m_pos_y);
    clip_get(&l_clip_cur);
    clip(&l_clip, &l_clip_cur);
    clip_push(&l_clip);

    for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
    {
        l_widget= reinterpret_cast<widget const*>(m_array.m_base[l_slot]);
        (*l_widget).draw();
    }

    clip_pop();
    matrix_pop();
    screen_pop();

    return;
}

void
group::focus_enter()
{
    size_t                              l_slot;
    widget*                             l_widget;

    widget::focus_enter();

    do
    {
        if (m_focused)
        {
            (*m_focused).focus_enter();
            break;
        }

        for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
        {
            l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
            if ((*l_widget).get_can_focus())
            {
                m_slot= l_slot;
                m_focused= l_widget;
                (*l_widget).focus_enter();
                break;
            }
        }

    }while(0);

    return;
}

void
group::focus_leave()
{

    widget::focus_leave();

    if (m_focused)
    {
        (*m_focused).focus_leave();
    }

    return;
}

void
group::focus_next()
{
    size_t                              l_slot;
    widget*                             l_widget;

    do
    {
        if (0 == m_array.m_taken)
        {
            m_focused= 0;
            m_slot= 0;
            break;
        }

        if (0 == m_focused)
        {
            m_focused= reinterpret_cast<widget *>(m_array.m_base[0]);
            m_slot= 0;
            (*m_focused).focus_enter();
            break;
        }

        l_slot= m_slot+1;

        do
        {

            if (l_slot == m_slot)
            {
                break;
            }

            if (m_array.m_taken <= l_slot)
            {
                l_slot= 0;
            }

            l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
            if ((*l_widget).get_can_focus())
            {
                (*m_focused).focus_leave();
                m_focused= l_widget;
                m_slot= l_slot;
                (*l_widget).focus_enter();
                break;
            }

            l_slot++;


        }while(1);

    }while(0);

    return;
}

void
group::focus_previous()
{
    size_t                              l_slot;
    widget*                             l_widget;

    do
    {
        if (0 == m_array.m_taken)
        {
            m_focused= 0;
            m_slot= 0;
            break;
        }

        if (0 == m_focused)
        {
            m_focused= reinterpret_cast<widget*>(m_array.m_base[m_array.m_taken-1]);
            m_slot= m_array.m_taken-1;
            (*m_focused).focus_enter();
            break;
        }

        if (m_slot)
        {
            l_slot= m_slot - 1;
        }
        else
        {
            l_slot= m_array.m_taken - 1;
        }

        do
        {

            if (l_slot == m_slot)
            {
                break;
            }

            l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
            if ((*l_widget).get_can_focus())
            {
                (*m_focused).focus_leave();
                m_focused= l_widget;
                m_slot= l_slot;
                (*l_widget).focus_enter();
                break;
            }

            if (l_slot)
            {
                l_slot--;
            }
            else
            {
                l_slot= m_array.m_taken - 1;
            }

        }while(1);

    }while(0);

    return;
}

enum event_response
group::event_key_default(
   enum scancodes const                 i_scan)
{
    enum event_response                 l_response;
    size_t                              l_slot;
    widget*                             l_widget;

    l_response= RESPONSE_NONE;

    for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
    {
        l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
        if ((*l_widget).get_can_focus())
        {
            l_response= (*l_widget).event_key_default(i_scan);
            if (RESPONSE_NONE != l_response)
            {
                break;
            }
        }
    }

    return l_response;
}

enum event_response
group::event_key(
   struct event_key const&              i_event)
{
    enum event_response                 l_response;
    enum event_response                 l_response2;

    l_response= RESPONSE_NONE;

    do
    {

        if (ASCII_TAB == i_event.m_ascii)
        {
            focus_next();
            l_response= RESPONSE_HANDLED;
        }
        else if (SCAN_SHIFT_TAB == i_event.m_scan)
        {
            focus_previous();
            l_response= RESPONSE_HANDLED;
        }

        if (m_focused)
        {
            l_response2= (*m_focused).event_key(i_event);
            if (RESPONSE_NONE == l_response)
            {
                l_response= l_response2;
            }
        }

    }while(0);

    return l_response;
}
    
enum event_response
group::event_mouse(
    struct event_mouse const&           i_event)
{
    enum event_response                 l_response;
    struct event_mouse                  l_mouse;
    size_t                              l_slot;
    widget*                             l_widget;
    int                                 l_hit;

    l_response= RESPONSE_NONE;
    l_mouse= i_event;
    l_mouse.m_pos_x= i_event.m_pos_x - m_box.m_pos_x - 1/*frame*/;
    l_mouse.m_pos_y= i_event.m_pos_y - m_box.m_pos_y - 1/*frame*/;

    do
    {

        if (0 > static_cast<int>(l_mouse.m_pos_x) || 
            0 > static_cast<int>(l_mouse.m_pos_y))
        {
            // frame
            break;
        }

        if (0 == m_array.m_taken)
        {
            break;
        }

        if (m_focused)
        {
            l_hit= (*m_focused).contains(l_mouse.m_pos_x, l_mouse.m_pos_y);

            if (l_hit)
            {
                l_response= (*m_focused).event_mouse(l_mouse);
                break;
            }
        }

        if (MOUSE_BUTTON_CLICKED != l_mouse.m_state)
        {
            break;
        }

        for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
        {
            l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
            if ((*l_widget).get_can_focus())
            {
                l_hit= (*l_widget).contains(l_mouse.m_pos_x, l_mouse.m_pos_y);
                if (l_hit)
                {
                    if (m_focused)
                    {
                        (*m_focused).focus_leave();
                    }
                    m_focused= l_widget;
                    m_slot= l_slot;
                    (*l_widget).focus_enter();
                    l_response= (*l_widget).event_mouse(l_mouse);
                    break;
                }
            }
        }

    }while(0);

    return l_response;
}

void
group::set_focus(
    widget&                             i_widget)
{
    size_t                              l_slot;
    widget*                             l_widget;

    for (l_slot= 0; m_array.m_taken > l_slot; l_slot++)
    {
        l_widget= reinterpret_cast<widget *>(m_array.m_base[l_slot]);
        if (&i_widget == l_widget)
        {
            if (l_widget == m_focused)
            {
                break;
            }
            if ((*l_widget).get_can_focus())
            {
                if (m_focused)
                {
                    (*m_focused).focus_leave();
                }
                m_focused= l_widget;
                m_slot= l_slot;
                (*l_widget).focus_enter();
            }
            break;
        }
    }

    return;
}
