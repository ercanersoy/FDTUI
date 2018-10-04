/*
 DRAW.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __draw_h__

#include "screen.h"
#include "skin.h"
#include "box.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void
draw_fill_area(
    int const                           i_col,
    int const                           i_row,
    unsigned int const                  i_columns,
    unsigned int const                  i_rows,
    screen_char_t const                 i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void
draw_putc(
    int const                           i_col,
    int const                           i_row,
    screen_char_t const                 i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void
draw_putvc(
    int const                           i_col,
    int const                           i_row,
    screen_char_t const                 i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void
draw_puts(
    int const                           i_col,
    int const                           i_row,
    unsigned char const*                i_string,
    unsigned int const                  i_length,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void
draw_frame(
    struct box const*const              i_box,
    struct skin_frame const*const       i_skin,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

#ifdef __cplusplus
}
#endif

#define __draw_h__
#endif
