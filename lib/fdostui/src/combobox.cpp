/*
 COMBOBOX.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "combobox.hpp"
#include "draw.h"
#include "skin.h"
#include "cursor.h"
#include "block.h"
#include "mouse.h"
#include "wm.hpp"

combobox::combobox(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    entry(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_listbox(i_pos_x, i_pos_y+1, i_len_x, 10)
{

    m_listbox.set_response(RESPONSE_ACCEPT);

    return;
}

combobox::~combobox()
{
    return;
}

void
combobox::draw() const
{
    int                                 l_pos_y;

    entry::draw();

    l_pos_y= m_box.m_pos_y + (m_box.m_len_y / 2);

    draw_putc(
        m_box.m_pos_x + m_box.m_len_x - 1,
        l_pos_y,
        _skin_buttons.m_down,
        1,
        _skin_slider.m_fcolor,
        _skin_slider.m_bcolor);
        
    return;
}

enum event_response
combobox::event_key(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    if (SCAN_DOWN == i_event.m_scan)
    {
        popup();
        l_response= RESPONSE_HANDLED;
    }

    return l_response;
}

enum event_response
combobox::event_mouse(
    struct event_mouse const&           i_event)
{
    enum event_response                 l_response;
    int                                 l_pos_y;

    l_response= RESPONSE_NONE;

    do
    {

        l_pos_y= m_box.m_pos_y + (m_box.m_len_y / 2);

        if (static_cast<int>(i_event.m_pos_y) == static_cast<int>(l_pos_y) &&
            static_cast<int>(i_event.m_pos_x) == 
            static_cast<int>(m_box.m_pos_x + m_box.m_len_x - 1))
        {
            if (MOUSE_BUTTON_CLICKED == i_event.m_state)
            {
                popup();
                l_response= RESPONSE_HANDLED;
            }
            break;
        }

        l_response= entry::event_mouse(i_event);

    }while(0);

    return l_response;
}

void
combobox::listbox_configure()
{
    struct box                          l_box;
    size_t                              l_items;

    translate_screen(&l_box.m_pos_x, &l_box.m_pos_y);

    if (0 > l_box.m_pos_x)
    {
        l_box.m_pos_x= 0;
    }

    l_box.m_len_x= m_box.m_len_x;
    l_items= m_listbox.get_count();

    if (_video_rows > l_items)
    {
        l_box.m_len_y= 2 + l_items;
    }
    else
    {
        l_box.m_pos_y= 0;
        l_box.m_len_y= _video_rows;
    }

    do
    {

        if (_video_rows >= l_box.m_pos_y + l_box.m_len_y)
        {
            break;
        }

        l_box.m_pos_y--;

    }while(1);

    m_listbox.move(l_box.m_pos_x, l_box.m_pos_y);
    m_listbox.resize(l_box.m_len_x, l_box.m_len_y);
    m_listbox.unselect_all();

    return;
}

void
combobox::popup()
{
    void __FAR*                         l_backing_store;
    struct box                          l_box;
    unsigned char const*                l_item;
    enum event_response                 l_response;
    int                                 l_selected;

    do
    {

        listbox_configure();
        m_listbox.get_box(l_box);

        cursor_hide();

        mouse_hide();
        l_backing_store= block_read(&l_box);
        m_listbox.draw();
        mouse_show();
    
        l_response= wm_run_modal(m_listbox);
    
        mouse_hide();
        block_write(l_backing_store, &l_box);
        mouse_show();
    
        block_free(l_backing_store);

        if (RESPONSE_ACCEPT == l_response)
        {
            l_selected= m_listbox.get_selected_first();
            l_item= m_listbox.get_item(l_selected);
            set_text(l_item);
            set_damage(DAMAGE_VALUE);
            wm_draw_widget(this);
            emit_changed();
        }

        show_cursor();

    }while(0);

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        int                             l_cur_x;
        int                             l_cur_y;
        translate_screen(&l_cur_x, &l_cur_y);
        ::move(l_cur_y, l_cur_x);
    }
#endif

    return;
}
