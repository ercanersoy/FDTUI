/*
 MENUITEM.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "menuitem.h"
#include "screen.h"
#include <string.h>

extern void
menuitem_count(
    unsigned int*const                  o_rows,
    unsigned int*const                  o_cols,
    struct menuitem *                   i_menu)
{
    unsigned int                        l_needed;

    *o_rows= 0;
    *o_cols= 0;

    do
    {

        if (0 == (*i_menu).m_label)
        {
            break;
        }

        l_needed= strlen((char const*)(*i_menu).m_label);

        if (MENUITEM_CHECKBOX & (*i_menu).m_flags)
        {
            l_needed+= 4;
        }

        if (MENUITEM_SUBMENU & (*i_menu).m_flags)
        {
            l_needed++;
            l_needed++;
        }
        else if ((*i_menu).m_accel_label)
        {
            l_needed+= 2+strlen((char const*)(*i_menu).m_accel_label);
        }

        if (*o_cols < l_needed)
        {
            *o_cols= l_needed;
        }

        if (MENUITEM_SEPERATOR & (*i_menu).m_flags)
        {
            (*o_rows)++;
        }

        (*o_rows)++;
        i_menu= menuitem_next(i_menu);

    }while(1);

    return;
}

extern struct menuitem*
menuitem_next(
    struct menuitem*                    i_menu)
{
    unsigned int                        l_level;

    l_level= 0;

    do
    {

        if (0 == (*i_menu).m_label)
        {
            if (0 == l_level)
            {
                break;
            }
            l_level--;
        }
        else if (MENUITEM_SUBMENU & (*i_menu).m_flags)
        {
            l_level++;
        }

        i_menu++;

    }while(l_level);

    return i_menu;
}

extern struct menuitem*
menuitem_nth(
    struct menuitem*                    i_menu,
    unsigned int const                  i_nth)
{
    unsigned int                        l_slot;

    l_slot= 0;

    do
    {

        if (i_nth <= l_slot)
        {
            break;
        }

        i_menu= menuitem_next(i_menu);

        if (0 == (*i_menu).m_label)
        {
            break;
        }

        l_slot++;

    }while(1);

    return i_menu;
}

extern void
menuitem_flatten(
    struct array*const                  o_array,
    struct menuitem*                    i_menu)
{

    (*o_array).m_taken= 0;

    do
    {

        if (0 == (*i_menu).m_label)
        {
            break;
        }

        array_push(o_array, (void*)i_menu);

        if (MENUITEM_SEPERATOR & (*i_menu).m_flags)
        {
            array_push(o_array, (void*)0);
        }

        i_menu= menuitem_next(i_menu);

    }while(1);

    return;
}

extern struct menuitem *
menuitem_find_scancode(
    struct menuitem *                   i_menu,
    enum scancodes const                i_scan)
{
    unsigned int                        l_level;
    struct menuitem *                   l_menu;

    l_level= 0;
    l_menu= i_menu;

    do
    {

        if (0 == (MENUITEM_DISABLED & (*l_menu).m_flags))
        {
            if (0 == (*l_menu).m_label)
            {
                if (0 == l_level)
                {
                    l_menu= 0;
                    break;
                }
                l_level--;
            }
            else if (MENUITEM_SUBMENU & (*l_menu).m_flags)
            {
                l_level++;
            }
            else if (i_scan == (*l_menu).m_accel_scancode)
            {
                break;
            }
        }

        l_menu++;

    }while(1);

    return l_menu;
}
