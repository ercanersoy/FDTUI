/*
 BUTTON.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "button.hpp"
#include "wm.hpp"
#include "wmhelper.hpp"

button::button(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_state(false),
    m_type(TYPE_BUTTON),
    m_style(STYLE_FLAT),
    m_label(0),
    m_response(RESPONSE_HANDLED),
    m_signal_clicked(0)
{
    return;
}
    
button::~button()
{
    free(m_label);
    return;
}

void
button::draw() const
{
    struct box                          l_box;
    unsigned char                       l_btn[4];
    int                                 l_center_x;
    int                                 l_center_y;
    int                                 l_length;
    enum foreground                     l_foreground;
    int                                 l_is_focused;

    l_is_focused= (m_flags & widget::HAS_FOCUS);

    if (0 == l_is_focused)
    {
        l_foreground= _skin_button.m_fcolor;
    }
    else
    {
        l_foreground= _skin_button.m_focused;
    }

    l_box= m_box;

    if ((STYLE_SHADOW & m_style) && (2 <= l_box.m_len_y))
    {
        if (l_box.m_len_x)
        {
            l_box.m_len_x--;
        }
        l_box.m_len_y--;
    }

    draw_fill_area(
        l_box.m_pos_x,
        l_box.m_pos_y,
        l_box.m_len_x,
        l_box.m_len_y,
        0,
        _skin_button.m_fcolor,
        _skin_button.m_bcolor);

    if ((STYLE_BOX & m_style) && (3 <= m_box.m_len_y))
    {
        draw_frame(
            &l_box,
            &_skin_frame_single,
            _skin_button.m_fcolor,
            _skin_button.m_bcolor);
    }

    do
    {

        if (0 == (STYLE_SHADOW & m_style))
        {
            break;
        }

        if ((STYLE_BOX & m_style) && (4 > m_box.m_len_y))
        {
            break;
        }
        else if (2 > m_box.m_len_y)
        {
            break;
        }

        draw_putc(
            m_box.m_pos_x+1,
            m_box.m_pos_y+m_box.m_len_y-1,
            0,
            m_box.m_len_x-1,
            _skin_button.m_fcolor,
            _skin_button.m_shadow);

        draw_putvc(
            m_box.m_pos_x+m_box.m_len_x-1,
            m_box.m_pos_y+1,
            0,
            m_box.m_len_y-2,
            _skin_button.m_fcolor,
            _skin_button.m_shadow);

    }while(0);

    do
    {

        if (0 == m_label)
        {
            break;
        }

        l_length= static_cast<int>(strlen(reinterpret_cast<char*>(m_label)));

        if (TYPE_BUTTON == m_type)
        {
            l_center_x= l_box.m_pos_x + ((l_box.m_len_x / 2) - (l_length / 2));

            if (l_center_x < l_box.m_pos_x)
            {
                l_center_x= l_box.m_pos_x + 1;
                l_length= l_box.m_len_x - 2;
            }
        }
        else
        {
            l_center_x= l_box.m_pos_x + 5;
            if (static_cast<int>(l_center_x + l_length) > 
                static_cast<int>(l_box.m_pos_x + l_box.m_len_x))
            {
                l_length= l_box.m_len_x - 5;
            }
        }

        if (0 < l_length)
        {
            l_center_y= l_box.m_pos_y + (l_box.m_len_y / 2);
            draw_puts(
                l_center_x,
                l_center_y,
                m_label,
                l_length,
                l_foreground,
                _skin_button.m_bcolor);
        }

    }while(0);

    do
    {

        if (TYPE_BUTTON == m_type)
        {
            break;
        }

        if (5 > l_box.m_len_x)
        {
            break;
        }


        if (TYPE_CHECK == m_type)
        {
            l_btn[0]= '[';
            l_btn[1]= (m_state) ? 'X' : ' ';
            l_btn[2]= ']';
            l_btn[3]= 0;
        }
        else
        {
            l_btn[0]= '(';
            l_btn[1]= (m_state) ? '*' : ' ';
            l_btn[2]= ')';
            l_btn[3]= 0;
        }

        l_center_y= l_box.m_pos_y + (l_box.m_len_y / 2);

        draw_puts(
            m_box.m_pos_x+1,
            l_center_y,
            l_btn,
            3,
            l_foreground,
            _skin_button.m_bcolor);

    }while(0);

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        int                             l_cur_x;
        int                             l_cur_y;
        translate_screen(&l_cur_x, &l_cur_y);
        if (TYPE_CHECK == m_type || TYPE_RADIO == m_type)
        {
            l_cur_x+= 2;
        }
        ::move(l_cur_y, l_cur_x);
    }
#endif

    return;
}

void
button::focus_enter()
{
    widget::focus_enter();
    wm_draw_widget(this);
    return;
}

void
button::focus_leave()
{
    widget::focus_leave();
    wm_draw_widget(this);
    return;
}

enum event_response
button::event_key(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    do
    {

        if (ASCII_ENTER != i_event.m_ascii)
        {
            if (TYPE_BUTTON == m_type)
            {
                break;
            }
            if (ASCII_SPACE != i_event.m_ascii)
            {
                break;
            }
        }

        if (TYPE_RADIO == m_type && m_state)
        {
            break;
        }

        m_state= !m_state;

        if (TYPE_BUTTON != m_type)
        {
            set_damage(DAMAGE_VALUE);
            wm_draw_widget(this);
            toggled();
        }

        emit_clicked();
        l_response= m_response;

    }while(0);

    return l_response;
}

enum event_response
button::event_mouse(
    struct event_mouse const&           i_event)
{
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    do
    {

        if (MOUSE_BUTTON_CLICKED != i_event.m_state)
        {
            break;
        }

        if (TYPE_RADIO == m_type && m_state)
        {
            break;
        }

        m_state= !m_state;

        if (TYPE_BUTTON != m_type)
        {
            set_damage(DAMAGE_VALUE);
            wm_draw_widget(this);
            toggled();
        }

        emit_clicked();
        l_response= m_response;

    }while(0);

    return l_response;
}
