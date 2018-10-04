/*
 TFILE.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "fdostui.hpp"
#include <stdio.h>

// button callback
void
button_clicked_cb(button const*, void* data)
{
    unsigned char* path;

    path= file_dialog_open();

    if (path)
    {
        entry* en= reinterpret_cast<entry*>(data);
        en->set_text(path);
        free(path);
        wm_draw_widget(en);
    }

    return;
}

int
main(void)
{

    // initialize the FDOSTUI subsytem (mouse and display)
    wm_error error= wm_init();

    // if an error occurred exit
    if (error)
    {
        printf("Unable to initialze FDOSTUI subsystem\n");
        return error;
    }

    // create our main window
    window* win= new window(23, 9, 37, 5);

    // register it with the window manager
    wm_register_window(win);

    label* lbl= new label(1, 1, 5, 1);
    lbl->set_text(reinterpret_cast<unsigned char const*>("Open:"));
    win->add(lbl);

    entry* en= new entry(7,1,16,1);
    win->add(en);

    button* btn= new button(25, 0, 8, 3);
    btn->set_text(reinterpret_cast<unsigned char const*>("Browse"));
    btn->set_style(button::STYLE_BOX);
    btn->set_user_data(en);
    btn->set_signal_clicked(button_clicked_cb);
    win->add(btn);

    // draw the window
    wm_draw(win);

    // enter the window manager loop
    wm_run();

    // restore the system to it's default settings
    wm_deinit();

    return 0;
}

/*
vim:expandtab:softtabstop=4:tabstop=4:shiftwidth=4:nowrap:ruler
*/
