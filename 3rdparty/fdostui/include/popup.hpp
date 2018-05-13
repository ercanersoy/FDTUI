/*
 POPUP.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __popup_hpp__

#include "menu.hpp"

extern enum event_response
popup_menu(
    struct menuitem *                   io_menu,
    int const                           i_pos_x,
    int const                           i_pos_y);

extern enum event_response
popup_message(
    unsigned char const*                i_title,
    unsigned char const*                i_message);

#define __popup_hpp__
#endif
