/*
 CURSOR.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "cursor.h"
#include <dpmi.h>
#include <stdlib.h>
#include <string.h>

enum cursor_mode                        _cursor_mode= CURSOR_OVERWRITE;

extern void
cursor_set_position(
    unsigned char const                 i_row,
    unsigned char const                 i_column)
{
    __dpmi_regs                         l_regs;

    memset(&l_regs, 0, sizeof(l_regs));
    l_regs.x.ax= 0x0200;
    l_regs.x.dx= (i_column << 8);
    l_regs.x.dx|= i_row;
    __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

    return;
}

extern void
cursor_hide()
{
    __dpmi_regs                         l_regs;

    memset(&l_regs, 0, sizeof(l_regs));
    l_regs.x.ax= 0x0100;
    l_regs.x.cx= 0x2000;
    __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

    return;
}

extern void
cursor_underline()
{
    __dpmi_regs                         l_regs;

    memset(&l_regs, 0, sizeof(l_regs));
    l_regs.x.ax= 0x0100;
    l_regs.x.cx= 0x0607;
    __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

    return;
}

extern void
cursor_block()
{
    __dpmi_regs                         l_regs;

    memset(&l_regs, 0, sizeof(l_regs));
    l_regs.x.ax= 0x0100;
    l_regs.x.cx= 0x0007;
    __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

    return;
}
