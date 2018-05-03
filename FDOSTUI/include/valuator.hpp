/*
 VALUATOR.HPP

 Derived from FLTK Valuator class

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
#ifndef __valuator_hpp__

#include "widget.hpp"

class valuator
{

public:

    typedef void (*signal_value_t)(
        widget const*                   i_widget,
        void*                           io_user_data);

    valuator();
    
    virtual
    ~valuator();

    virtual double
    get_value() const;

    double
    get_minimum() const;

    double
    get_maximum() const;

    double
    get_step() const;

    void
    set_range(
        double const                    i_minimum,
        double const                    i_maximum);

    void
    set_step(
        double const                    i_step);

    void
    set_step(
        double const                    i_step,
        int const                       i_precision);

    virtual bool
    set_value(
        double const                    i_value);

    void
    set_precision(
        int const                       i_digits);

    virtual int
    format(
        char*                           o_buffer,
        unsigned int const              i_size);

    void
    set_signal_value(
        signal_value_t                  i_signal);

    double
    round(
        double const                    i_value) const;

    double
    clamp(
        double const                    i_value) const;

    double
    clamp_soft(
        double const                    i_value) const;

    double
    increment(
        double const                    i_value,
        int const                       i_amount) const;

    void
    set_debounce(
        bool const                      i_debounce);

protected:

    bool                                m_debounce;
    double                              m_value;
    double                              m_previous_value;
    double                              m_minimum;
    double                              m_maximum;
    double                              m_step;
    double                              m_precision;
    signal_value_t                      m_signal_value;

    void
    handle_push();

    void
    handle_drag(
        widget *const                   io_widget,
        double const                    i_value);

    void
    handle_release(
        widget *const                   io_widget);

private:
    
    valuator(
    const valuator&);

    valuator&
    operator=(valuator const&);

};

inline void
valuator::set_signal_value(
    signal_value_t                      i_signal)
{

    m_signal_value= i_signal;

    return;
}

inline double
valuator::get_value() const
{
    return m_value;
}

inline double
valuator::get_minimum() const
{
    return m_minimum;
}

inline double
valuator::get_maximum() const
{
    return m_maximum;
}

inline double
valuator::get_step() const
{
    double                              l_step;

    l_step= (m_step / m_precision);

    return l_step;
}

inline void
valuator::set_range(
    double const                        i_minimum,
    double const                        i_maximum)
{
    m_minimum= i_minimum;
    m_maximum= i_maximum;
    // _mjo fixme if value out of new range
    return;
}

inline void
valuator::set_step(
    double const                        i_step)
{
    m_step= i_step;
    return;
}

inline void
valuator::set_step(
    double const                        i_step,
    int const                           i_precision)
{
    m_step= i_step;
    m_precision= i_precision;
    return;
}

inline void
valuator::handle_push()
{
    m_previous_value= m_value;
    return;
}

inline void
valuator::set_debounce(
    bool const                          i_debounce)
{
    m_debounce= i_debounce;
}

#define __valuator_hpp__
#endif
