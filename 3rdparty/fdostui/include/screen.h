/*
 SCREEN.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#ifndef __screen_h__

#if (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__)
typedef unsigned short int screen_block_t;
typedef unsigned char screen_char_t;
#endif

#if defined(__NT__)
#    ifdef _WIN32_WINNT
#    undef _WIN32_WINNT
#    endif
#define _WIN32_WINNT 0x0500
#include <windows.h>
typedef CHAR_INFO screen_block_t;
typedef unsigned char screen_char_t;
#endif

#ifdef __CURSES__
#include <curses.h>
typedef chtype screen_block_t;
typedef chtype screen_char_t;
#endif

#ifndef __FAR
#   if defined(__FLAT__ ) || defined(__LARGE__)
#       define __FAR
#   else
#       define __FAR __far
#   endif
#endif

#if (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__)

enum foreground
{
    fcolor_black                        = 0,
    fcolor_blue                         = 1,
    fcolor_green                        = 2,
    fcolor_cyan                         = 3,
    fcolor_red                          = 4,
    fcolor_magenta                      = 5,
    /* fcolor_brown                     = 6, */
    /* fcolor_light_gray                = 7, */
    /* fcolor_gray                      = 8, */
    /* fcolor_light_blue                = 9, */
    /* fcolor_light_green               = 10, */
    /* fcolor_light_cyan                = 11, */
    /* fcolor_light_red                 = 12, */
    /* fcolor_light_magenta             = 13, */
    fcolor_yellow                       = 14,
    fcolor_white                        = 15
};

#endif /* (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__) */

#if defined(__NT__) && !defined(__CURSES__)

#define FCOLOR_BLACK                    (0)
#define FCOLOR_RED                      (FOREGROUND_RED)
#define FCOLOR_GREEN                    (FOREGROUND_GREEN)
#define FCOLOR_BLUE                     (FOREGROUND_BLUE)
#define FCOLOR_YELLOW                   (FOREGROUND_RED   | FOREGROUND_GREEN)
#define FCOLOR_MAGENTA                  (FOREGROUND_RED   | FOREGROUND_BLUE)
#define FCOLOR_WHITE                    (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FCOLOR_CYAN                     (FOREGROUND_GREEN | FOREGROUND_BLUE)

#define FCOLOR_LIGHT_BLACK              (FCOLOR_BLACK   | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_RED                (FCOLOR_RED     | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_GREEN              (FCOLOR_GREEN   | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_BLUE               (FCOLOR_BLUE    | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_YELLOW             (FCOLOR_YELLOW  | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_MAGENTA            (FCOLOR_MAGENTA | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_WHITE              (FCOLOR_WHITE   | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_CYAN               (FCOLOR_CYAN    | FOREGROUND_INTENSITY)

enum foreground
{
    fcolor_black                        = FCOLOR_BLACK,
    fcolor_blue                         = FCOLOR_BLUE,
    fcolor_green                        = FCOLOR_GREEN,
    fcolor_cyan                         = FCOLOR_CYAN,
    fcolor_red                          = FCOLOR_RED,
    fcolor_magenta                      = FCOLOR_MAGENTA,
    fcolor_yellow                       = FCOLOR_YELLOW,
    fcolor_white                        = FCOLOR_WHITE
};

#endif /* #if defined(__NT__) && !defined(__CURSES__) */

#if defined(__CURSES__)

enum foreground
{
    fcolor_black                        = COLOR_BLACK,
    fcolor_blue                         = COLOR_BLUE,
    fcolor_green                        = COLOR_GREEN,
    fcolor_cyan                         = COLOR_CYAN,
    fcolor_red                          = COLOR_RED,
    fcolor_magenta                      = COLOR_MAGENTA,
    fcolor_yellow                       = COLOR_YELLOW,
    fcolor_white                        = COLOR_WHITE
};

#endif /* #if defined(__CURSES__) */

#if (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__)

enum background
{
    bcolor_black                        = 0,
    bcolor_blue                         = 1,
    bcolor_green                        = 2,
    bcolor_cyan                         = 3,
    bcolor_red                          = 4,
    bcolor_magenta                      = 5,
    /* bcolor_brown                     = 6, */
    bcolor_light_gray                   = 7
};

