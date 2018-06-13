/*
 KEYBOARD.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "keyboard.h"
#include <bios.h>

short int
keyboard_query()
{
    unsigned				l_key;

    l_key= _bios_keybrd(_NKEYBRD_READY);

    return (short int)l_key;
}

void
keyboard_read(
    unsigned char *const                o_scan,
    unsigned char *const                o_ascii)
{
    unsigned                            l_key;

    l_key= _bios_keybrd(_NKEYBRD_READ);
    *o_ascii= (0xff & l_key);
    *o_scan= (l_key >> 8);

    return;
}
