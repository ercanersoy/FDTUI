/*
 WINDOW.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __window_hpp__

#include "group.hpp"
#include <string.h>
#include <stdlib.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

class window : public group
{

public:

    enum attributes
    {
        EMPTY=                          (1 << 0),
        BORDER=                         (1 << 1),
        TITLE=                          (1 << 2),
        MINMAX=                         (1 << 3),
        RESIZE=                         (1 << 4),
        SYSTEM=                         (1 << 5)
    };
    
    window(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~window();
    
    virtual void
    draw() const;

    enum attributes
    get_attributes() const;
    
    unsigned char const*
    get_title() const;

    void
    set_attributes(
        unsigned const                  i_attributes);
    
    void
    set_title(
        unsigned char const*            i_str);

protected:

    unsigned char*                      m_title;
    enum attributes                     m_attributes;

private:

    window();

    window(
    const window&);

    window&
    operator=(window const&);

};

inline window::attributes
window::get_attributes() const
{
    return m_attributes;
}

inline unsigned char const*
window::get_title() const
{
    return m_title;
}

inline void
window::set_attributes(
    unsigned const                      i_attributes)
{
    m_attributes= static_cast<enum attributes>(i_attributes);
    return;
}

inline void
window::set_title(
    unsigned char const*                i_str)
{

    free(m_title);
    m_title= 0;

    if (i_str && i_str[0])
    {
        size_t                          l_len;
        l_len= 1+strlen(reinterpret_cast<char const*>(i_str));
        m_title= reinterpret_cast<unsigned char*>(malloc(l_len));
        if (m_title)
        {
            memcpy(m_title, i_str, l_len);
        }
    }

    return;
}

#define __window_hpp__
#endif
