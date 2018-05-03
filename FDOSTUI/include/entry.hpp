/*
 ENTRY.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __entry_hpp__

#include "widget.hpp"

class entry : public widget
{

public:

    typedef void (*signal_changed_t)(
        entry const*                    i_entry,
        void*                           io_user_data);

    enum purpose
    {
        PURPOSE_ANY,
        PURPOSE_ALPHA,
        PURPOSE_DIGIT,
        PURPOSE_INTEGER,
        PURPOSE_FLOAT
    };

    entry(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~entry();

    virtual void
    draw() const;
    
    virtual enum event_response
    event_key(
        struct event_key const&         i_event);

    virtual void
    focus_enter();

    virtual void
    focus_leave();

    unsigned char const*
    get_text() const;

    size_t
    get_text_length() const;

    enum purpose 
    get_purpose() const;

    void
    set_editable(
        bool const                      i_editable=true);

    void
    set_purpose(
        enum purpose const              i_purpose);

    void
    set_text(
        unsigned char const*            i_str);

    // _mjo: fixme a signal is not emitted 
    void
    set_signal_changed(
        signal_changed_t                i_signal);

protected:

    enum purpose                        m_purpose;
    size_t                              m_caret;
    size_t                              m_offset;
    size_t                              m_slot;
    size_t                              m_length;
    size_t                              m_block_size;
    unsigned char*                      m_block;
    bool                                m_editable;
    signal_changed_t                    m_signal_changed;

    void
    show_cursor();

    void
    grow(
        size_t const                    i_needed);

    virtual void
    get_bbox(
        struct box&                     o_bbox) const;

    int
    pos_inc();

    int
    pos_dec();

    void
    del();

    void
    ins(
        unsigned char const             i_char);

    void
    ovw(
        unsigned char const             i_char);

    bool
    validate(
        unsigned char const             i_char);

    void
    caret_set_position() const;

    void
    emit_changed();

private:

    entry();

    entry(
        const entry&);

    entry&
    operator=(entry const&);

};

inline unsigned char const*
entry::get_text() const
{
    m_block[m_length]= 0;
    return m_block;
}

inline size_t
entry::get_text_length() const
{
    return m_length;
}

inline enum entry::purpose
entry::get_purpose() const
{
    return m_purpose;
}

inline void
entry::set_purpose(
    enum purpose const                  i_purpose)
{
    m_purpose= i_purpose;
    return;
}

inline void
entry::get_bbox(
    struct box&                         o_bbox) const
{
    o_bbox= m_box;
}

inline void
entry::set_editable(
    bool const                          i_editable)
{
    m_editable= i_editable;
    return;
}

inline void
entry::set_signal_changed(
    signal_changed_t                    i_signal)
{
    m_signal_changed= i_signal;
    return;
}

inline void
entry::emit_changed()
{

    if (m_signal_changed)
    {
        (*m_signal_changed)(this, m_user_data);
    }

    return;
}

inline void
entry::show_cursor()
{
    // fixme: if widget is disabled
    if (m_editable)
    {
        if (CURSOR_OVERWRITE == _cursor_mode)
        {
            cursor_block();
        }
        else
        {
            cursor_underline();
        }
    }
    else
    {
        cursor_hide();
    }

    return;
}

#define __entry_hpp__
#endif
