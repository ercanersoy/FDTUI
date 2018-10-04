/*
 SCRLLBAR.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __scrollbar_hpp__

#include "slider.hpp"

class scrollbar : public slider
{

public:

    scrollbar(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~scrollbar();

    virtual void
    draw() const;

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);
    
    virtual enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    int
    get_pagesize() const;

    void
    set_pagesize(
        int const                       i_pagesize);

protected:

    int                                 m_pagesize;
    bool                                m_dragging;

private:

    scrollbar();
    
    scrollbar(
    const scrollbar&);

    scrollbar&
    operator=(scrollbar const&);

    enum where
    {
        WHERE_OUTOFBOUNDS               = 0,
        WHERE_THUMB,
        WHERE_UPARROW,
        WHERE_DOWNARROW,
        WHERE_LEFTOFTHUMB,
        WHERE_RIGHTOFTHUMB
    };

    enum where
    event_where_horizontal(
        struct event_mouse const&       i_event) const;

    enum where
    event_where_vertical(
        struct event_mouse const&       i_event) const;

    enum where
    event_where(
        struct event_mouse const&       i_event) const;

};

inline int
scrollbar::get_pagesize() const
{
    return m_pagesize;
}

inline void
scrollbar::set_pagesize(
    int const                           i_pagesize)
{
    m_pagesize= i_pagesize;
    return;
}

#define __scrollbar_hpp__
#endif
