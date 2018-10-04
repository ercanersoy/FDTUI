/*
 WMMENU.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.

*/
#include "wmmenu.hpp"
#include "wmhelper.hpp"

extern wmhelper                         _wm_helper;

static void
wmmenu_close_cb(
    struct menuitem*                    /*i_menu*/,
    void*                               /*i_user_data*/)
{
    _wm_helper.close();
}

static void
wmmenu_maximize_cb(
    struct menuitem*                    /*i_menu*/,
    void*                               /*i_user_data*/)
{
    _wm_helper.maximize();
}

static void
wmmenu_minimize_cb(
    struct menuitem*                    /*i_menu*/,
    void*                               /*i_user_data*/)
{
    _wm_helper.minimize();
}

static void
wmmenu_resize_cb(
    struct menuitem*                    /*i_menu*/,
    void*                               /*i_user_data*/)
{
    _wm_helper.capture_resize();
}

static void
wmmenu_move_cb(
    struct menuitem*                    /*i_menu*/,
    void*                               /*i_user_data*/)
{
    _wm_helper.capture_move();
}

struct menuitem                         _system_menu[]=
{
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Move"),
    /*.m_mnemonic_offset=*/0,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/wmmenu_move_cb,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Resize"),
    /*.m_mnemonic_offset=*/4,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/wmmenu_resize_cb,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Maximize"),
    /*.m_mnemonic_offset=*/2,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/wmmenu_maximize_cb,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Iconify"),
    /*.m_mnemonic_offset=*/3,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/MENUITEM_SEPERATOR,
    /*.m_callback=*/wmmenu_minimize_cb,
    /*.m_user_data=*/0
    },
    {
    /*.m_label=*/reinterpret_cast<unsigned char const*>("Close"),
    /*.m_mnemonic_offset=*/0,
    /*.m_accel_label=*/0,
    /*.m_accel_scancode=*/SCAN_NONE,
    /*.m_flags=*/0,
    /*.m_callback=*/wmmenu_close_cb,
    /*.m_user_data=*/0
    },
    {0},
};

menuitem*                               _system_menu_hook= _system_menu;

