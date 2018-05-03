/*
 MENUITEM.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __menuitem_h__

#include "array.h"
#include "keyboard.h"

#define MENUITEM_MNEMONIC_NONE          (unsigned int)-1

struct menuitem;

typedef void (*menuitem_signal_t)(
    struct menuitem*                    i_menu,
    void*                               i_user_data);

enum menuitem_flags
{
    MENUITEM_NONE=                      (0<<0),
    MENUITEM_VALUE=                     (1<<1),
    MENUITEM_DISABLED=                  (1<<2),
    MENUITEM_CHECKBOX=                  (1<<3),
    MENUITEM_SEPERATOR=                 (1<<4),
    MENUITEM_SUBMENU=                   (1<<5)
};

struct menuitem
{
    unsigned char const*                m_label;
    unsigned int                        m_mnemonic_offset;
    unsigned char const*                m_accel_label;
    enum scancodes                      m_accel_scancode;
    unsigned int                        m_flags;
    menuitem_signal_t                   m_signal;
    void*                               m_user_data;
};

struct menuitembar
{
    unsigned char const*                m_label;
    unsigned int                        m_mnemonic_offset;
    enum scancodes                      m_accel_scancode;
    struct menuitem*                    m_items;
};

#ifdef __cplusplus
extern "C" {
#endif

/* _mjo i_menu should be const*/
extern void
menuitem_count(
    unsigned int*const                  o_rows,
    unsigned int*const                  o_cols,
    struct menuitem *                   i_menu);

extern void
menuitem_emit(
    struct menuitem *                   i_menu);

extern struct menuitem*
menuitem_next(
    struct menuitem*                    i_menu);

extern struct menuitem*
menuitem_nth(
    struct menuitem*                    i_menu,
    unsigned int const                  i_nth);

extern void
menuitem_flatten(
    struct array*const                  o_array,
    struct menuitem*                    i_menu);

/* _mjo i_menu  should be const*/
extern struct menuitem*
menuitem_find_scancode(
    struct menuitem *                   i_menu,
    enum scancodes const                i_scan);

#define menuitem_emit(i_menu) \
    if ( (i_menu)->m_signal ) \
        (*(i_menu)->m_signal)((i_menu), (i_menu)->m_user_data);

#ifdef __cplusplus
}
#endif

#define __menuitem_h__
#endif
