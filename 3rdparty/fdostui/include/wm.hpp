/*
 WM.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __wm_hpp__

#include "window.hpp"

enum wm_error
{
    ERROR_NONE,
    ERROR_INIT_SCREEN,
    ERROR_INIT_MOUSE,
    ERROR_EMPTY_QUEUE,
    ERROR_USER_ESC
};

extern enum wm_error
wm_init();

extern void
wm_deinit();

extern void
wm_register_window(
    window*const                        i_window);

extern enum wm_error
wm_run();

extern enum event_response
wm_run_modal(
    widget&                             i_widget);

extern void
wm_quit(
    enum wm_error const                 i_reason);

extern void
wm_draw(
    window *const                       i_window);

extern void
wm_draw_widget(
    widget *const                       i_widget);

extern window*const
wm_grab(
    window*const                        i_window);

extern void
wm_unregister_window(
    window*const                        i_window);

#define __wm_hpp__
#endif
