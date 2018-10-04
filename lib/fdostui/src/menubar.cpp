/*
 MENUBAR.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "menubar.hpp"
#include "menu.hpp"
#include "wm.hpp"
#include "skin.h"
#include "draw.h"
#include "mouse.h"
#include "block.h"
#include "poll.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

menubar::menubar(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_selected(0),
    m_item(0),
    m_item_count(0)
{

    return;
}

menubar::~menubar()
{
    free(m_item);
    return;
}

void
menubar::set_menu(
    struct menuitembar const*           i_menu)
{
    size_t                              l_len;
    int                                 l_pos_x;
    size_t                              l_slot;

    free(m_item);
    m_item_count= 0;

    for (l_slot= 0; i_menu[l_slot].m_label; l_slot++);

    m_item_count= l_slot;

    m_item= reinterpret_cast<struct item*>(calloc(l_slot, sizeof(struct item)));

    l_pos_x= m_box.m_pos_x+1;

    for (l_slot= 0; i_menu[l_slot].m_label; l_slot++)
    {
        l_len= strlen(reinterpret_cast<char const*>(i_menu[l_slot].m_label));
        m_item[l_slot].m_menu= &i_menu[l_slot];
        m_item[l_slot].m_pos_x= l_pos_x;
        m_item[l_slot].m_len_x= l_len;
        l_pos_x+= l_len+2;
    }

    return;
}

void
menubar::draw_item(
    struct item const&                  i_item) const
{
    enum background                     l_bcolor;
    enum foreground                     l_fcolor;

    l_fcolor= _skin_menu.m_window.m_text_fcolor;
    l_bcolor= _skin_menu.m_window.m_text_bcolor;

    draw_puts(
        i_item.m_pos_x,
        m_box.m_pos_y,
        i_item.m_menu->m_label,
        i_item.m_len_x,
        l_fcolor,
        l_bcolor);

    if (MENUITEM_MNEMONIC_NONE != i_item.m_menu->m_mnemonic_offset)
    {

#if defined(__CURSES__)
        if (!_video_has_color)
        {
            attron(A_UNDERLINE);
        }
#endif

        draw_putc(
            i_item.m_pos_x+i_item.m_menu->m_mnemonic_offset,
            m_box.m_pos_y,
            i_item.m_menu->m_label[i_item.m_menu->m_mnemonic_offset],
            1,
            _skin_menu.m_hotkey_fcolor,
            _skin_menu.m_hotkey_bcolor);

#if defined(__CURSES__)
        if (!_video_has_color)
        {
            attroff(A_UNDERLINE);
        }
#endif

    }

    return;
}

void
menubar::draw() const
{
    size_t                              l_slot;

    draw_fill_area(
        m_box.m_pos_x,
        m_box.m_pos_y,
        m_box.m_len_x,
        m_box.m_len_y,
        0,
        _skin_menu.m_window.m_text_fcolor,
        _skin_menu.m_window.m_text_bcolor);

    for (l_slot= 0; m_item_count > l_slot; l_slot++)
    {
        draw_item(m_item[l_slot]);
    }

    return;
}

enum event_response
menubar::event_key_default(
   enum scancodes const                 i_scan)
{
    group*                              l_group;
    struct menuitembar const*           l_itembar;
    struct menuitem*                    l_itemmenu;
    enum event_response                 l_response;
    size_t                              l_slot;

    l_response= RESPONSE_NONE;

    do
    {

        if (false == get_can_focus())
        {
            break;
        }

        l_group= get_parent();

        for (l_slot= 0; m_item_count > l_slot; l_slot++)
        {
            l_itembar= m_item[l_slot].m_menu;
            if (i_scan == (*l_itembar).m_accel_scancode)
            {
                if (l_group)
                {
                    (*l_group).set_focus(*this);
                }
                popup(m_item[l_slot]);
                l_response= RESPONSE_HANDLED;
                break;
            }
        }

        if (RESPONSE_HANDLED == l_response)
        {
            break;
        }

        for (l_slot= 0; m_item_count > l_slot; l_slot++)
        {
            l_itembar= m_item[l_slot].m_menu;
            l_itemmenu= (*l_itembar).m_items;
            if (l_itemmenu)
            {
                l_itemmenu= menuitem_find_scancode(l_itemmenu, i_scan);
                if (l_itemmenu)
                {
                    menuitem_emit(l_itemmenu);
                    l_response= RESPONSE_HANDLED;
                    break;
                }
            }
        }

    }while(0);

    return l_response;
}

enum event_response
menubar::event_key(
    struct event_key const&             i_event)
{
    bool                                l_activate;
    struct menuitembar const*           l_itembar;
    struct menuitem*                    l_itemmenu;
    size_t                              l_slot;
    unsigned char                       l_hotkey;

    l_activate= false;

    do
    {

        if (i_event.m_ascii)
        {
            for (l_slot= 0; m_item_count > l_slot; l_slot++)
            {
                l_itembar= m_item[l_slot].m_menu;
                if (MENUITEM_MNEMONIC_NONE != (*l_itembar).m_mnemonic_offset)
                {
                    l_hotkey= (*l_itembar).m_label[(*l_itembar).m_mnemonic_offset];
                    if (toupper(i_event.m_ascii) == toupper(l_hotkey))
                    {
                       m_selected= l_slot;
                       l_activate= true;
                       break;
                    }
                }
            }
        }

        if (0 == i_event.m_scan)
        {
            break;
        }

        if (SCAN_DOWN == i_event.m_scan)
        {
            l_activate= true;
            break; 
        }

        if (SCAN_LEFT == i_event.m_scan)
        {
            if (m_selected)
            {
                m_selected--;
            }
            else
            {
                m_selected= m_item_count - 1;
            }
            l_activate= true;
            break;
        }

        if (SCAN_RIGHT == i_event.m_scan)
        {
            m_selected++;
            if (m_item_count <= m_selected)
            {
                m_selected= 0;
            }
            l_activate= true;
            break;
        }

        for (l_slot= 0; m_item_count > l_slot; l_slot++)
        {

            l_itembar= m_item[l_slot].m_menu;
            l_itemmenu= (*l_itembar).m_items;
            if (l_itemmenu)
            {
                l_itemmenu= menuitem_find_scancode(l_itemmenu, i_event.m_scan);
                if (l_itemmenu)
                {
                    menuitem_emit(l_itemmenu);
                    break;
                }
            }
        }

    }while(0);

    if (l_activate)
    {
        popup(m_item[m_selected]);
    }

    return RESPONSE_NONE;
}

enum event_response
menubar::event_mouse(
    struct event_mouse const&           i_event)
{
    size_t                              l_slot;
    bool                                l_hit;
    struct item*                        l_item;

    do
    {

        if (MOUSE_BUTTON_CLICKED != i_event.m_state)
        {
            break;
        }

        if (static_cast<int>(i_event.m_pos_y) != 
            static_cast<int>(m_box.m_pos_y))
        {
            break;
        }

        for (l_slot= 0; m_item_count > l_slot; l_slot++)
        {
            l_item= &m_item[l_slot];

            l_hit= (
                static_cast<int>(i_event.m_pos_x) >= 
                static_cast<int>((*l_item).m_pos_x) &&
                static_cast<int>(i_event.m_pos_x) <= 
                static_cast<int>((*l_item).m_pos_x+(*l_item).m_len_x-1)
                   );

            if (l_hit)
            {
                popup(*l_item);
                break;
            }
        }

    }while(0);

    return RESPONSE_NONE;
}

void
menubar::popup(
    struct item&                        i_item)
{
    struct box                          l_backing_box;
    void __FAR*                         l_backing_store;
    struct event_key                    l_key;
    menu*                               l_menu;
    int                                 l_response;
    int                                 l_pos_x;
    int                                 l_pos_y;

    translate_screen(&l_pos_x, &l_pos_y);

    do
    {

        if (0 == i_item.m_menu->m_items)
        {
            break;
        }

        l_menu= new menu(l_pos_x+i_item.m_pos_x, l_pos_y+1, _video_cols, _video_rows);
        (*l_menu).set_menu(i_item.m_menu->m_items);
        (*l_menu).set_response_extended(true);
        (*l_menu).get_box(l_backing_box);
    
        mouse_hide();
        l_backing_store= block_read(&l_backing_box);
        (*l_menu).draw();
        mouse_show();
    
        l_response= static_cast<int>(wm_run_modal(*l_menu));
    
        mouse_hide();
        block_write(l_backing_store, &l_backing_box);
        mouse_show();
    
        delete l_menu;
        block_free(l_backing_store);
    
        memset(&l_key, 0, sizeof(l_key));
    
        if (menu::RESPONSE_LEFT == l_response)
        {
            l_key.m_scan= SCAN_LEFT;
            poll_keyboard_unget(&l_key);
        }
        else if (menu::RESPONSE_RIGHT == l_response)
        {
            l_key.m_scan= SCAN_RIGHT;
            poll_keyboard_unget(&l_key);
        }

    }while(0);

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        ::move(l_pos_y, l_pos_x+i_item.m_pos_x);
    }
#endif

    return;
}
