/*
 HW.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "hw.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

unsigned char                           _hw_drives[HW_DRIVE_MAX]; /* drives 'A'-'Z' 0=not available 1=available */

extern void
hw_detect_drives()
{
    DWORD                               l_drives;
    unsigned int                        l_index;
    DWORD                               l_mask;

    l_drives= GetLogicalDrives();
    l_mask= 1;

    for (l_index= 0; HW_DRIVE_MAX > l_index; l_index++)
    {
        _hw_drives[l_index]= (l_drives & l_mask);
        l_mask <<= 1;
    }

    return;
}
