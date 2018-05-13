/*
 TMENU.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "fdostui.hpp"
#include <stdio.h>

struct menuitem                         _file_menu[]=
{
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Open"),
    /*.m_mnemonic_offset=*/0,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("New"),
    /*.m_mnemonic_offset=*/0,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Close"),
    /*.m_mnemonic_offset=*/0,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/MENUITEM_SEPERATOR,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Exit"),
    /*.m_mnemonic_offset=*/1,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {0}
};

struct menuitem                         _sub_menu[]=
{
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Level 1"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/MENUITEM_SUBMENU,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
        {
        /*.m_label=*/reinterpret_cast<unsigned char const*>("Level 2"),
        /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
        /*.m_accel_label=*/0,
        /*.m_accel_scancode=*/SCAN_NONE,
        /*.m_flags=*/MENUITEM_SUBMENU,
        /*.m_callback=*/0,
        /*.m_user_data=*/0
        },
            {
            /*.m_label=*/reinterpret_cast<unsigned char const*>("Level 3"),
            /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
            /*.m_accel_label=*/0,
            /*.m_accel_scancode=*/SCAN_NONE,
            /*.m_flags=*/MENUITEM_SUBMENU,
            /*.m_callback=*/0,
            /*.m_user_data=*/0
            },
                {
                /*.m_label=*/reinterpret_cast<unsigned char const*>("Level 4"),
                /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
                /*.m_accel_label=*/0,
                /*.m_accel_scancode=*/SCAN_NONE,
                /*.m_flags=*/MENUITEM_SUBMENU,
                /*.m_callback=*/0,
                /*.m_user_data=*/0
                },
                    {
                    /*.m_label=*/reinterpret_cast<unsigned char const*>("Level 5"),
                    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
                    /*.m_accel_label=*/0,
                    /*.m_accel_scancode=*/SCAN_NONE,
                    /*.m_flags=*/MENUITEM_SUBMENU,
                    /*.m_callback=*/0,
                    /*.m_user_data=*/0
                    },
                        {
                        /*.m_label=*/reinterpret_cast<unsigned char const*>("Level 6"),
                        /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
                        /*.m_accel_label=*/0,
                        /*.m_accel_scancode=*/SCAN_NONE,
                        /*.m_flags=*/MENUITEM_SUBMENU,
                        /*.m_callback=*/0,
                        /*.m_user_data=*/0
                        },
                            {
                            /*.m_label=*/reinterpret_cast<unsigned char const*>("Level 7"),
                            /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
                            /*.m_accel_label=*/0,
                            /*.m_accel_scancode=*/SCAN_NONE,
                            /*.m_flags=*/MENUITEM_SUBMENU,
                            /*.m_callback=*/0,
                            /*.m_user_data=*/0
                            },
                                {
                                /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 1"),
                                /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
                                /*.m_accel_label=*/0,
                                /*.m_accel_scancode=*/SCAN_NONE,
                                /*.m_flags=*/0,
                                /*.m_callback=*/0,
                                /*.m_user_data=*/0
                                },
                                {
                                /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 2"),
                                /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
                                /*.m_accel_label=*/0,
                                /*.m_accel_scancode=*/SCAN_NONE,
                                /*.m_flags=*/0,
                                /*.m_callback=*/0,
                                /*.m_user_data=*/0
                                },
                                {
                                /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 3"),
                                /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
                                /*.m_accel_label=*/0,
                                /*.m_accel_scancode=*/SCAN_NONE,
                                /*.m_flags=*/0,
                                /*.m_callback=*/0,
                                /*.m_user_data=*/0
                                },

                                {0},
                            {0},
                        {0},
                    {0},
                {0},
            {0},
        {0},
    {0}
};

struct menuitem                         _large_menu[]=
{
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 1"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 2"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 3"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 4"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 5"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 6"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 7"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 8"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 9"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 10"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 11"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 12"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 13"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 14"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 15"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 16"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 17"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 18"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 19"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 20"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 21"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 22"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 23"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 24"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Item 25"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {0}
};

struct menuitem                         _feature_menu[]=
{
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Checkbox"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/MENUITEM_CHECKBOX,
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Checkbox On"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/(MENUITEM_CHECKBOX|MENUITEM_VALUE|MENUITEM_DISABLED),
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Disabled"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/(MENUITEM_SUBMENU|MENUITEM_DISABLED),
    /*.m_callback=*/0,
    /*.m_user_data=*/0
    },
        {
        /*.m_label=*/reinterpret_cast<unsigned char const*>("no access"),
        /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
        /*.m_accel_label=*/0,
        /*.m_accel_scancode=*/SCAN_NONE,
        /*.m_flags=*/0,
        /*.m_callback=*/0,
        /*.m_user_data=*/0
        },
        {0},
    {0}
};

struct menuitembar                      _menubar[]=
{
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("File"),
    /*.m_mnemonic_offset=*/0,
    /*.m_accel_scancode=*/SCAN_ALT_F,
    /*.m_items=*/_file_menu
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Sub"),
    /*.m_mnemonic_offset=*/0,
    /*.m_accel_scancode=*/SCAN_ALT_S,
    /*.m_items=*/_sub_menu
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Large"),
    /*.m_mnemonic_offset=*/0,
    /*.m_accel_scancode=*/SCAN_ALT_L,
    /*.m_items=*/_large_menu
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Empty"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_items=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Features"),
    /*.m_mnemonic_offset=*/MENUITEM_MNEMONIC_NONE,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_items=*/_feature_menu
    },
    {0}
};

int
main(void)
{

    // initialize the FDOSTUI subsytem (mouse and display)
    wm_error error= wm_init();

    // if an error occurred exit
    if (error)
    {
        printf("Unable to initialze FDOSTUI subsystem\n");
        return error;
    }

    // create our main window
    window* win= new window(0, 0, 20, 18);

    // register it with the window manager
    wm_register_window(win);

    menubar* mbar= new menubar(0,0,_video_cols,1);
    (*mbar).set_menu(_menubar);
    (*win).add(mbar);

    checkbutton* btn= new checkbutton(1,2,10,1);
    (*btn).set_text(reinterpret_cast<unsigned char const*>("Button1"));
    (*win).add(btn);

    btn= new checkbutton(1,3,10,1);
    (*btn).set_text(reinterpret_cast<unsigned char const*>("Button2"));
    (*win).add(btn);

    // draw the window
    wm_draw(win);

    // enter the window manager loop
    wm_run();

    wm_deinit();

    return 0;
}
/*
vim:expandtab:softtabstop=4:tabstop=4:shiftwidth=4:nowrap:ruler
*/
