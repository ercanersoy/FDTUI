/*
 FDTUICFG.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "fdtuicfg.h"
#include "ini.h"
#include "skin.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#if defined(__DOS__) || defined(__NT__) && !defined(__NTCRUSTY__)
#include <sys\types.h>
#include <sys\stat.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#endif

#if !defined(PATH_MAX)
#   define PATH_MAX                     4096
#endif

static enum background
strtobcolor(
    char const *                        i_color,
    enum background const               i_default)
{
    enum background                     l_color;
    int                                 l_rc;

    l_color= i_default;

    do
    {

        l_rc= strcmp(i_color, "black");
        if (0 == l_rc)
        {
            l_color= bcolor_black;
            break;
        }

        l_rc= strcmp(i_color, "blue");
        if (0 == l_rc)
        {
            l_color= bcolor_blue;
            break;
        }

        l_rc= strcmp(i_color, "green");
        if (0 == l_rc)
        {
            l_color= bcolor_green;
            break;
        }

        l_rc= strcmp(i_color, "cyan");
        if (0 == l_rc)
        {
            l_color= bcolor_cyan;
            break;
        }

        l_rc= strcmp(i_color, "red");
        if (0 == l_rc)
        {
            l_color= bcolor_red;
            break;
        }

        l_rc= strcmp(i_color, "magenta");
        if (0 == l_rc)
        {
            l_color= bcolor_magenta;
            break;
        }

        l_rc= strcmp(i_color, "grey");
        if (0 == l_rc)
        {
            l_color= bcolor_light_gray;
            break;
        }

    }while(0);

    return l_color;
}

static enum foreground
strtofcolor(
    char const *                        i_color,
    enum foreground const               i_default)
{
    enum foreground                     l_color;
    int                                 l_rc;

    l_color= i_default;

    do
    {

        l_rc= strcmp(i_color, "black");
        if (0 == l_rc)
        {
            l_color= fcolor_black;
            break;
        }

        l_rc= strcmp(i_color, "blue");
        if (0 == l_rc)
        {
            l_color= fcolor_blue;
            break;
        }

        l_rc= strcmp(i_color, "green");
        if (0 == l_rc)
        {
            l_color= fcolor_green;
            break;
        }

        l_rc= strcmp(i_color, "cyan");
        if (0 == l_rc)
        {
            l_color= fcolor_cyan;
            break;
        }

        l_rc= strcmp(i_color, "red");
        if (0 == l_rc)
        {
            l_color= bcolor_red;
            break;
        }

        l_rc= strcmp(i_color, "magenta");
        if (0 == l_rc)
        {
            l_color= fcolor_magenta;
            break;
        }

        l_rc= strcmp(i_color, "yellow");
        if (0 == l_rc)
        {
            l_color= fcolor_yellow;
            break;
        }

        l_rc= strcmp(i_color, "white");
        if (0 == l_rc)
        {
            l_color= fcolor_white;
            break;
        }

    }while(0);

    return l_color;
}

static void
desktop_load(
    struct skin_desktop*const           io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    l_value= ini_lookup(&l_found, i_ini, i_section, "fill");
    if (l_value) 
    {
        (*io_skin).m_fillc= *l_value;
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "foreground");
    if (l_value)
    {
        (*io_skin).m_fcolor= strtofcolor(l_value, (*io_skin).m_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "background");
    if (l_value)
    {
        (*io_skin).m_bcolor= strtobcolor(l_value, (*io_skin).m_bcolor);
    }

    return;
}

static void
window_load(
    struct skin_window*const            io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    l_value= ini_lookup(&l_found, i_ini, i_section, "fill");
    if (l_value) 
    {
        (*io_skin).m_fillc= *l_value;
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "foreground");
    if (l_value)
    {
        (*io_skin).m_fcolor= strtofcolor(l_value, (*io_skin).m_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "background");
    if (l_value)
    {
        (*io_skin).m_bcolor= strtobcolor(l_value, (*io_skin).m_bcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "text.foreground");
    if (l_value)
    {
        (*io_skin).m_text_fcolor= strtofcolor(l_value, (*io_skin).m_text_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "text.background");
    if (l_value)
    {
        (*io_skin).m_text_bcolor= strtobcolor(l_value, (*io_skin).m_text_bcolor);
    }

    return;
}

static void
menu_load(
    struct skin_menu*const              io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    window_load(&(*io_skin).m_window, i_ini, i_section);

    l_value= ini_lookup(&l_found, i_ini, i_section, "selected.foreground");
    if (l_value)
    {
        (*io_skin).m_selected_fcolor= strtofcolor(l_value, (*io_skin).m_selected_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "selected.background");
    if (l_value)
    {
        (*io_skin).m_selected_bcolor= strtobcolor(l_value, (*io_skin).m_selected_bcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "disabled.foreground");
    if (l_value)
    {
        (*io_skin).m_disabled_fcolor= strtofcolor(l_value, (*io_skin).m_disabled_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "disabled.background");
    if (l_value)
    {
        (*io_skin).m_disabled_bcolor= strtobcolor(l_value, (*io_skin).m_disabled_bcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "hotkey.foreground");
    if (l_value)
    {
        (*io_skin).m_hotkey_fcolor= strtofcolor(l_value, (*io_skin).m_hotkey_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "hotkey.background");
    if (l_value)
    {
        (*io_skin).m_hotkey_bcolor= strtobcolor(l_value, (*io_skin).m_hotkey_bcolor);
    }

    return;
}

static void
button_load(
    struct skin_button*const            io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    l_value= ini_lookup(&l_found, i_ini, i_section, "foreground");
    if (l_value)
    {
        (*io_skin).m_fcolor= strtofcolor(l_value, (*io_skin).m_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "background");
    if (l_value)
    {
        (*io_skin).m_bcolor= strtobcolor(l_value, (*io_skin).m_bcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "focused.foreground");
    if (l_value)
    {
        (*io_skin).m_focused= strtofcolor(l_value, (*io_skin).m_focused);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "shadow.background");
    if (l_value)
    {
        (*io_skin).m_shadow= strtobcolor(l_value, (*io_skin).m_shadow);
    }

    return;
}

static void
label_load(
    struct skin_label*const             io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    l_value= ini_lookup(&l_found, i_ini, i_section, "foreground");
    if (l_value)
    {
        (*io_skin).m_fcolor= strtofcolor(l_value, (*io_skin).m_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "background");
    if (l_value)
    {
        (*io_skin).m_bcolor= strtobcolor(l_value, (*io_skin).m_bcolor);
    }

    return;
}

static void
slider_load(
    struct skin_slider*const            io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    l_value= ini_lookup(&l_found, i_ini, i_section, "fill");
    if (l_value) 
    {
        (*io_skin).m_fillc= *l_value;
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "foreground");
    if (l_value)
    {
        (*io_skin).m_fcolor= strtofcolor(l_value, (*io_skin).m_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "background");
    if (l_value)
    {
        (*io_skin).m_bcolor= strtobcolor(l_value, (*io_skin).m_bcolor);
    }

    return;
}

static void
listbox_load(
    struct skin_listbox*const           io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    l_value= ini_lookup(&l_found, i_ini, i_section, "foreground");
    if (l_value)
    {
        (*io_skin).m_fcolor= strtofcolor(l_value, (*io_skin).m_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "background");
    if (l_value)
    {
        (*io_skin).m_bcolor= strtobcolor(l_value, (*io_skin).m_bcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "highlight.foreground");
    if (l_value)
    {
        (*io_skin).m_highlight_fcolor= strtofcolor(l_value, (*io_skin).m_highlight_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "highlight.background");
    if (l_value)
    {
        (*io_skin).m_highlight_bcolor= strtobcolor(l_value, (*io_skin).m_highlight_bcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "highlight.foreground");
    if (l_value)
    {
        (*io_skin).m_selected_fcolor= strtofcolor(l_value, (*io_skin).m_selected_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "selected.foreground");
    if (l_value)
    {
        (*io_skin).m_selected_fcolor= strtofcolor(l_value, (*io_skin).m_selected_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "selected.background");
    if (l_value)
    {
        (*io_skin).m_selected_bcolor= strtobcolor(l_value, (*io_skin).m_selected_bcolor);
    }

    return;
}

static void
edit_load(
    struct skin_edit*const              io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    l_value= ini_lookup(&l_found, i_ini, i_section, "foreground");
    if (l_value)
    {
        (*io_skin).m_fcolor= strtofcolor(l_value, (*io_skin).m_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "background");
    if (l_value)
    {
        (*io_skin).m_bcolor= strtobcolor(l_value, (*io_skin).m_bcolor);
    }

    return;
}

static void
tree_load(
    struct skin_tree*const              io_skin,
    struct ini *const                   i_ini,
    char const*                         i_section)
{
    int                                 l_found;
    char const *                        l_value;

    l_value= ini_lookup(&l_found, i_ini, i_section, "foreground");
    if (l_value)
    {
        (*io_skin).m_fcolor= strtofcolor(l_value, (*io_skin).m_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "background");
    if (l_value)
    {
        (*io_skin).m_bcolor= strtobcolor(l_value, (*io_skin).m_bcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "selected.foreground");
    if (l_value)
    {
        (*io_skin).m_selected_fcolor= strtofcolor(l_value, (*io_skin).m_selected_fcolor);
    }

    l_value= ini_lookup(&l_found, i_ini, i_section, "selected.background");
    if (l_value)
    {
        (*io_skin).m_selected_bcolor= strtobcolor(l_value, (*io_skin).m_selected_bcolor);
    }

    return;
}

extern void
fdtuicfg_load_path(
    char const*                         i_path)
{
    int                                 l_exit;
    struct ini                          l_ini;

    do
    {

        l_exit= ini_assign(&l_ini);

        if (l_exit)
        {
            break;
        }

        l_exit= ini_parse(&l_ini, i_path, INI_OPTION_NONE);            

        if (l_exit)
        {
            break;
        }

        desktop_load(&_skin_desktop, &l_ini, "desktop");
        window_load(&_skin_window, &l_ini, "window");
        menu_load(&_skin_menu, &l_ini, "menu");
        button_load(&_skin_button, &l_ini, "button");
        label_load(&_skin_label, &l_ini, "label");
        slider_load(&_skin_slider, &l_ini, "slider");
        listbox_load(&_skin_listbox, &l_ini, "listbox");
        edit_load(&_skin_edit, &l_ini, "edit");
        tree_load(&_skin_tree, &l_ini, "tree");

    }while(0);

    ini_discharge(&l_ini);

    return;
}

extern void
fdtuicfg_load()
{
#if defined(__DOS__) || defined(__NT__)
    size_t                              l_length;
    char*                               l_ptr;
#endif
    char*                               l_path;
    int                                 l_rc;
    struct stat                         l_stat;

    l_path= 0;

    do
    {

        l_rc= stat("fdostui.ini", &l_stat);
        if (0 == l_rc)
        {
            fdtuicfg_load_path("fdostui.ini");
            break;
        }

        l_path= calloc(1, 1+PATH_MAX);

        if (0 == l_path)
        {
            break;
        }

#if defined(__NT__)
       GetModuleFileNameA(0, l_path, PATH_MAX);
#define TEST_MODULE
#elif defined(__DOS__)
       strlcpy(l_path, __argv[0], PATH_MAX);
#define TEST_MODULE
#endif

#if defined(TEST_MODULE)
        l_length= strlen(l_path);
        l_ptr= &l_path[l_length];
        while(l_length)
        {
            if ('\\' == (*l_ptr))
            {
                break;
            }
            if (':' == (*l_ptr))
            {
                l_ptr++;
                break;
            }
            l_ptr--;
            l_length--;
        }
        strcpy(l_ptr, "\\fdostui.ini");

        l_rc= stat(l_path, &l_stat);
        if (0 == l_rc)
        {
            fdtuicfg_load_path(l_path);
            break;
        }
#endif

#if defined(__NCURSES__)
        l_ptr= getenv("XDG_CONFIG_HOME");
        if (l_ptr)
        {
            snprintf(l_path, PATH_MAX, "%s/fdostui.ini", l_ptr);
        }
        else
        {
            l_ptr= getenv("HOME");
            if (0 == l_ptr)
            {
                break;
            }
            snprintf(l_path, PATH_MAX, "%s/.config/fdostui.ini", l_ptr);
        }

        l_rc= stat(l_path, &l_stat);
        if (0 == l_rc)
        {
            fdtuicfg_load_path(l_path);
            break;
        }
#endif

    }while(0);

    free(l_path);

    return;
}
