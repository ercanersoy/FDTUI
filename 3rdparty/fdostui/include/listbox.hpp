/*
 LISTBOX.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __listbox_hpp__

#include "scrllbar.hpp"
#include "array.h"
#include <string.h>
#include <stdlib.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

class listbox : public widget
{

public:

    typedef void (*signal_selected_t)(
        listbox const*                  i_listbox,
        void*                           io_user_data);

    listbox(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~listbox();

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

    unsigned char const*
    get_item(
        size_t const                    i_slot) const;

    size_t
    get_count() const;

    int
    get_selected_first() const;

    int
    get_selected_next(
        int const                       i_iter) const;

    virtual void
    move(
        int const                       i_pos_x,
        int const                       i_pos_y);

    virtual void
    resize(
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);

    void
    remove(
        size_t const                    i_slot);

    void
    remove_all();

    void
    set_label(
        unsigned char const*            i_str);

    void
    set_multiline(
        bool const                      i_multiline);

    virtual void
    set_parent(
        group*const                     i_parent);

    void
    set_response(
        int const                       i_response);

    void
    set_signal_selected(
        signal_selected_t               i_signal);

    void
    set_selected(
        size_t const                    i_slot,
        bool const                      i_selected=true);

    void
    unselect_all();

protected:

    bool                                m_multiline;
    size_t                              m_line;
    struct array                        m_array;
    scrollbar                           m_vertical;
    unsigned char*                      m_label;
    enum event_response                 m_response;
    signal_selected_t                   m_signal_selected;

private:

    listbox();
    
    listbox(
    const listbox&);

    listbox&
    operator=(listbox const&);

    void
    select(
        size_t const                    i_slot);

    static void
    vertical_cb(
        widget const*                   i_widget,
        void*                           io_user_data);

    void
    vertical_configure();

    void
    emit_selected();

public:

    void
    scroll_internal();

};

inline size_t
listbox::get_count() const
{
    return m_array.m_taken;
}

inline void
listbox::set_multiline(
    bool const                          i_multiline)
{

    m_multiline= i_multiline;

    return;
}

inline void
listbox::set_label(
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

inline void
listbox::set_parent(
    group*const                         i_parent)
{

    widget::set_parent(i_parent);
    m_vertical.set_parent(i_parent);

    return;
}

inline void
listbox::set_response(
    int const                           i_response)
{
    m_response= static_cast<enum event_response>(i_response);
    return;
}

inline void
listbox::set_signal_selected(
    signal_selected_t                   i_signal)
{

    m_signal_selected= i_signal;

    return;
}

inline void
listbox::emit_selected()
{

    if (m_signal_selected)
    {
        (*m_signal_selected)(this, m_user_data);
    }

    return;
}

#define __listbox_hpp__
#endif
