/*
 ENTRY.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "entry.hpp"
#include "wm.hpp"
#include "draw.h"
#include "skin.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#if defined(__WATCOMC__) 
#include <malloc.h>
#endif

entry::entry(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_purpose(PURPOSE_ANY),
    m_caret(0),
    m_offset(0),
    m_slot(0),
    m_length(0),
    m_block_size(0),
    m_block(0),
    m_editable(true),
    m_signal_changed(0)
{
    grow(1);
    return;
}

entry::~entry()
{

    free(m_block);

    return;
}

void
entry::draw() const
{
    struct box                          l_bbox;
    size_t                              l_length;
    int                                 l_pos_y;

    get_bbox(l_bbox);

    draw_fill_area(
        l_bbox.m_pos_x,
        l_bbox.m_pos_y,
        l_bbox.m_len_x,
        l_bbox.m_len_y,
        0,
        _skin_window.m_text_fcolor,
        _skin_window.m_text_bcolor);

    do
    {

        if (0 == m_length)
        {
            break;
        }

        l_length= m_length - m_offset;

        if (l_bbox.m_len_x < l_length)
        {
            l_length= l_bbox.m_len_x;
        }

        l_pos_y= l_bbox.m_pos_y + (l_bbox.m_len_y / 2);
        
        draw_puts(
            l_bbox.m_pos_x,
            l_pos_y,
            m_block+m_offset,
            l_length,
            _skin_window.m_text_fcolor,
            _skin_window.m_text_bcolor);

    }while(0);

#if !defined(__CURSES__)
    if (DAMAGE_MOVED & m_damage)
#endif
    {
        caret_set_position();
    }

    return;
}

void
entry::grow(
    size_t const                        i_needed)
{
    unsigned char*                      l_block;
    size_t                              l_size;
    size_t                              l_size_alloc;

    do
    {

        l_size= m_length + i_needed;

        if (l_size < m_length)
        {
            break;
        }

        if (m_block_size >= l_size)
        {
            break;
        }

        l_size_alloc= 2 * l_size;

        if (l_size_alloc < l_size)
        {
            break;
        }

        if (128 > l_size_alloc)
        {
            l_size_alloc= 128;
        }

        l_block= reinterpret_cast<unsigned char*>(realloc(m_block, l_size_alloc));

        if (l_block)
        {
            m_block= l_block;
            m_block_size= l_size_alloc;
        }

    }while(0);

    return;
}

void
entry::del()
{
    unsigned char*                      l_dst;
    unsigned char*                      l_src;
    size_t                              l_slot;

    do
    {

        if (0 == m_length)
        {
            break;
        }

        l_src= m_block+m_slot+1;
        l_dst= m_block+m_slot;
        l_slot= m_slot;

        do
        {

            if (l_slot == m_length)
            {
                break;
            }

            *l_dst++= *l_src++;
            l_slot++;

        }while(1);

        m_length--;

    }while(0);

    return;
}

void
entry::ins(
    unsigned char const                 i_char)
{
    unsigned char*                      l_dst;
    unsigned char*                      l_src;
    size_t                              l_slot;

    do
    {

        if (m_block_size <= m_length)
        {
            break;
        }

        if (0 == m_length)
        {
            *m_block= i_char;
            m_length++;
            break;
        }
        
        l_dst= m_block + m_length;
        l_src= m_block + m_length-1;
        l_slot= m_length;
            
        do
        {
            if (m_slot == l_slot)
            {
                *(m_block+l_slot)= i_char;
                m_length++;
                break;
            }
            *l_dst--= *l_src--;
            l_slot--;
        }while(1);

    }while(0);

    pos_inc();

    return;
}

void
entry::ovw(
    unsigned char const                 i_char)
{

    if (m_block)
    {
        m_block[m_slot]= i_char;
    
        if (m_slot == m_length)
        {
            m_length++;
        }
    
        pos_inc();
    }

    return;
}

bool
entry::validate(
    unsigned char const             i_char)
{
    bool                            l_valid;

    l_valid= true;

    do
    {

        if (PURPOSE_ANY == m_purpose)
        {
            break;
        }

        if (PURPOSE_ALPHA == m_purpose)
        {
            l_valid= isalpha(static_cast<int>(i_char));
            break;
        }

        if (PURPOSE_DIGIT == m_purpose)
        {
            l_valid= isdigit(static_cast<int>(i_char));
            break;
        }

        if (PURPOSE_INTEGER == m_purpose)
        {
            l_valid= isdigit(static_cast<int>(i_char));
            if (!l_valid)
            {
                if ('-' == i_char)
                {
                    l_valid= true;
                }
            }
            break;
        }

        if (PURPOSE_FLOAT == m_purpose)
        {
            l_valid= isdigit(static_cast<int>(i_char));
            if (!l_valid)
            {
                switch(i_char)
                {
                    case '-':
                    case '+':
                    case '.':
                    case 'e':
                    case 'E':
                        l_valid= true;
                        break;
                    default:
                        break;
                }
            }
            break;
        }

    }while(0);

    return l_valid;
}
    
enum event_response
entry::event_key(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;
    size_t                              l_caret;
    bool                                l_valid;

    l_response= RESPONSE_NONE;
    l_caret= m_caret;

    do
    {

        if (false == m_editable)
        {
            // _mjo fixme should still be able to navigate left/right
            break;
        }

        if (0x20 <= i_event.m_ascii && 0x7f > i_event.m_ascii)
        {
            grow(1);

            l_valid= validate(i_event.m_ascii);

            if (!l_valid)
            {
                break;
            }

            if (CURSOR_INSERT == _cursor_mode)
            {
                ins(i_event.m_ascii);
            }
            else
            {
                ovw(i_event.m_ascii);
            }
            wm_draw_widget(this);
            break;
        }

        if (ASCII_BS == i_event.m_ascii)
        {
            if (m_slot)
            {
                pos_dec();
                del();
                wm_draw_widget(this);
            }
            break;
        }

        if (SCAN_DEL == i_event.m_scan)
        {
            if (m_length && m_slot != m_length)
            {
                del();
                wm_draw_widget(this);
            }
            break;
        }

        if (SCAN_RIGHT == i_event.m_scan)
        {
            if (pos_inc()) wm_draw_widget(this);
            break;
        }

        if (SCAN_LEFT == i_event.m_scan)
        {
            if (pos_dec()) wm_draw_widget(this);
            break;
        }

        if (SCAN_INS == i_event.m_scan)
        {
            _cursor_mode= static_cast<enum cursor_mode>(
		static_cast<int>(!_cursor_mode));
            show_cursor();
            break;
        }

    }while(0);

    if (m_caret != l_caret)
    {
        caret_set_position();
    }

    return l_response;
}

void
entry::focus_enter()
{
    show_cursor();
    caret_set_position();
}

void
entry::focus_leave()
{
    cursor_hide();
}

void
entry::caret_set_position() const
{
    group const*                        l_parent;
    struct box                          l_bbox;
    struct box                          l_wbox;
    int                                 l_pos_x;
    int                                 l_pos_y;

    get_bbox(l_bbox);
    l_parent= get_parent();
    if (l_parent)
    {
        (*l_parent).get_box(l_wbox);
    }
    else
    {
        l_wbox.m_pos_x= 0;
        l_wbox.m_pos_y= 0;
    }
    l_pos_x= 1 + l_wbox.m_pos_x + l_bbox.m_pos_x;
    l_pos_y= 1 + l_wbox.m_pos_y + l_bbox.m_pos_y;
    l_pos_x+= m_caret;
    cursor_set_position(l_pos_x, l_pos_y);

    return;
}

int
entry::pos_inc()
{
    int                                 l_overflow;
    struct box                          l_bbox;

    get_bbox(l_bbox);
    l_overflow= 0;

    do
    {

        if (m_length <= m_slot)
        {
            break;
        }

        l_overflow= 1;

        if ((l_bbox.m_len_x - 1) > m_caret)
        {
            if (m_length > m_caret)
            {
                m_caret++;
                l_overflow= 0;
            }
        }

        m_slot++;

        if (l_overflow)
        {
            m_offset++;
        }

    }while(0);

    return l_overflow;
}

int
entry::pos_dec()
{
    int                                 l_redraw;

    l_redraw= 0;

    if (0 == m_caret && m_offset)
    {
        l_redraw= 1;
        m_offset--;
    }

    if (m_caret)
    {
        m_caret--;
    }

    if (m_slot)
    {
        m_slot--;
    }

    return l_redraw;
}

void
entry::set_text(
    unsigned char const*                i_str)
{
    size_t                              l_length;

    m_caret= 0;
    m_offset= 0;
    m_slot= 0;

    do
    {

        if (0 == i_str || 0 == i_str[0])
        {
            m_length= 0;
            break;
        }

        l_length= strlen(reinterpret_cast<char const*>(i_str));
        grow(l_length+1);

        if (m_block_size < l_length)
        {
            l_length= m_block_size;
        }

        memcpy(m_block, i_str, l_length);

        m_length= l_length;

    }while(0);

    return;
}
