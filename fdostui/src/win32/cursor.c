/*
 CURSOR.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "cursor.h"
#include <windows.h>

extern HANDLE                           _screen_active;
enum cursor_mode                        _cursor_mode= CURSOR_OVERWRITE;

extern void
cursor_set_position(
    unsigned char const                 i_row,
    unsigned char const                 i_column)
{
    COORD                               l_coord;

    l_coord.X= i_row;
    l_coord.Y= i_column;
    SetConsoleCursorPosition(_screen_active, l_coord);

    return;
}

extern void
cursor_hide()
{
    CONSOLE_CURSOR_INFO                 l_info;

    GetConsoleCursorInfo(_screen_active, &l_info);
    l_info.bVisible= 0;
    SetConsoleCursorInfo(_screen_active, &l_info);

    return;
}

extern void
cursor_underline()
{
    CONSOLE_CURSOR_INFO                 l_info;

    l_info.dwSize= 15;
    l_info.bVisible= 1;
    SetConsoleCursorInfo(_screen_active, &l_info);

    return;
}

extern void
cursor_block()
{
    CONSOLE_CURSOR_INFO                 l_info;

    l_info.dwSize= 100;
    l_info.bVisible= 1;
    SetConsoleCursorInfo(_screen_active, &l_info);

    return;
}
