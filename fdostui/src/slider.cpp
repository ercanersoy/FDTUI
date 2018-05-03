/*
 SLIDER.CPP

 Derived from FLTK Slider class

 Copyright 1998-2006 by Bill Spitzak and others.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 USA.

*/
#include "slider.hpp"
#include "draw.h"
#include "window.hpp"
#include <math.h>


#include <stdio.h>
#include <string.h>

#define rint(v) floor((v)+.5)

slider::slider(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    valuator(),
    m_orientation(HORIZONTAL)
{
    return;
}
    
slider::~slider()
{
}

double
slider::value2pct(
    double const                        i_value) const
{
    bool                                l_flip;
    double                              l_min;
    double                              l_max;
    double                              l_pct;
    double                              l_tmp;
    double                              l_value;

    do
    {

        if (m_minimum == m_maximum)
        {
            l_pct = 0.;
            break;
        }

        l_flip = (m_maximum < m_minimum);

        if (!l_flip)
        {
            l_min= m_minimum;
            l_max= m_maximum;
        }
        else
        {
            l_min= m_maximum;
            l_max= m_minimum;
        }

        if (VERTICAL == m_orientation)
        {
            l_flip = !l_flip;
        }

        if (l_max > 0)
        {
            l_value= i_value;
        }
        else
        {
            l_flip= !l_flip;
            l_tmp= l_min;
            l_min= -l_max;
            l_max= -l_tmp;
            l_value= -i_value;
        }

        l_pct= (l_value - l_min) / (l_max - l_min);

        if (l_flip)
        {
            l_pct= 1-l_pct;
        }

        if (l_pct > 1)
        {
            l_pct= 1;
        }
        else if (l_pct < 0)
        {
            l_pct= 0;
        }

    }while(0);

    return l_pct;
}

int
slider::value2screen(
    double const                        i_value,
    int const                           i_len) const
{
    double                              l_pct;
    int                                 l_pos;
    double                              l_value;

    l_pct= value2pct(i_value);
    l_value= l_pct * i_len;
    l_value= round(l_value);
    l_pos= static_cast<int>(l_value);

    if (i_len <= l_pos)
    {
        l_pos--;
    }

    return l_pos;
}

double
slider::screen2value(
    int const                           i_pos,
    int const                           i_len) const
{
    bool                                l_flip;
    double                              l_max;
    double                              l_min;
    double                              l_value;
    double                              l_pct;
    int                                 l_pos;
    double                              l_tmp;

    do
    {

        if (i_len <= 0)
        {
            l_value= m_minimum;
            break;
        }

        if (i_pos == i_len - 1)
        {
            if (slider::HORIZONTAL == m_orientation)
            {
                l_value= m_maximum;
            }
            else
            {
                l_value= m_minimum;
            }
            break;
        }

        l_pos= i_pos;

        l_flip= (m_maximum < m_minimum);

        if (!l_flip)
        {
            l_min= m_minimum;
            l_max= m_maximum;
        }
        else
        {
            l_min= m_maximum;
            l_max= m_minimum;
        }

        if (slider::VERTICAL == m_orientation)
        {
            l_flip= !l_flip;
        }

        if (l_flip)
        {
            l_pos= i_len-l_pos;
        }

        l_pct= static_cast<double>(l_pos)/i_len;

        if (l_pct <= 0)
        {
            l_value= l_min;
            break;
        }

        if (l_pct >= 1)
        {
            l_value= l_max;
            break;
        }

        l_flip= (l_max <= 0);

        if (l_flip)
        {
            l_tmp= l_min;
            l_min= -l_max;
            l_max= -l_tmp;
            l_pct= 1 - l_pct;
        }

        l_value= l_pct * (l_max - l_min) + l_min;
/*
_mjo
        double l_derivative= (l_max - l_min) / i_len;

        l_step= get_step();

        if (l_step && l_derivative > l_step) {
            double w = log10(l_derivative);
            double l = ceil(w);
            int num = 1;
            int i; for (i = 0; i < l; i++) num *= 10;
            int denom = 1;
            for (i = -1; i >= l; i--) denom *= 10;
            if (l-w > 0.69897) denom *= 5;
            else if (l-w > 0.30103) denom *= 2;
            l_value = floor(l_value*denom/num+.5)*num/denom;
        }
*/

        if (l_flip)
        {
            l_value= -l_value;
        }

    }while(0);

    return l_value;
}

void
slider::draw(
    struct box const&                   i_box) const
{
    int                                 l_value;
    int                                 l_pos_x;
    int                                 l_pos_y;

    draw_fill_area(
        i_box.m_pos_x,
        i_box.m_pos_y,
        i_box.m_len_x,
        i_box.m_len_y,
        _skin_slider.m_fillc,
        _skin_slider.m_fcolor,
        _skin_slider.m_bcolor);

    if (HORIZONTAL == m_orientation)
    {
        l_value= value2screen(m_value, i_box.m_len_x);
        l_pos_x= l_value + i_box.m_pos_x;
        l_pos_y= i_box.m_pos_y + (i_box.m_len_y / 2);
    }
    else
    {
        l_value= value2screen(m_value, i_box.m_len_y);
        l_pos_y= l_value + i_box.m_pos_y;
        l_pos_x= i_box.m_pos_x + (i_box.m_len_x / 2);
    }

    draw_putc(
        l_pos_x,
        l_pos_y,
        _skin_buttons.m_thumb,
        1,
        _skin_slider.m_fcolor,
        _skin_slider.m_bcolor);

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        int                             l_cur_x;
        int                             l_cur_y;
        translate_screen(&l_cur_x, &l_cur_y);
        ::move(l_cur_y, l_cur_x+l_pos_x-1);
    }
#endif

    return;
}

