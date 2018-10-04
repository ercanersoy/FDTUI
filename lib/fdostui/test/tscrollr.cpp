/*
 TSCROLLR.CPP

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

    wm_error error= wm_init();

    if (error)
    {
        printf("Unable to initialze FDOSTUI subsystem\n");
        return error;
    }

    window* win= new window(0, 0, _video_cols, _video_rows);
    wm_register_window(win);

    scroller* scrl= new scroller(0,0,20,10);
    (*win).add(scrl);

    int i= 0;
    for(int row=0; row < 30; row++)
    {
        for(int col=0; col < 78; col+=13)
        {
            button *btn= new button(col, row, 13, 1);
            unsigned char buf[30];
            sprintf(reinterpret_cast<char*>(buf), "button %03d", i++);
            (*btn).set_text(buf); 
            (*scrl).add(btn);
        }
    }

    wm_draw(win);
    wm_run();
    wm_deinit();

    return 0;
}

/*
vim:expandtab:softtabstop=4:tabstop=4:shiftwidth=4:nowrap:ruler
*/
