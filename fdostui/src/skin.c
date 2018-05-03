/*
 SKIN.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "skin.h"

struct skin_frame                       _skin_frame_single=
{
    /* .m_hline=           */ 0xc4,
    /* .m_vline=           */ 0xb3,
    /* .m_top_left=        */ 0xda,
    /* .m_top_right=       */ 0xbf,
    /* .m_bottom_left=     */ 0xc0,
    /* .m_bottom_right=    */ 0xd9,
    /* .m_middle_left=     */ 0xc3,
    /* .m_middle_right=    */ 0xb4
};

struct skin_buttons                     _skin_buttons=
{
#if defined(__DOS__)
    /* .m_system=          */ 0xf0,
    /* .m_close=           */ 'x',
    /* .m_maximize=        */ 0x18,
    /* .m_minimize=        */ 0x19,
    /* .m_resize=          */ 0xfe,
    /* .m_up=              */ 0x1e,
    /* .m_down=            */ 0x1f,
    /* .m_left=            */ 0x11,
    /* .m_right=           */ 0x10,
    /* .m_checked=         */ 0xfb,
    /* .m_submenu=         */ 0x10,
    /* .m_thumb=           */ 0xb1,
    /* .m_expand=          */ '+',
    /* .m_collapse=        */ '-'
#else
    /* .m_system=          */ 0xf0,
    /* .m_close=           */ 'x',
    /* .m_maximize=        */ '^',
    /* .m_minimize=        */ 'v',
    /* .m_resize=          */ 0xfe,
    /* .m_up=              */ '^',
    /* .m_down=            */ 'v',
    /* .m_left=            */ '<',
    /* .m_right=           */ '>',
    /* .m_checked=         */ 0xfb,
    /* .m_submenu=         */ '>',
    /* .m_thumb=           */ 0xb1,
    /* .m_expand=          */ '+',
    /* .m_collapse=        */ '-'
#endif
};

struct skin_desktop                     _skin_desktop=
{
    /* .m_fillc=           */ 0,
    /* .m_fcolor=          */ fcolor_white,
    /* .m_bcolor=          */ bcolor_black
};

struct skin_window                      _skin_window=
{
    /* .m_fillc=           */ 0,
    /* .m_fcolor=          */ fcolor_white,
    /* .m_bcolor=          */ bcolor_black,
    /* .m_text_fcolor=     */ fcolor_white,
    /* .m_text_bcolor=     */ bcolor_black
};

struct skin_menu                        _skin_menu=
{
    /* .m_window= */
    {
        /* .m_fcolor=      */ fcolor_yellow,
        /* .m_bcolor=      */ bcolor_blue,
        /* .m_text_fcolor= */ fcolor_white,
        /* .m_text_bcolor= */ bcolor_blue
    },
    /* .m_selected_fcolor= */ fcolor_white,
    /* .m_selected_bcolor= */ bcolor_black,
    /* .m_disabled_fcolor= */ fcolor_red,
    /* .m_disabled_bcolor= */ bcolor_blue,
    /* .m_hotkey_fcolor=   */ fcolor_yellow,
    /* .m_hotkey_bcolor=   */ bcolor_blue
};

struct skin_button                      _skin_button=
{
    /* .m_fcolor=          */ fcolor_white,
    /* .m_bcolor=          */ bcolor_black,
    /* .m_focused=         */ fcolor_blue,
    /* .m_shadow=          */ bcolor_green
};

struct skin_label                       _skin_label=
{
    /* .m_fcolor=           */ fcolor_white,
    /* .m_bcolor=           */ bcolor_black
};

struct skin_slider                      _skin_slider=
{
    /* .m_fillc=           */ 0,
    /* .m_fcolor=          */ fcolor_black,
    /* .m_bcolor=          */ bcolor_light_gray
};

struct skin_listbox                     _skin_listbox=
{
    /* .m_fcolor=          */ fcolor_white,
    /* .m_bcolor=          */ bcolor_black,
    /* .m_highlight_fcolor=*/ fcolor_black,
    /* .m_highlight_bcolor=*/ bcolor_light_gray,
    /* .m_selected_fcolor= */ fcolor_white,
    /* .m_selected_bcolor= */ bcolor_blue
};

struct skin_edit                        _skin_edit=
{
    /* .m_fcolor=          */ fcolor_white,
    /* .m_bcolor=          */ bcolor_black
};

struct skin_tree                        _skin_tree=
{
    /* .m_fcolor=          */ fcolor_white,
    /* .m_bcolor=          */ bcolor_black,
    /* .m_selected_fcolor= */ fcolor_white,
    /* .m_selected_bcolor= */ bcolor_blue
};

#ifdef __CURSES__
#include <curses.h>

extern void
skin_set_defaults()
{

    _skin_frame_single.m_hline= ACS_HLINE;
    _skin_frame_single.m_vline= ACS_VLINE;
    _skin_frame_single.m_top_left= ACS_ULCORNER;
    _skin_frame_single.m_top_right= ACS_URCORNER;
    _skin_frame_single.m_bottom_left= ACS_LLCORNER;
    _skin_frame_single.m_bottom_right= ACS_LRCORNER;
    _skin_frame_single.m_middle_left= ACS_LTEE;
    _skin_frame_single.m_middle_right= ACS_RTEE;

    _skin_buttons.m_system= '=';
    _skin_buttons.m_close= 'x';
/*
    _skin_buttons.m_maximize= ACS_UARROW;
    _skin_buttons.m_minimize= ACS_DARROW;
*/
    _skin_buttons.m_resize= ACS_BULLET;
/*
    _skin_buttons.m_up= ACS_UARROW;
    _skin_buttons.m_down= ACS_DARROW;
    _skin_buttons.m_left= ACS_LARROW;
    _skin_buttons.m_right= ACS_RARROW;
*/
    _skin_buttons.m_checked= 'X';
/*
    _skin_buttons.m_submenu= ACS_RARROW;
*/
    _skin_buttons.m_thumb= ACS_BLOCK;

    return;
}
#endif
