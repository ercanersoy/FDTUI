/*
 FILEDLG.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "filedlg.hpp"
#include "strings.h"
#include "hw.h"
#include <sys\types.h>
#include <dos.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#if defined(__DJGPP__)
#include <dir.h>
#include <limits.h>
#include <unistd.h>
#define _getdcwd(drive, buffer, max) getcwd((buffer), (max))
#define _dos_findclose(f)
#define _chdir chdir
#endif

void
filedlg::listbox_drive_cb(
    listbox const*                      i_listbox,
    void*                               io_user_data)
{
    filedlg*                            l_filedlg;

    l_filedlg= reinterpret_cast<filedlg*>(io_user_data);
    (*l_filedlg).drive_changed_internal();

    return;
}

void
filedlg::combobox_directory_cb(
    entry const*                        i_entry,
    void*                               io_user_data)
{
    filedlg*                            l_filedlg;

    l_filedlg= reinterpret_cast<filedlg*>(io_user_data);
    (*l_filedlg).directory_changed_internal();

    return;

}

void
filedlg::listbox_file_cb(
    listbox const*                      i_listbox,
    void*                               io_user_data)
{
    filedlg*                            l_filedlg;

    l_filedlg= reinterpret_cast<filedlg*>(io_user_data);
    (*l_filedlg).file_changed_internal();

    return;
}

void
filedlg::checkbutton_attribute_cb(
    button const*                       i_button,
    void*                               io_user_data)
{
    filedlg*                            l_filedlg;

    l_filedlg= reinterpret_cast<filedlg*>(io_user_data);
    (*l_filedlg).attribute_changed_internal();

    return;
}

void
filedlg::button_refresh_cb(
    button const*                       i_button,
    void*                               io_user_data)
{
    filedlg*                            l_filedlg;

    l_filedlg= reinterpret_cast<filedlg*>(io_user_data);
    (*l_filedlg).refresh();

    return;
}

filedlg::filedlg(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    window(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_label_file(0),
    m_entry_file(0),
    m_label_filter(0),
    m_entry_filter(0),
    m_checkbutton_readonly(0),
    m_checkbutton_hidden(0),
    m_checkbutton_system(0),
    m_button_refresh(0),
    m_button_cancel(0),
    m_button_ok(0),
    m_label_path(0),
    m_label_message(0),
    m_listbox_file(0),
    m_combobox_directories(0),
    m_listbox_drives(0)
{

    set_attributes(window::BORDER|window::TITLE);

    m_label_file= new label(1,1,5,1);
    (*m_label_file).set_text(_text_name);

    m_entry_file= new entry(7,1,33,1);

    m_label_filter= new label(1,3,7,1);
    (*m_label_filter).set_text(_text_filter);

    m_entry_filter= new entry(9,3,13,1);

    m_checkbutton_readonly= new checkbutton(1,5,14,1);
    (*m_checkbutton_readonly).set_text(_text_read_only);
    (*m_checkbutton_readonly).set_user_data(this);
    (*m_checkbutton_readonly).set_signal_clicked(checkbutton_attribute_cb);

    m_checkbutton_hidden= new checkbutton(16,5,13,1);
    (*m_checkbutton_hidden).set_text(_text_hidden);
    (*m_checkbutton_hidden).set_user_data(this);
    (*m_checkbutton_hidden).set_signal_clicked(checkbutton_attribute_cb);

    m_checkbutton_system= new checkbutton(28,5,13,1);
    (*m_checkbutton_system).set_text(_text_system);
    (*m_checkbutton_system).set_user_data(this);
    (*m_checkbutton_system).set_signal_clicked(checkbutton_attribute_cb);

    m_label_path= new label(1,7,48,1);

    m_label_message= new label(1,8,48,1);

    m_listbox_file= new listbox(1,11,15,10);
    (*m_listbox_file).set_label(_text_files);
    (*m_listbox_file).set_user_data(this);
    (*m_listbox_file).set_signal_selected(listbox_file_cb);

    m_combobox_directories= new combobox(20, 11, 15, 1);
    (*m_combobox_directories).set_user_data(this);
    (*m_combobox_directories).set_signal_changed(combobox_directory_cb);
    (*m_combobox_directories).set_editable(false);

    m_listbox_drives= new listbox(40, 11, 15, 10);
    (*m_listbox_drives).set_label(_text_drives);
    (*m_listbox_drives).set_user_data(this);
    (*m_listbox_drives).set_signal_selected(listbox_drive_cb);

    m_button_refresh= new button(42,1,13,3);
    (*m_button_refresh).set_style(button::STYLE_BOX);
    (*m_button_refresh).set_text(_text_refresh);
    (*m_button_refresh).set_user_data(this);
    (*m_button_refresh).set_signal_clicked(button_refresh_cb);

    m_button_cancel= new button(42,4,13,3);
    (*m_button_cancel).set_style(button::STYLE_BOX);
    (*m_button_cancel).set_text(_text_cancel);
    (*m_button_cancel).set_response(RESPONSE_CANCEL);

    m_button_ok= new button(42,7,13,3);
    (*m_button_ok).set_style(button::STYLE_BOX);
    (*m_button_ok).set_text(_text_ok);
    (*m_button_ok).set_response(RESPONSE_ACCEPT);

    add(m_label_file);
    add(m_entry_file);
    add(m_label_filter);
    add(m_entry_filter);
    add(m_checkbutton_readonly);
    add(m_checkbutton_hidden);
    add(m_checkbutton_system);
    add(m_label_path);
    add(m_label_message);
    add(m_listbox_file);
    add(m_combobox_directories);
    add(m_listbox_drives);
    add(m_button_refresh);
    add(m_button_cancel);
    add(m_button_ok);

    configure_default();

    return;
}
    
filedlg::~filedlg()
{
    return;
}

void
filedlg::configure_listbox_file()
{
    unsigned                            l_attributes;
    struct find_t                       l_info;
    unsigned                            l_rc;
    bool                                l_state;
    unsigned char const*                l_filter;

    (*m_listbox_file).remove_all();

    l_filter= (*m_entry_filter).get_text();

    if (0 == l_filter || 0 == l_filter[0])
    {
        l_filter= reinterpret_cast<unsigned char const*>("*.*");
        (*m_entry_filter).set_text(l_filter);
    }

    l_attributes= _A_NORMAL;

    l_state= (*m_checkbutton_hidden).get_state();

    if (l_state)
    {
        l_attributes|= _A_HIDDEN;
    }

    l_state= (*m_checkbutton_system).get_state();

    if (l_state)
    {
        l_attributes|= _A_SYSTEM;
    }

    l_rc= _dos_findfirst(
        reinterpret_cast<char const*>(l_filter),
        l_attributes,
        &l_info);

    do
    {

        if (l_rc)
        {
            break;
        }

        (*m_listbox_file).add(reinterpret_cast<unsigned char const*>(l_info.name));

        l_rc= _dos_findnext(&l_info);

    }while(1);

    _dos_findclose(&l_info);

    return;
}

void
filedlg::configure_listbox_directories()
{
    unsigned char const*                l_path;
    struct dirent*                      l_ent;
    DIR*                                l_dir;

    l_path= (*m_label_path).get_text();
    (*m_combobox_directories).remove_all();

    do
    {

        if (l_path && l_path[0])
        {
            l_dir= opendir(reinterpret_cast<char const*>(l_path));
        }
        else
        {
            l_dir= opendir(".");
        }

        if (0 == l_dir)
        {
            break;
        }

        do
        {
            l_ent= readdir(l_dir);

            if (0 == l_ent)
            {
                break;
            }

#if defined(__DJGPP__)
            if ((DT_DIR & (*l_ent).d_type) &&
#else
            if ((_A_SUBDIR & (*l_ent).d_attr) &&
#endif
                !('.' == (*l_ent).d_name[0] && 0 == (*l_ent).d_name[1]))
            {
                (*m_combobox_directories).add(
                    reinterpret_cast<unsigned char const*>((*l_ent).d_name));
            }

        }while(1);

        closedir(l_dir);

    }while(0);

    return;
}

void
filedlg::configure_listbox_drives()
{
    unsigned char                       l_buf[2];
    unsigned int                        l_drivecur;
    size_t                              l_drive;
    size_t                              l_slot;

#ifdef __NT__
    hw_detect_drives();
#endif

    _dos_getdrive(&l_drivecur);
    l_drivecur--;

    (*m_listbox_drives).remove_all();
    l_buf[1]= 0;

    for (l_drive= 0; HW_DRIVE_MAX > l_drive; l_drive++)
    {
        if (_hw_drives[l_drive])
        {
            l_buf[0]= 'A'+l_drive;
            (*m_listbox_drives).add(l_buf);
            if (l_drive == l_drivecur)
            {
                l_slot= (*m_listbox_drives).get_count() - 1;
                (*m_listbox_drives).set_selected(l_slot);
            }
        }
    }

    return;
}

void
filedlg::configure_default()
{
    unsigned int                        l_drive;
    unsigned char                       l_buf[1+PATH_MAX];

    _dos_getdrive(&l_drive);
    _getdcwd(
        l_drive,
        reinterpret_cast<char *>(l_buf),
        sizeof(l_buf));

    (*m_label_path).set_text(&l_buf[0]);

    configure_listbox_file();
    configure_listbox_directories();
    configure_listbox_drives();

    return;
}

void
filedlg::drive_changed_internal()
{
    unsigned char                       l_buf[PATH_MAX+1];
    unsigned char const*                l_item;
    unsigned int                        l_drive;
    int                                 l_slot;
    char*                               l_ptr;
    unsigned int                        l_total;
    int                                 l_rc;

    do
    {

        (*m_label_message).set_text(0);
        (*m_entry_file).set_text(0);
        (*m_label_path).set_text(0);
        (*m_combobox_directories).remove_all();
        (*m_listbox_file).remove_all();

        l_slot= (*m_listbox_drives).get_selected_first();

        if (0 > l_slot)
        {
            break;
        }

        l_item= (*m_listbox_drives).get_item(static_cast<size_t const>(l_slot));
        l_drive= toupper(l_item[0]) - 'A' + 1;

        _dos_setdrive(l_drive, &l_total);
        l_ptr= _getdcwd(l_drive, reinterpret_cast<char*>(l_buf), sizeof(l_buf));

        if (0 == l_ptr)
        {
            (*m_label_message).set_text(_text_unable_to_change_directory);
            break;
        }

        l_rc= _chdir(reinterpret_cast<char const*>(l_buf));

        if (l_rc)
        {
            (*m_label_message).set_text(_text_unable_to_change_directory);
            break;
        }

        (*m_label_path).set_text(l_buf);
        configure_listbox_directories();
        configure_listbox_file();

    }while(0);

    wm_draw_widget(this);

    return;
}

void
filedlg::directory_changed_internal()
{
    unsigned char                       l_buf[PATH_MAX+1];
    unsigned int                        l_drive;
    unsigned char const*                l_item;
    char*                               l_ptr;
    int                                 l_rc;

    do
    {

        (*m_label_message).set_text(0);
        (*m_entry_file).set_text(0);
        (*m_label_path).set_text(0);

        l_item= (*m_combobox_directories).get_text();

        if (0 == l_item || 0 == l_item[0])
        {
            break;
        }

        l_rc= _chdir(reinterpret_cast<char const*>(l_item));

        if (l_rc)
        {
            (*m_label_message).set_text(_text_unable_to_change_directory);
            break;
        }

        _dos_getdrive(&l_drive);
        l_ptr= _getdcwd(l_drive, reinterpret_cast<char*>(l_buf), sizeof(l_buf));

        if (0 == l_ptr)
        {
            (*m_label_message).set_text(_text_unable_to_change_directory);
            break;
        }

        (*m_label_path).set_text(l_buf);
        configure_listbox_directories();
        configure_listbox_file();

    }while(0);

    wm_draw_widget(this);

    return;
}

void
filedlg::file_changed_internal()
{
    unsigned char const*                l_item;
    int                                 l_slot;

    do
    {

        l_slot= (*m_listbox_file).get_selected_first();

        if (0 > l_slot)
        {
            break;
        }

        l_item= (*m_listbox_file).get_item(static_cast<size_t const>(l_slot));

        if (l_item)
        {
            (*m_entry_file).set_text(l_item);
            wm_draw_widget(m_entry_file);
        }

    }while(0);

    return;
}

void
filedlg::attribute_changed_internal()
{
    configure_listbox_file();
    return;
}

unsigned char *
filedlg::get_filename() const
{
    unsigned char const*                l_filename;
    unsigned char*                      l_fullpath;
#if !defined(__DJGPP__)
    char*                               l_ptr;
#endif

    l_fullpath= 0;

    do
    {

        l_filename= (*m_entry_file).get_text();

        if (0 == l_filename)
        {
            break;
        }

        l_fullpath= reinterpret_cast<unsigned char*>(malloc(PATH_MAX+1));

        if (0 == l_fullpath)
        {
            break;
        }

        memset(l_fullpath, 0, PATH_MAX+1);

#if defined(__DJGPP__)
        realpath(reinterpret_cast<char const*>(l_filename), reinterpret_cast<char*>(l_fullpath));
#else
        l_ptr= _fullpath(
            reinterpret_cast<char*>(l_fullpath),
            reinterpret_cast<char const*>(l_filename),
            PATH_MAX+1);

        if (0 == l_ptr)
        {
            strlcpy(
                reinterpret_cast<char*>(l_fullpath),
                reinterpret_cast<char const*>(l_filename),
                PATH_MAX+1);
        }
#endif

    }while(0);

    return l_fullpath;
}

void
filedlg::set_filename(
    unsigned char const*                i_filename)
{
    unsigned int                        l_drivenum;
    unsigned int                        l_drivecur;
    int                                 l_error;
    unsigned char*                      l_fullpath;
    char*                               l_fullpath2;
    unsigned int                        l_total;
#if defined(__DJGPP__)
    char                                l_drive[MAXDRIVE];
    char                                l_dir[MAXDIR];
    char                                l_fname[MAXFILE];
    char                                l_ext[MAXEXT];
#else
    char*                               l_drive;
    char*                               l_dir;
    char*                               l_fname;
    char*                               l_ext;
    char*                               l_ptr;
#endif

    l_fullpath= 0;
    l_fullpath2= 0;

    do
    {

        (*m_label_path).set_text(0);
        (*m_label_message).set_text(0);
        (*m_listbox_file).remove_all();
        (*m_listbox_drives).unselect_all();
        (*m_combobox_directories).remove_all();

        l_fullpath= reinterpret_cast<unsigned char*>(malloc(PATH_MAX+1));

        if (0 == l_fullpath)
        {
            break;
        }

#if defined(__DJGPP__)
        realpath(reinterpret_cast<char const*>(i_filename), reinterpret_cast<char*>(l_fullpath));
#else
        l_ptr= _fullpath(
            reinterpret_cast<char*>(l_fullpath),
            reinterpret_cast<char const*>(i_filename),
            PATH_MAX+1);

        if (0 == l_ptr)
        {
            (*m_entry_file).set_text(i_filename);
            (*m_label_message).set_text(_text_unable_to_change_directory);
            break;
        }
#endif

#if defined(__DJGPP__)
        fnsplit(reinterpret_cast<char const*>(l_fullpath), l_drive, l_dir, l_fname, l_ext);
#else
        l_fullpath2= reinterpret_cast<char*>(malloc(_MAX_PATH2+1));

        _splitpath2(
            reinterpret_cast<char*>(l_fullpath),
            l_fullpath2,
            &l_drive,
            &l_dir,
            &l_fname,
            &l_ext);
#endif

        l_error= 0;

        if (l_drive)
        {
            l_drivenum= toupper(l_drive[0]) - 'A' + 1;
            _dos_setdrive(l_drivenum, &l_total);
            _dos_getdrive(&l_drivecur);
            if (l_drivecur != l_drivenum)
            {
                l_error= -1;
                (*m_label_message).set_text(_text_unable_to_change_directory);
            }
        }

        sprintf(
            reinterpret_cast<char*>(l_fullpath),
            "%s%s",
            ((l_drive) ? l_drive : ""),
            ((l_dir) ? l_dir : ""));

        if (0 == l_error && l_fullpath[0])
        {
            l_error= _chdir(reinterpret_cast<char const*>(l_fullpath));
            if (l_error)
            {
                (*m_label_message).set_text(_text_unable_to_change_directory);
            }
        }

        (*m_label_path).set_text(l_fullpath);

        l_fullpath[0]= 0;

        if (l_fname)
        {
            sprintf(
                reinterpret_cast<char*>(l_fullpath),
                "%s%s",
                l_fname,
                ((l_ext) ? l_ext : ""));
        }

        (*m_entry_file).set_text(l_fullpath);

        if (l_error)
        {
            break;
        }

        configure_listbox_drives();
        configure_listbox_directories();
        configure_listbox_file();

    }while(0);

    free(l_fullpath);
    free(l_fullpath2);

    return;
}

void
filedlg::refresh()
{
    unsigned char const*                l_filename;

    l_filename= (*m_entry_file).get_text();

    if (l_filename && l_filename[0])
    {
        set_filename(l_filename);
        set_damage(DAMAGE_ALL);
        wm_draw_widget(this);
    }
    else
    {
        configure_listbox_file();
        (*m_listbox_file).set_damage(DAMAGE_VALUE);
        wm_draw_widget(m_listbox_file);
    }

    return;
}
