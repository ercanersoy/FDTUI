/*
 COMBOBOX.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __combobox_hpp__

#include "entry.hpp"
#include "listbox.hpp"

class combobox : public entry
{

public:

    combobox(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~combobox();

    int
    add(
        unsigned char const*            i_label);

    virtual void
    draw() const;

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);

    virtual enum event_response
    event_mouse(
        struct event_mouse const&       i_event);

    void
    remove_all();

protected:

    listbox                             m_listbox;

    virtual void
    get_bbox(
        struct box&                     o_bbox) const;

    void
    popup();

private:

    combobox();
    
    combobox(
    const combobox&);

    combobox&
    operator=(combobox const&);

    void
    listbox_configure();

};

inline void
combobox::get_bbox(
    struct box&                         o_bbox) const
{

    o_bbox= m_box;
    o_bbox.m_len_x--;

    return;
}

inline int
combobox::add(
    unsigned char const*                i_label)
{
    int                                 l_exit;

    l_exit= m_listbox.add(i_label);

    return l_exit;
}

inline void
combobox::remove_all()
{

    m_listbox.remove_all();
    set_text(0);

    return;
}

#define __combobox_hpp__
#endif
