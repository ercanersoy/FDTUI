/*
 EDIT.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "edit.hpp"
#include "wm.hpp"
#include "skin.h"
#include "draw.h"
#include "screen.h"
#include "matrix.h"

edit::edit(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_caret_x(0),
    m_caret_y(0),
    m_sticky_caret_x(0),
    m_cursor(0),
    m_offset_x(0),
    m_line_top(0),
    m_line_top_offset(0),
    m_lines(0),
    m_gap()
{

    gapbuf_assign(&m_gap);

    return;
}

edit::~edit()
{

    gapbuf_discharge(&m_gap);

    return;
}

void
edit::caret_set_position() const
{
    group const*                        l_parent;
    struct box                          l_bbox;
    struct box                          l_wbox;
    int                                 l_pos_x;
    int                                 l_pos_y;

    get_box(l_bbox);
    l_parent= get_parent();
    (*l_parent).get_box(l_wbox);
    l_pos_x= 1 + l_wbox.m_pos_x + l_bbox.m_pos_x;
    l_pos_y= 1 + l_wbox.m_pos_y + l_bbox.m_pos_y;
    l_pos_x+= (1+m_caret_x);
    l_pos_y+= (1+m_caret_y);
    cursor_set_position(l_pos_x, l_pos_y);

    return;
}

bool
edit::char_backspace()
{
    size_t                              l_caret_x;
    size_t                              l_caret_y;
    size_t                              l_cursor;
    unsigned char __FAR*                l_ptr;
    bool                                l_redraw;

    l_caret_x= m_caret_x;
    l_caret_y= m_caret_y;
    l_redraw= false;

    do
    {

        if (0 == m_cursor)
        {
            break;
        }

        if (m_caret_x || m_offset_x)
        {
            l_redraw= gapbuf_backspace(&m_gap, m_cursor);
            if (l_redraw)
            {
                m_cursor--;
                if (m_caret_x)
                {
                    m_caret_x--;
                }
                else
                {
                    m_offset_x--;
                }
            }
            break;
        }

        m_caret_x= 0;
        l_cursor= m_cursor - 1;
        l_ptr= gapbuf_line_prev(&l_cursor, &m_gap);

        if (l_ptr)
        {
            l_ptr= gapbuf_char_next(&l_cursor, &m_gap);
            if (m_caret_y)
            {
                m_caret_y--;
            }
            else
            {
                m_line_top--;
                m_line_top_offset= l_cursor;
            }
        }
        else
        {
            m_caret_y= 0;
            m_line_top_offset= 0;
            m_line_top= 0;
            l_cursor= 0;
            l_ptr= gapbuf_ptr(&m_gap, 0);
        }

        do
        {

            if ('\n' == (*l_ptr))
            {
                break;
            }

            if ((1 + m_caret_x) < (m_box.m_len_x - 2))
            {
                m_caret_x++;
            }
            else
            {
                m_offset_x++;
            }

            l_ptr= gapbuf_char_next(&l_cursor, &m_gap);

            if (0 == l_ptr)
            {
                break;
            }

        }while(1);

        m_cursor--;
        char_delete();
        l_redraw= true;

    }while(0);

    if (l_caret_x != m_caret_x || l_caret_y != m_caret_y)
    {
        caret_set_position();
    }

    return l_redraw;
}

bool
edit::char_delete()
{
    bool                                l_redraw;
    unsigned char __FAR*                l_ptr;

    l_redraw= false;

    do
    {

        l_ptr= gapbuf_ptr(&m_gap, m_cursor);

        if (0 == l_ptr)
        {
            break;
        }

        if ('\n' == (*l_ptr))
        {
            m_lines--;
        }

        l_redraw= gapbuf_delete(&m_gap, m_cursor);

    }while(0);

    return l_redraw;
}

bool
edit::char_insert(
    unsigned char const                 i_char,
    enum cursor_mode const              i_cursor_mode)
{
    bool                                l_inserted;
    unsigned char __FAR*                l_ptr;

    l_ptr= gapbuf_ptr(&m_gap, m_cursor);

    if ((l_ptr && '\n' == (*l_ptr)) || CURSOR_INSERT == i_cursor_mode)
    {
        l_inserted= !gapbuf_insert(&m_gap, m_cursor, i_char);
    }
    else
    {
        l_inserted= !gapbuf_overwrite(&m_gap, m_cursor, i_char);
    }

    do
    {

        if (false == l_inserted)
        {
            break;
        }

        m_cursor++;

        if ((1 + m_caret_x) < (m_box.m_len_x - 2))
        {
            m_caret_x++;
            caret_set_position();
            break;
        }

        m_offset_x++;

    }while(0);

    return l_inserted;
}

bool
edit::char_insert_newline()
{
    bool                                l_redraw;

    do
    {

        l_redraw= char_insert('\n', CURSOR_INSERT);

        if (false == l_redraw)
        {
            break;
        }

        m_lines++;
        m_offset_x= 0;
        m_sticky_caret_x= 0;
        m_caret_x= 0;

        if ((1 + m_caret_y) < (m_box.m_len_y - 2))
        {
            m_caret_y++;
        }
        else
        {
            m_line_top++;
            gapbuf_line_next(&m_line_top_offset, &m_gap);
        }

        caret_set_position();

    }while(0);

    return l_redraw;
}

bool
edit::cursor_down()
{
    size_t                              l_caret_x;
    size_t                              l_caret_y;
    size_t                              l_cursor;
    size_t                              l_offset;
    size_t                              l_offset_x;
    bool                                l_redraw;
    unsigned char const __FAR*          l_ptr;

    l_caret_x= m_caret_x;
    l_caret_y= m_caret_y;
    l_offset_x= m_offset_x;
    l_redraw= false;

    do
    {

        if (m_line_top + m_caret_y >= m_lines)
        {
            break;
        }

        if ((1 + m_caret_y) < (m_box.m_len_y - 2))
        {
            m_caret_y++;
        }
        else
        {
            m_line_top++;
            gapbuf_line_next(&m_line_top_offset, &m_gap);
            l_redraw= true;
        }

        l_cursor= m_cursor;
        l_ptr= gapbuf_line_next(&l_cursor, &m_gap);

        if (0 == l_ptr)
        {
            m_caret_x= 0;
            m_offset_x= 0;
            m_cursor= m_gap.m_block_size - (m_gap.m_end - m_gap.m_start);
            break;
        }

        l_offset= 0;
        m_caret_x= 0;
        m_cursor= l_cursor;
        m_offset_x= 0;

        if ('\n' == (*l_ptr))
        {
            break;
        }

        do
        {

            if (l_offset == m_sticky_caret_x)
            {
                break;
            }

            l_ptr= gapbuf_char_next(&l_cursor, &m_gap);

            if (0 == l_ptr)
            {
                break;
            }

            m_cursor= l_cursor;

            if ((1 + m_caret_x) < (m_box.m_len_x - 2))
            {
                m_caret_x++;
            }
            else
            {
                m_offset_x++;
            }

            if ('\n' == (*l_ptr))
            {
                break;
            }

            l_offset++;

        }while(1);

    }while(0);

    if (m_offset_x != l_offset_x)
    {
        l_redraw= true;
    }

    if (l_caret_x != m_caret_x || l_caret_y != m_caret_y)
    {
        caret_set_position();
    }

    return l_redraw;
}

bool
edit::cursor_end()
{
    size_t                              l_caret_x;
    size_t                              l_cursor;
    size_t                              l_offset_x;
    unsigned char __FAR*                l_ptr;
    bool                                l_redraw;

    l_caret_x= m_caret_x;
    l_offset_x= m_offset_x;
    l_cursor= m_cursor;
    l_ptr= gapbuf_ptr(&m_gap, m_cursor);

    do
    {

        if (0 == l_ptr)
        {
            break;
        }
    
        if ('\n' == (*l_ptr))
        {
            break;
        }

        m_cursor++;

        if ((1 + m_caret_x) < (m_box.m_len_x - 2))
        {
            m_caret_x++;
        }
        else
        {
            m_offset_x++;
        }

        l_ptr= gapbuf_ptr(&m_gap, m_cursor);

    }while(1);

    m_sticky_caret_x= (m_caret_x + m_offset_x);

    if (l_caret_x != m_caret_x || l_offset_x != m_offset_x)
    {
        caret_set_position();
    }

    l_redraw= (m_cursor != l_cursor);

    return l_redraw;
}

bool
edit::cursor_home()
{
    bool                                l_redraw;
    unsigned char __FAR*                l_ptr;

    l_redraw= (m_caret_x || m_offset_x);

    if (l_redraw)
    {
        l_ptr= gapbuf_line_prev(&m_cursor, &m_gap);
        if (l_ptr)
        {
            gapbuf_char_next(&m_cursor, &m_gap);
        }
        else
        {
            m_cursor= 0;
        }
        m_caret_x= 0;
        m_offset_x= 0;
    }

    m_sticky_caret_x= 0;

    return l_redraw;
}

bool
edit::cursor_left()
{
    bool                                l_redraw;

    l_redraw= false;

    do
    {

        if (0 == m_caret_x)
        {
            if (0 == m_offset_x)
            {
                break;
            }
            m_offset_x--;
            l_redraw= true;
        }
        else
        {
            m_caret_x--;
            caret_set_position();
        }

        m_cursor--;

    }while(0);

    m_sticky_caret_x= (m_caret_x + m_offset_x);

    return l_redraw;
}

bool
edit::cursor_right()
{
    bool                                l_redraw;
    unsigned char const __FAR*          l_ptr;

    l_redraw= false;

    do
    {

        l_ptr= gapbuf_ptr(&m_gap, m_cursor);
    
        if (0 == l_ptr)
        {
            break;
        }

        if ('\n' == (*l_ptr))
        {
            break;
        }

        m_cursor++;

        if ((1 + m_caret_x) < (m_box.m_len_x - 2))
        {
            m_caret_x++;
            caret_set_position();
            break;
        }

        m_offset_x++;
        l_redraw= true;

    }while(0);

    m_sticky_caret_x= (m_caret_x + m_offset_x);

    return l_redraw;
}

bool
edit::cursor_up()
{
    size_t                              l_caret_x;
    size_t                              l_caret_y;
    size_t                              l_cursor;
    size_t                              l_offset;
    size_t                              l_offset_x;
    unsigned char const __FAR*          l_ptr;
    bool                                l_redraw;

    l_redraw= false;
    l_caret_x= m_caret_x;
    l_caret_y= m_caret_y;
    l_offset_x= m_offset_x;

    do
    {


        l_cursor= m_cursor;
        l_ptr= gapbuf_line_prev(&l_cursor, &m_gap);

        if (0 == l_ptr)
        {
            if (m_caret_y)
            {
                if (m_line_top)
                {
                    m_line_top= 0;
                    m_line_top_offset= 0;
                    l_redraw= true;
                }
                m_caret_y= 0;
                m_caret_x= 0;
                m_offset_x= 0;
                m_cursor= 0;
            }
            break;
        }

        l_ptr= gapbuf_line_prev(&l_cursor, &m_gap);

        if (l_ptr)
        {
            l_ptr= gapbuf_char_next(&l_cursor, &m_gap);
            if (m_caret_y)
            {
                m_caret_y--;
            }
            else if (m_line_top)
            {
                m_line_top--;
                m_line_top_offset= l_cursor;
                l_redraw= true;
            }
        }
        else
        {
            l_ptr= gapbuf_ptr(&m_gap, 0);
            l_cursor= 0;
            m_caret_y= 0;
            if (m_line_top)
            {
                m_line_top= 0;
                m_line_top_offset= 0;
                l_redraw= true;
            }
        }

        l_offset= 0;
        m_caret_x= 0;
        m_cursor= l_cursor;
        m_offset_x= 0;

        if (0 == l_ptr || '\n' == (*l_ptr))
        {
            break;
        }

        do
        {

            if (l_offset == m_sticky_caret_x)
            {
                break;
            }

            l_ptr= gapbuf_char_next(&l_cursor, &m_gap);

            if (0 == l_ptr)
            {
                break;
            }

            m_cursor= l_cursor;

            if ((1 + m_caret_x) < (m_box.m_len_x - 2))
            {
                m_caret_x++;
            }
            else
            {
                m_offset_x++;
            }

            if ('\n' == (*l_ptr))
            {
                break;
            }

            l_offset++;

        }while(1);

    }while(0);

    if (m_offset_x != l_offset_x)
    {
        l_redraw= true;
    }

    if (l_caret_x != m_caret_x || l_caret_y != m_caret_y)
    {
        caret_set_position();
    }

    return l_redraw;
}

void
edit::focus_enter()
{
    cursor_show();
    caret_set_position();
}

void
edit::focus_leave()
{
    cursor_hide();
}

bool
edit::move_to_offset_x(
    unsigned char __FAR*&               io_ptr,
    size_t&                             io_offset,
    int&                                io_pos_y) const
{
    bool                                l_valid;
    size_t                              l_counter;

    l_valid= true;
    l_counter= 0;

    do
    {

        if (l_counter == m_offset_x)
        {
            break;
        }

        if (0 == io_ptr)
        {
            l_valid= false;
            break;
        }

        if ('\n' != (*io_ptr))
        {
            l_counter++;
        }
        else
        {
            l_counter= 0;
            io_pos_y++;
            if (io_pos_y >= static_cast<int>(m_box.m_len_y - 2))
            {
                l_valid= false;
                break;
            }
        }

        io_ptr= gapbuf_char_next(&io_offset, &m_gap);

    }while(1);

    return l_valid;
}

void
edit::draw() const
{
    bool                                l_bool;
    size_t                              l_offset;
    int                                 l_pos_x;
    int                                 l_pos_y;
    unsigned char __FAR*                l_ptr;

    screen_push(1);

    draw_fill_area(
        m_box.m_pos_x,
        m_box.m_pos_y,
        m_box.m_len_x,
        m_box.m_len_y,
        0,
        _skin_edit.m_fcolor,
        _skin_edit.m_bcolor);

    draw_frame(
        &m_box,
        &_skin_frame_single,
        _skin_edit.m_fcolor,
        _skin_edit.m_bcolor);

    matrix_push();
    matrix_translate(m_box.m_pos_x, m_box.m_pos_y);

    l_offset= m_line_top_offset;
    l_pos_x= 0;
    l_pos_y= 0;
    l_ptr= gapbuf_ptr(&m_gap, l_offset);
    move_to_offset_x(l_ptr, l_offset, l_pos_y);

    do
    {

        if (0 == l_ptr)
        {
            break;
        }

        if (l_pos_y >= static_cast<int>(m_box.m_len_y - 2))
        {
            break;
        }

        if ('\n' == (*l_ptr))
        {

            l_ptr= gapbuf_char_next(&l_offset, &m_gap);

draw_move_to_offset:

            if (0 == l_ptr)
            {
                break;
            }

            l_pos_x= 0;
            l_pos_y++;

            if ('\n' != (*l_ptr) && m_offset_x)
            {
                l_bool= move_to_offset_x(l_ptr, l_offset, l_pos_y);
                if (false == l_bool)
                {
                    break;
                }
            }

            continue;
        }

        if (l_pos_x >= static_cast<int>((m_box.m_len_x - 2)))
        {
            l_ptr= gapbuf_line_next(&l_offset, &m_gap);
            goto draw_move_to_offset;
        }

        draw_putc(
            (1+l_pos_x),
            (1+l_pos_y),
            (*l_ptr),
            1,
            _skin_edit.m_fcolor,
            _skin_edit.m_bcolor);

        l_pos_x++;
        l_ptr= gapbuf_char_next(&l_offset, &m_gap);

    }while(1);

    matrix_pop();

    caret_set_position();
    screen_pop();

    return;
}

enum event_response
edit::event_key(
    struct event_key const&             i_event)
{
    bool                                l_redraw;
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;
    l_redraw= false;

    do
    {

        if ('\r' == i_event.m_ascii)
        {
            l_redraw= char_insert_newline();
            break;
        }

        if (0x20 <= i_event.m_ascii && 0x7f > i_event.m_ascii)
        {
            l_redraw= char_insert(i_event.m_ascii, _cursor_mode);
            m_sticky_caret_x= (m_caret_x + m_offset_x);
            break;
        }

        if (SCAN_INS == i_event.m_scan)
        {
            _cursor_mode= static_cast<enum cursor_mode>(
                static_cast<int>(!_cursor_mode));
            cursor_show();
            break;
        }

        if (SCAN_LEFT == i_event.m_scan)
        {
            l_redraw= cursor_left();
            break;
        }

        if (SCAN_RIGHT == i_event.m_scan)
        {
            l_redraw= cursor_right();
            break;
        }

        if (SCAN_DOWN == i_event.m_scan)
        {
            l_redraw= cursor_down();
            break;
        }

        if (SCAN_UP == i_event.m_scan)
        {
            l_redraw= cursor_up();
            break;
        }

        if (SCAN_DEL == i_event.m_scan)
        {
            l_redraw= char_delete();
            break;
        }

        if (ASCII_BS == i_event.m_ascii)
        {
            l_redraw= char_backspace();
            break;
        }

        if (SCAN_HOME == i_event.m_scan)
        {
            l_redraw= cursor_home();
            break;
        }

        if (SCAN_END == i_event.m_scan)
        {
            l_redraw= cursor_end();
            break;
        }

    }while(0);

    if (l_redraw)
    {
        wm_draw_widget(this);
    }

    return l_response;
}

