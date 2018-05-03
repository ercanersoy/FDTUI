/*
 VALUATOR.CPP

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
#include "valuator.hpp"
#include "wm.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf(buf, len, format, ...) \
    _snprintf_s(buf, len, len, format, __VA_ARGS__)
#endif

#if defined(__DMC__) 
#define snprintf _snprintf
#endif

#define rint(v) floor((v)+.5)

valuator::valuator() :
    m_debounce(false),
    m_value(0.),
    m_previous_value(1.),
    m_minimum(0.),
    m_maximum(1.),
    m_step(0.),
    m_precision(1),
    m_signal_value(0)
{
    return;
}

valuator::~valuator()
{
    return;
}

void
valuator::set_precision(
    int const                           i_digits)
{
    unsigned int                        l_digits;

    if (i_digits > 9)
    {
        l_digits= 9;
    }
    else if (i_digits < 0)
    {
        l_digits= 0;
    }
    else
    {
        l_digits= static_cast<unsigned int>(i_digits);
    }

    m_step= 1.;
    m_precision= 1;

    do
    {

        if (0 == l_digits)
        {
            break;
        }

        m_precision *= 10;
        l_digits--;

    }while(1);

    return;
}

double
valuator::round(
    double const                        i_value) const
{
    double                              l_value;

    do
    {
        if (!m_step)
        {
            l_value= i_value;
            break;
        }

        l_value= rint(i_value * m_precision / m_step) * m_step / m_precision;

    }while(0);

    return l_value;
}

double
valuator::clamp(
    double const                        i_value) const
{
    double                              l_value;

    do
    {

        if ((i_value < m_minimum) == ( m_minimum <= m_maximum))
        {
            l_value= m_minimum;
            break;
        }

        if ((i_value > m_maximum) == (m_minimum <= m_maximum))
        {
            l_value= m_maximum;
            break;
        }

        l_value= i_value;

    }while(0);

    return l_value;
}

double
valuator::clamp_soft(
    double const                        i_value) const
{
    int                                 l_which;
    double                              l_value;

    l_which = (m_minimum<=m_maximum);

    do
    {

        if (
            (i_value < m_minimum) == l_which &&
            m_previous_value != m_minimum &&
            (m_previous_value < m_minimum) != l_which
           )
        {
            l_value= m_minimum;
            break;
        }


        if (
            (i_value > m_maximum) == l_which &&
            m_previous_value !=m_maximum &&
            (m_previous_value > m_maximum) != l_which
           )
        {
            l_value= m_maximum;
            break;
        }

        l_value= i_value;

    }while(0);

    return l_value;
}

double
valuator::increment(
    double const                        i_value,
    int const                           i_amount) const
{
    double                              l_value;
    int                                 l_amount;

    l_amount= i_amount;

    do
    {
        if (!m_step)
        {
            l_value= i_value + l_amount * (m_maximum-m_minimum) / 100.;
            break;
        }

        if (m_minimum > m_maximum)
        {
            l_amount= -i_amount;
        }

        l_value= (rint(i_value * m_precision / m_step) + l_amount) *
            m_step / m_precision;

    }while(0);

    return l_value;
}

bool
valuator::set_value(
  double const                          i_value)
{
    bool                                l_changed;

    l_changed= false;

    if (m_value != i_value)
    {
        m_value= i_value;
        l_changed= true;
    }

    return l_changed;
}

void
valuator::handle_drag(
    widget *const                       io_widget,
    double const                        i_value)
{
    bool                                l_changed;

    l_changed= set_value(i_value);

    if (l_changed && false == m_debounce)
    {
        (*io_widget).set_damage(widget::DAMAGE_VALUE);
        wm_draw_widget(io_widget);
    }

    return;
}

void
valuator::handle_release(
    widget *const                       io_widget)
{
    if (m_previous_value != m_value)
    {
        if (m_signal_value)
        {
            (*m_signal_value)(io_widget, (*io_widget).get_user_data());
        }
    }
    return;
}

int
valuator::format(
    char*                               buffer,
    unsigned int const                  i_size)
{
  double v = m_value;

#if defined(__BORLANDC__) 
    (void)i_size;
#endif

  if (!m_step) 
#if !defined(__BORLANDC__) 
    return snprintf(buffer, i_size, "%g", v);
#else
    return sprintf(buffer, "%g", v);
#endif

  double s = m_step-floor(m_step);

  if (!s)
#if !defined(__BORLANDC__) 
    return snprintf(buffer, i_size, "%ld", static_cast<long>(v));
#else
    return sprintf(buffer, "%ld", static_cast<long>(v));
#endif

  int i=1;
  for (; i < 8; i++) {
    if (s >= .099) break;
    s *= 10;
  }

#if !defined(__BORLANDC__) 
    return snprintf(buffer, i_size, "%.*f", i, v);
#else
    return sprintf(buffer, "%.*f", i, v);
#endif
}