#endif /* (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__) */

#if defined(__NT__) && !defined(__CURSES__)

#define BCOLOR_BLACK                    (0)
#define BCOLOR_RED                      (BACKGROUND_RED)
#define BCOLOR_GREEN                    (BACKGROUND_GREEN)
#define BCOLOR_BLUE                     (BACKGROUND_BLUE)
#define BCOLOR_YELLOW                   (BACKGROUND_RED   | BACKGROUND_GREEN)
#define BCOLOR_MAGENTA                  (BACKGROUND_RED   | BACKGROUND_BLUE)
#define BCOLOR_WHITE                    (BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BCOLOR_CYAN                     (BACKGROUND_GREEN | BACKGROUND_BLUE)

#define BCOLOR_LIGHT_BLACK              (BCOLOR_BLACK   | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_RED                (BCOLOR_RED     | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_GREEN              (BCOLOR_GREEN   | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_BLUE               (BCOLOR_BLUE    | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_YELLOW             (BCOLOR_YELLOW  | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_MAGENTA            (BCOLOR_MAGENTA | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_WHITE              (BCOLOR_WHITE   | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_CYAN               (BCOLOR_CYAN    | BACKGROUND_INTENSITY)

enum background
{
    bcolor_black                        = BCOLOR_BLACK,
    bcolor_blue                         = BCOLOR_BLUE,
    bcolor_green                        = BCOLOR_GREEN,
    bcolor_cyan                         = BCOLOR_CYAN,
    bcolor_red                          = BCOLOR_RED,
    bcolor_magenta                      = BCOLOR_MAGENTA,
    bcolor_light_gray                   = BCOLOR_WHITE
};

#endif /* #if defined(__NT__) && !defined(__CURSES__) */

#if defined(__CURSES__)

enum background
{
    bcolor_black                        = COLOR_BLACK,
    bcolor_blue                         = COLOR_BLUE,
    bcolor_green                        = COLOR_GREEN,
    bcolor_cyan                         = COLOR_CYAN,
    bcolor_red                          = COLOR_RED,
    bcolor_magenta                      = COLOR_MAGENTA,
    bcolor_light_gray                   = COLOR_WHITE
};

#endif /* #if defined(__CURSES__) */

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int                     _video_cols;
extern unsigned int                     _video_rows;

#if defined(__CURSES__)
extern int                              _video_has_color;
#endif

extern int screen_init();

extern void screen_deinit();

extern int
screen_set25lines();

extern int
screen_set50lines();

#if defined(__CURSES__)
#define screen_set25lines() -1
#define screen_set50lines() -1
#endif

#if defined(__NT__) || defined(__CURSES__)
extern void
screen_changed_size();
#endif

extern void 
screen_fill(
    screen_char_t const                 i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void 
screen_fill_area(
    int const                           i_col,
    int const                           i_row,
    unsigned int const                  i_columns,
    unsigned int const                  i_rows,
    screen_char_t const                 i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void 
screen_putc(
    int const                           i_col,
    int const                           i_row,
    screen_char_t const                 i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void 
screen_putvc(
    int const                           i_col,
    int const                           i_row,
    screen_char_t const                 i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void 
screen_puts(
    int const                           i_col,
    int const                           i_row,
    unsigned char const*                i_string,
    unsigned int const                  i_length,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern void 
screen_set_color(
    int const                           i_col,
    int const                           i_row,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor);

extern screen_block_t
screen_getc(
    int const                           i_col,
    int const                           i_row);

extern screen_block_t __FAR*
screen_read(
    screen_block_t __FAR*               o_block,
    unsigned int const                  i_columns,
    int const                           i_col,
    int const                           i_row);

extern screen_block_t const __FAR*
screen_write(
    int const                           i_col,
    int const                           i_row,
    screen_block_t const __FAR*         i_block,
    unsigned int const                  i_columns);

extern void
screen_push(
    int const                           i_copy);

extern void
screen_pop();

#if defined(__FLAT__) && !defined(__NT__)
#define screen_push(i_copy)
#define screen_pop()
#endif

#ifdef __cplusplus
}
#endif

#define __screen_h__
#endif
