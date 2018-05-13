/*
 SLIDER.HPP

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
#ifndef __slider_hpp__

#include "valuator.hpp"

class slider : public widget, public valuator
{
public:

    enum orientation
    {
        HORIZONTAL,
        VERTICAL 
    };

    slider(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~slider();

    virtual void
    draw() const;

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);
    
    virtual enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    void
    set_orientation(
        enum orientation const          i_orientation);

protected:

    enum orientation                    m_orientation;

    double
    value2pct(
        double const                    i_value) const;

    int
    value2screen(
        double const                    i_value,
        int const                       i_len) const;

    double
    screen2value(
        int const                       i_pos,
        int const                       i_len) const;

    // _mjo this should be removed and use get_bbox instead
    void
    draw(
        struct box const&               i_box) const;

    // _mjo this should be removed and replaced with get_bbox
    enum event_response
    event_mouse(
        struct event_mouse const&       i_event,
        struct box const&               i_box);

private:

    slider();
    
    slider(
    const slider&);

    slider&
    operator=(slider const&);

    enum event_response
    event_key_horizontal(
        struct event_key const&         i_event);

    enum event_response
    event_key_vertical(
        struct event_key const&         i_event);

};

inline void
slider::set_orientation(
    enum orientation const              i_orientation)
{
    m_orientation= i_orientation;
    return;
}

#define __slider_hpp__
#endif