void
slider::draw() const
{
    draw(m_box);
}

enum event_response
slider::event_key_horizontal(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    do
    {

        if (SCAN_LEFT == i_event.m_scan)
        {
          handle_push();
          handle_drag(this, clamp(increment(m_value, -1)));
          handle_release(this);
          l_response= RESPONSE_HANDLED;
          break;
        }

        if (SCAN_RIGHT == i_event.m_scan)
        {
          handle_push();
          handle_drag(this, clamp(increment(m_value, 1)));
          handle_release(this);
          l_response= RESPONSE_HANDLED;
        }

    }while(0);

    return l_response;
}

enum event_response
slider::event_key_vertical(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    do
    {

        if (SCAN_UP == i_event.m_scan)
        {
            handle_push();
            handle_drag(this, clamp(increment(m_value, 1)));
            handle_release(this);
            l_response= RESPONSE_HANDLED;
            break;
        }

        if (SCAN_DOWN == i_event.m_scan)
        {
            handle_push();
            handle_drag(this, clamp(increment(m_value, -1)));
            handle_release(this);
            l_response= RESPONSE_HANDLED;
            break;
        }

    }while(0);

    return l_response;
}

enum event_response
slider::event_key(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    switch(m_orientation)
    {
        case HORIZONTAL:
            l_response= event_key_horizontal(i_event);
            break;
        case VERTICAL:
            l_response= event_key_vertical(i_event);
            break;
        default:
            break;
    }

    return l_response;
}
    
enum event_response
slider::event_mouse(
    struct event_mouse const&           i_event,
    struct box const&                   i_box)
{
    int                                 l_len;
    int                                 l_pos;
    enum event_response                 l_response;
    double                              l_value;

    l_response= RESPONSE_NONE;

    if ((MOUSE_BUTTON_CLICKED == i_event.m_state) ||
        (MOUSE_BUTTON_DRAGGING == i_event.m_state))
    {

        if (HORIZONTAL == m_orientation)
        {
            l_pos= i_event.m_pos_x - i_box.m_pos_x;
            l_len= i_box.m_len_x;
        }
        else
        {
            l_pos= i_event.m_pos_y - i_box.m_pos_y;
            l_len= i_box.m_len_y;
        }

        l_value= screen2value(l_pos, l_len);
        handle_push();
        handle_drag(this, clamp(l_value));
        handle_release(this);
        l_response= RESPONSE_HANDLED;
    }

    return l_response;
}

enum event_response
slider::event_mouse(
    struct event_mouse const&           i_event)
{
    enum event_response                 l_response;

    l_response= event_mouse(i_event, m_box);

    return l_response;
}
