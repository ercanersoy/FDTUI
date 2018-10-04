/*
 SKIN.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __skin_h__

#include "screen.h"

struct skin_frame
{
    screen_char_t                       m_hline;
    screen_char_t                       m_vline;
    screen_char_t                       m_top_left;
    screen_char_t                       m_top_right;
    screen_char_t                       m_bottom_left;
    screen_char_t                       m_bottom_right;
    screen_char_t                       m_middle_left;
    screen_char_t                       m_middle_right;
};

struct skin_buttons
{
    screen_char_t                       m_system;
    screen_char_t                       m_close;
    screen_char_t                       m_maximize;
    screen_char_t                       m_minimize;
    screen_char_t                       m_resize;
    screen_char_t                       m_up;
    screen_char_t                       m_down;
    screen_char_t                       m_left;
    screen_char_t                       m_right;
    screen_char_t                       m_checked;
    screen_char_t                       m_submenu;
    screen_char_t                       m_thumb;
    screen_char_t                       m_expand;
    screen_char_t                       m_collapse;
};

struct skin_desktop
{
    screen_char_t                       m_fillc;
    enum foreground                     m_fcolor;
    enum background                     m_bcolor;
};

struct skin_window
{
    screen_char_t                       m_fillc;
    enum foreground                     m_fcolor;
    enum background                     m_bcolor;
    enum foreground                     m_text_fcolor;
    enum background                     m_text_bcolor;
};

struct skin_menu
{
    struct skin_window                  m_window;
    enum foreground                     m_selected_fcolor;
    enum background                     m_selected_bcolor;
    enum foreground                     m_disabled_fcolor;
    enum background                     m_disabled_bcolor;
    enum foreground                     m_hotkey_fcolor;
    enum background                     m_hotkey_bcolor;
};

struct skin_button
{
    enum foreground                     m_fcolor;
    enum background                     m_bcolor;
    enum foreground                     m_focused;
    enum background                     m_shadow;
};

struct skin_label
{
    enum foreground                     m_fcolor;
    enum background                     m_bcolor;
};

struct skin_slider
{
    screen_char_t                       m_fillc;
    enum foreground                     m_fcolor;
    enum background                     m_bcolor;
};

struct skin_listbox
{
    enum foreground                     m_fcolor;
    enum background                     m_bcolor;
    enum foreground                     m_highlight_fcolor;
    enum background                     m_highlight_bcolor;
    enum foreground                     m_selected_fcolor;
    enum background                     m_selected_bcolor;
};

struct skin_edit
{
    enum foreground                     m_fcolor;
    enum background                     m_bcolor;
};

struct skin_tree
{
    enum foreground                     m_fcolor;
    enum background                     m_bcolor;
    enum foreground                     m_selected_fcolor;
    enum background                     m_selected_bcolor;
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct skin_buttons              _skin_buttons;
extern struct skin_frame                _skin_frame_single;
extern struct skin_desktop              _skin_desktop;
extern struct skin_window               _skin_window;
extern struct skin_menu                 _skin_menu;
extern struct skin_label                _skin_label;
extern struct skin_button               _skin_button;
extern struct skin_slider               _skin_slider;
extern struct skin_listbox              _skin_listbox;
extern struct skin_edit                 _skin_edit;
extern struct skin_tree                 _skin_tree;

extern void
skin_set_defaults();

#if !defined(__CURSES__)
#define skin_set_defaults()
#endif

#ifdef __cplusplus
}
#endif

#define __skin_h__
#endif
