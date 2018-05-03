/*
 SPINNER.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __spinner_hpp__

#include "valuator.hpp"
#include "entry.hpp"
#include <stdlib.h>

class spinner : public entry, public valuator
{

public:

    spinner(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~spinner();

    virtual void
    draw() const;

    virtual double
    get_value() const;

    virtual bool
    set_value(
        double const                    i_value);

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);
    
    virtual enum event_response
    event_mouse(
    struct event_mouse const&       i_event);

protected:

    virtual void
    get_bbox(
        struct box&                     o_bbox) const;

private:

    spinner();
    
    spinner(
    const spinner&);

    spinner&
    operator=(spinner const&);

};

inline void
spinner::get_bbox(
    struct box&                         o_bbox) const
{
    o_bbox= m_box;
    if (3 <= m_box.m_len_x)
    {
        o_bbox.m_len_x--;
        o_bbox.m_len_x--;
    }
}

inline double
spinner::get_value() const
{
    double                              l_value;

    m_block[m_length]= 0;
    l_value= strtod(reinterpret_cast<char*>(m_block), 0);

    return l_value;
}

#define __spinner_hpp__
#endif
