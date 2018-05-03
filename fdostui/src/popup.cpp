/*
 POPUP.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "popup.hpp"
#include "wm.hpp"
#include "label.hpp"
#include "button.hpp"
#include "mouse.h"
#include "block.h"
#include "screen.h"

extern enum event_response
popup_menu(
    struct menuitem *                   io_menu,
    int const                           i_pos_x,
    int const                           i_pos_y)
{
    struct box                          l_backing_box;
    void __FAR*                         l_backing_store;
    menu*                               l_menu;
    enum event_response                 l_response;

    l_menu= new menu(i_pos_x, i_pos_y, _video_cols, _video_rows);
    (*l_menu).set_menu(io_menu);
    (*l_menu).get_box(l_backing_box);

    mouse_hide();
    l_backing_store= block_read(&l_backing_box);
    (*l_menu).draw();
    mouse_show();
    l_response= wm_run_modal(*l_menu);
    delete l_menu;

    mouse_hide();
    block_write(l_backing_store, &l_backing_box);
    mouse_show();

    block_free(l_backing_store);

    return l_response;
}

extern enum event_response
popup_message(
    unsigned char const*                i_title,
    unsigned char const*                i_message)
{
    enum event_response                 l_response;

    window* l_dialog= new window(20,5,40,15);
    (*l_dialog).set_attributes(window::BORDER|window::TITLE);
    (*l_dialog).set_title(i_title);

    label* l_label= new label(0,0,38,10);
    (*l_label).set_text(i_message);

    button* l_button_ok= new button(15,10,10,3);
    (*l_button_ok).set_style(button::STYLE_BOX);
    (*l_button_ok).set_text(reinterpret_cast<unsigned char const*>("Ok"));
    (*l_button_ok).set_response(RESPONSE_ACCEPT);

    button* l_button_cancel= new button(26,10,10,3);
    (*l_button_cancel).set_style(button::STYLE_BOX);
    (*l_button_cancel).set_text(reinterpret_cast<unsigned char const*>("Cancel"));
    (*l_button_cancel).set_response(RESPONSE_ACCEPT);

    (*l_dialog).add(l_label);
    (*l_dialog).add(l_button_ok);
    (*l_dialog).add(l_button_cancel);

    wm_draw(l_dialog);
    l_response= wm_run_modal(*l_dialog);

    delete l_dialog;

    return l_response;
}
