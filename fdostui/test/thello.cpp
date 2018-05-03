/*
 THELLO.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "fdostui.hpp"
#include <stdio.h>

// button callback. signal the window manager to exit.
void
button_clicked_cb(button const*, void*)
{
    wm_quit(ERROR_USER_ESC);
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
    window* win= new window(30, 9, 20, 8);

    // register it with the window manager
    wm_register_window(win);

    // create a button widget and add it to the window
    button* btn= new button(1, 1, 16, 4);
    btn->set_text(reinterpret_cast<unsigned char const*>("Hello World!"));
    btn->set_style(button::STYLE_BOX|button::STYLE_SHADOW);
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
