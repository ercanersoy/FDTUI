/*
  VIDEO.C
 
  Adapted from the work of Jerome Shidel INFO.PAS.
 
  Copyright 1990-2015, Jerome Shidel.
    
  This project and related files are subject to the terms of the Mozilla 
  Public License,  v. 2.0. If a copy of the MPL was not distributed with 
  this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "video.h"
#include <sys/farptr.h>
#include <dpmi.h>
#include <pc.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static int
video_detect_vesa()
{
    int                                 l_exit;
    int                                 l_seg;
    int                                 l_sel;
    __dpmi_regs                         l_regs;

    l_exit= -1;

    do
    {

        l_seg= __dpmi_allocate_dos_memory(((512+15)>>4), &l_sel);

        if (-1 == l_seg)
        {
            break;
        }

        /* Get SuperVGA information */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x4f00;
        l_regs.x.es= l_seg;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);
        __dpmi_free_dos_memory(l_sel);

        if (0x4f != l_regs.x.ax)
        {
            break;
        }

        l_exit= 0;

    }while(0);

    return l_exit;
}

extern enum video_card
video_detect()
{
    enum video_card                     l_card;
    int                                 l_rc;
    __dpmi_regs                         l_regs;

    l_card= VIDEO_CARD_NONE;

    do
    {

        l_rc= video_detect_vesa();

        if (0 == l_rc)
        {
            l_card= VIDEO_CARD_VESA;
            break;
        }

        /*
            Video refresh control (VGA)
            inputs:
            al 00h enable refresh
            al 01h disable refresh
            returns al = 12h if function supported
        */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x1200;
        l_regs.x.bx= 0x36;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

        if (0x12 == (0xff & l_regs.x.ax))
        {
            l_card= VIDEO_CARD_VGA;
            break;
        }

        /*
            video addressing (VGA,MCGA)
            inputs:
            al 00h enable video addressing
            al 01h disable video addressing
            returns al=12h if function supported
        */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x1200;
        l_regs.x.bx= 0x32;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

        if (0x12 == (0xff & l_regs.x.ax))
        {
            l_card= VIDEO_CARD_MCGA;
            break;
        }
  
        /*
            get EGA info
            returns:
            bh video state
                00h color mode in effect
                01h mono mode in effect
            bl installed memory 
                00h 64K
                01h 128K
                02h 192K
                03h 256K
            ch feature connector bits
            cl switch settings
        */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x1200;
        l_regs.x.bx= 0x10;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

        if (0x10 == (0xff & l_regs.x.bx))
        {
            l_card= VIDEO_CARD_EGA;
            break;
        }

        /*
            Get Video State
            input
                ah= 0x0f
            returns
                ah = number of screen colors
                al = current video mode
                bh = current display page
        */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x0f00;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

        if (0x07 == (0xff & l_regs.x.ax))
        {
            l_card= VIDEO_CARD_MDA;
            break;
        }

        l_card= VIDEO_CARD_CGA;

    }while(0);

    return l_card;
}
