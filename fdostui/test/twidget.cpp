/*
 TWIDGET.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "fdostui.hpp"
#include <stdio.h>

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
    window* win= new window(0, 0, _video_cols, _video_rows);

    // register it with the window manager
    wm_register_window(win);

    entry* en= new entry(1,1,16,1);
    (*en).set_text(reinterpret_cast<unsigned char const*>("Entry"));
    win->add(en);

    label* lbl= new label(1,2,16,1);
    (*lbl).set_text(reinterpret_cast<unsigned char const*>("Radio Group"));
    win->add(lbl);

    radiobutton* rbtn1= new radiobutton(1,3,16,1);
    (*rbtn1).set_text(reinterpret_cast<unsigned char const*>("radio 1"));
    win->add(rbtn1);

    radiobutton* rbtn2= new radiobutton(1,4,16,1);
    (*rbtn2).set_text(reinterpret_cast<unsigned char const*>("radio 2"));
    win->add(rbtn2);

    radiobutton* rbtn3= new radiobutton(1,5,16,1);
    (*rbtn3).set_text(reinterpret_cast<unsigned char const*>("radio 3"));
    win->add(rbtn3);

    (*rbtn1).join(rbtn2);
    (*rbtn1).join(rbtn3);

    checkbutton* cbtn= new checkbutton(1,6,16,1);
    (*cbtn).set_text(reinterpret_cast<unsigned char const*>("Checkbox"));
    win->add(cbtn);

    spinner* spin= new spinner(1,8,16,1);
    win->add(spin);

    slider* sldr= new slider(1,10,16,1);
    win->add(sldr);

    button* btn= new button(1,11,16,1);
    (*btn).set_text(reinterpret_cast<unsigned char const*>("Flat Button"));
    win->add(btn);

    listbox* lb= new listbox(1, 12, 16, 8);
    lb->set_label(reinterpret_cast<unsigned char const*>("Listbox"));

    lb->add(reinterpret_cast<unsigned char const*>("1"));
    lb->add(reinterpret_cast<unsigned char const*>("2"));
    lb->add(reinterpret_cast<unsigned char const*>("3"));
    lb->add(reinterpret_cast<unsigned char const*>("4"));
    lb->add(reinterpret_cast<unsigned char const*>("5"));
    lb->add(reinterpret_cast<unsigned char const*>("6"));
    lb->add(reinterpret_cast<unsigned char const*>("7"));
    lb->add(reinterpret_cast<unsigned char const*>("8"));
    lb->add(reinterpret_cast<unsigned char const*>("9"));
    lb->add(reinterpret_cast<unsigned char const*>("10"));
    lb->add(reinterpret_cast<unsigned char const*>("11"));
    lb->add(reinterpret_cast<unsigned char const*>("12"));
    lb->add(reinterpret_cast<unsigned char const*>("13"));
    lb->add(reinterpret_cast<unsigned char const*>("14"));
    lb->add(reinterpret_cast<unsigned char const*>("15"));
    lb->add(reinterpret_cast<unsigned char const*>("16"));
    win->add(lb);

    edit* ed= new edit(25,1,40,15);
    win->add(ed);

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
