/*
 HW.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "hw.h"
#include <sys/farptr.h>
#include <dpmi.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

unsigned char                           _hw_drives[HW_DRIVE_MAX];

extern void
hw_detect_drives()
{
    int                                 l_seg;
    int                                 l_sel;
    int                                 l_flop;
    size_t                              l_index;
    __dpmi_regs                         l_regs;

    memset(_hw_drives, 0, sizeof(_hw_drives));

    do
    {
        /*
            fcb needs 40 bytes
            filespec needs 3 bytes
            allocate 3 pages (3*16=48)
        */

        l_seg= __dpmi_allocate_dos_memory(((48+15)>>4), &l_sel);

        if (-1 == l_seg)
        {
            break;
        }

        l_index= 0;

        do
        {
            /* fill filespec */
            _farsetsel(l_sel);
            _farnspokeb(0, 'A'+l_index);
            _farnspokeb(1, ':');
            _farnspokeb(2, '0');

            memset(&l_regs, 0, sizeof(l_regs));
            l_regs.x.ds= l_seg; /* filespec segment */
            l_regs.x.es= l_seg; /* fcb segment */
            l_regs.x.di= 4; /* fcb offet */

            l_regs.x.ax= 0x2900; /* parse filename */
            __dpmi_simulate_real_mode_interrupt(0x21, &l_regs);

            if (0xff == (0x0ff & l_regs.x.ax))
            {
                _hw_drives[l_index]= 0;
            }
            else
            {
                _hw_drives[l_index]= 1;
            }

            l_index++;

            if (26 <= l_index)
            {
                break;
            }

        }while(1);

        __dpmi_free_dos_memory(l_sel);

        /* get bios equipment word */
        memset(&l_regs, 0, sizeof(l_regs));
        __dpmi_int(0x11, &l_regs);

        /*
        check BIOS equipment word to see how many floppies there are
        bits 6 & 7 number of floppies less 1
        */

        l_flop= l_regs.x.ax;
        l_flop >>= 5;
        l_flop &= 0x3;
        l_flop--;

        if (0 == l_flop)
        {
            _hw_drives[0]= 0; /* no a drive */
            _hw_drives[1]= 0; /* no b drive */
        }
        else if (1 == l_flop)
        {
            _hw_drives[1]= 0; /* no b drive */
        }

    }while(0);

    return;
}
