/*
 SPINNER.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "spinner.hpp"
#include "draw.h"
#include "skin.h"

spinner::spinner(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    entry(i_pos_x, i_pos_y, i_len_x, i_len_y),
    valuator()
{
    set_step(1.);
    set_range(0., 100.);
    set_purpose(PURPOSE_INTEGER);
    return;
}
    
spinner::~spinner()
{
    return;
}

void
spinner::draw() const
{
    int                                 l_pos_y;

    entry::draw();

    do
    {

        if (3 > m_box.m_len_x)
        {
            break;
        }

        l_pos_y= m_box.m_pos_y + (m_box.m_len_y / 2);

        draw_putc(
            m_box.m_pos_x+m_box.m_len_x-2,
            l_pos_y,
            _skin_buttons.m_up,
            1,
            _skin_slider.m_fcolor,
            _skin_slider.m_bcolor);

        draw_putc(
            m_box.m_pos_x+m_box.m_len_x-1,
            l_pos_y,
            _skin_buttons.m_down,
            1,
            _skin_slider.m_fcolor,
            _skin_slider.m_bcolor);

    }while(0);

    return;
}

enum event_response
spinner::event_key(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;
    double                              l_value;

    do
    {

        if (SCAN_UP == i_event.m_scan)
        {
            l_value= get_value();
            handle_push();
            handle_drag(this, clamp(increment(l_value, 1)));
            handle_release(this);
            l_response= RESPONSE_HANDLED;
            break;
        }

        if (SCAN_DOWN == i_event.m_scan)
        {
            l_value= get_value();
            handle_push();
            handle_drag(this, clamp(increment(l_value, -1)));
            handle_release(this);
            l_response= RESPONSE_HANDLED;
            break;
        }

        l_response= entry::event_key(i_event);

    }while(0);

    return l_response;
}
    
enum event_response
spinner::event_mouse(
    struct event_mouse const&           i_event)
{
    enum event_response                 l_response;
    double                              l_value;

    do
    {

        if (MOUSE_BUTTON_CLICKED == i_event.m_state)
        {
            if (i_event.m_pos_x == m_box.m_pos_x+m_box.m_len_x-2)
            {
                l_value= get_value();
                handle_push();
                handle_drag(this, clamp(increment(l_value, 1)));
                handle_release(this);
                l_response= RESPONSE_HANDLED;
                break;
            }

            if (i_event.m_pos_x == m_box.m_pos_x+m_box.m_len_x-1)
            {
                l_value= get_value();
                handle_push();
                handle_drag(this, clamp(increment(l_value, -1)));
                handle_release(this);
                l_response= RESPONSE_HANDLED;
                break;
            }
        }

        l_response= entry::event_mouse(i_event);

    }while(0);

    return l_response;
}

bool
spinner::set_value(
    double const                        i_value)
{
    bool                                l_changed;

    do
    {
        l_changed= valuator::set_value(i_value);

        if (!l_changed)
        {
            break;
        }

        grow(129);
        m_length= format(reinterpret_cast<char*>(m_block), m_block_size);
        m_caret= 0;
        m_offset= 0;
        m_slot= 0;
        caret_set_position();

    }while(0);

    return l_changed;
}
