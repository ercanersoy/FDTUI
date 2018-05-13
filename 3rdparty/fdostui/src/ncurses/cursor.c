/*
vim:expandtab:softtabstop=4:tabstop=4:shiftwidth=4:nowrap:ruler
*/
/*
 CURSOR.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "cursor.h"
#include <curses.h>

enum cursor_mode                        _cursor_mode= CURSOR_OVERWRITE;
unsigned int                            _cursor_features= CURSOR_CAN_HIDE;

extern void
cursor_set_position(
    unsigned char const                 i_row,
    unsigned char const                 i_column)
{

    move(i_column, i_row);
    refresh();

    return;
}

extern void
cursor_hide()
{

    curs_set(0);

    return;
}

extern void
cursor_underline()
{

    curs_set(1);

    return;
}

extern void
cursor_block()
{

    curs_set(1);

    return;
}
