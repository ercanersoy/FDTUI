/*
 LABEL.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __label_hpp__

#include "draw.h"
#include "widget.hpp"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

class label : public widget
{
public:

    label(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const        i_len_y);
    
    virtual
    ~label();

    virtual void
    draw() const;

    unsigned char const*
    get_text() const;

    void
    set_text(
        unsigned char const*            i_str);

protected:

    unsigned char*                      m_label;

private:

    label();
    
    label(
    const label&);

    label&
    operator=(label const&);

};

inline unsigned char const*
label::get_text() const
{
    return m_label;
}

inline void
label::set_text(
    unsigned char const*                i_str)
{

    free(m_label);
    m_label= 0;

    if (i_str && i_str[0])
    {
        size_t                          l_len;
        l_len= 1+strlen(reinterpret_cast<char const*>(i_str));
        m_label= reinterpret_cast<unsigned char*>(malloc(l_len));
        if (m_label)
        {
            memcpy(m_label, i_str, l_len);
        }
    }

    return;
}

#define __label_hpp__
#endif
