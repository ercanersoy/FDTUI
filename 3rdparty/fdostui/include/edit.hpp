/*
 EDIT.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __edit_hpp__

#include "cursor.h"
#include "scrllbar.hpp"
#include "gapbuf.h"

class edit : public widget
{

public:

    edit(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~edit();

    virtual void
    draw() const;

    virtual enum event_response
    event_key(
        struct event_key const&         i_event);

    void
    focus_enter();

    void
    focus_leave();

    unsigned char const __FAR*
    get_text();

    size_t
    get_text_length() const;

protected:

    size_t                              m_caret_x;
    size_t                              m_caret_y;
    size_t                              m_sticky_caret_x;
    size_t                              m_cursor;
    size_t                              m_offset_x;
    size_t                              m_line_top;
    size_t                              m_line_top_offset;
    size_t                              m_lines;
    struct gapbuf                       m_gap;

private:

    edit();

    edit(
        const edit&);

    edit&
    operator=(edit const&);

    void
    caret_set_position() const;

    bool
    char_backspace();

    bool
    char_delete();

    bool
    char_insert(
        unsigned char const             i_char,
        enum cursor_mode const          i_cursor_mode);
    bool
    char_insert_newline();

    bool
    cursor_down();

    bool
    cursor_end();

    bool
    cursor_home();

    bool
    cursor_left();

    bool
    cursor_right();

    void
    cursor_show();

    bool
    cursor_up();

    bool
    move_to_offset_x(
        unsigned char __FAR*&           io_ptr,
        size_t&                         io_offset,
        int&                            io_pos_y) const;
};

inline void
edit::cursor_show()
{

    if (CURSOR_OVERWRITE == _cursor_mode)
    {
        cursor_block();
    }
    else
    {
        cursor_underline();
    }

    return;
}

inline unsigned char const __FAR*
edit::get_text()
{
    unsigned char const __FAR*          l_ptr;

    l_ptr= gapbuf_text(&m_gap);

    return l_ptr;
}

inline size_t
edit::get_text_length() const
{
    size_t                              l_size;

    l_size= m_gap.m_block_size - (m_gap.m_end - m_gap.m_start);

    return l_size;
}

#define __edit_hpp__
#endif
