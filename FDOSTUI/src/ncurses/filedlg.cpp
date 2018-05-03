/*
vim:expandtab:softtabstop=4:tabstop=4:shiftwidth=4:nowrap:ruler
*/
/*
 FILEDLG.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "filedlg.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <fnmatch.h>
#include <errno.h>

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
    m_checkbutton_hidden(0),
    m_button_refresh(0),
    m_button_cancel(0),
    m_button_ok(0),
    m_label_path(0),
    m_label_message(0),
    m_listbox_file(0)
{

    set_attributes(window::BORDER|window::TITLE);

    m_label_file= new label(1,1,5,1);
    (*m_label_file).set_text(reinterpret_cast<unsigned char const*>("Name:"));

    m_entry_file= new entry(7,1,33,1);

    m_label_filter= new label(1,3,7,1);
    (*m_label_filter).set_text(reinterpret_cast<unsigned char const*>("Filter:"));

    m_entry_filter= new entry(9,3,13,1);

    m_checkbutton_hidden= new checkbutton(1,5,14,1);
    (*m_checkbutton_hidden).set_text(reinterpret_cast<unsigned char const*>("Hidden"));
    (*m_checkbutton_hidden).set_user_data(this);
    (*m_checkbutton_hidden).set_signal_clicked(checkbutton_attribute_cb);

    m_label_path= new label(1,7,48,1);

    m_label_message= new label(1,8,48,1);

    m_listbox_file= new listbox(1,11,15,10);
    (*m_listbox_file).set_label(reinterpret_cast<unsigned char const*>("Files"));
    (*m_listbox_file).set_user_data(this);
    (*m_listbox_file).set_signal_selected(listbox_file_cb);

    m_combobox_directories= new combobox(20, 11, 15, 1);
    (*m_combobox_directories).set_user_data(this);
    (*m_combobox_directories).set_signal_changed(combobox_directory_cb);
    (*m_combobox_directories).set_editable(false);

    m_button_refresh= new button(42,1,13,3);
    (*m_button_refresh).set_style(button::STYLE_BOX);
    (*m_button_refresh).set_text(reinterpret_cast<unsigned char const*>("Refresh"));
    (*m_button_refresh).set_user_data(this);
    (*m_button_refresh).set_signal_clicked(button_refresh_cb);

    m_button_cancel= new button(42,4,13,3);
    (*m_button_cancel).set_style(button::STYLE_BOX);
    (*m_button_cancel).set_text(reinterpret_cast<unsigned char const*>("Cancel"));
    (*m_button_cancel).set_response(RESPONSE_CANCEL);

    m_button_ok= new button(42,7,13,3);
    (*m_button_ok).set_style(button::STYLE_BOX);
    (*m_button_ok).set_text(reinterpret_cast<unsigned char const*>("Ok"));
    (*m_button_ok).set_response(RESPONSE_ACCEPT);

    add(m_label_file);
    add(m_entry_file);
    add(m_label_filter);
    add(m_entry_filter);
    add(m_checkbutton_hidden);
    add(m_label_path);
    add(m_label_message);
    add(m_listbox_file);
    add(m_combobox_directories);
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
    DIR*                           	    l_dir;
	struct dirent*			            l_ent;
    unsigned char const*                l_filter;
    bool                                l_include_hidden;
    int                                 l_rc;
    struct stat                         l_stat;

    (*m_listbox_file).remove_all();
    (*m_combobox_directories).remove_all();
    (*m_combobox_directories).add(reinterpret_cast<unsigned char const*>(".."));

    l_filter= (*m_entry_filter).get_text();

    l_include_hidden= (*m_checkbutton_hidden).get_state();

    do
    {

		l_dir= opendir(".");

		if (0 == l_dir)
        {
            (*m_label_message).set_text(reinterpret_cast<unsigned char const*>(
                "Error: Unable to open directory"));
            break;
        }

        do
        {
            l_ent= readdir(l_dir);

            if (0 == l_ent)
            {
                break;
            }

            if ('.' == l_ent->d_name[0])
            {
                if (0 == l_ent->d_name[1]) 
                {
                    continue;
                }

                if ('.' == (*l_ent).d_name[1])
                {
                    if (0 == (*l_ent).d_name[2])
                    {
                        continue;
                    }
                }

                if (!l_include_hidden)
                {
                    continue;
                }
            }

            l_rc= stat((*l_ent).d_name, &l_stat);

            if (-1 == l_rc)
            {
                continue;
            }

            if (S_ISDIR(l_stat.st_mode))
            {
                (*m_combobox_directories).add(
                    reinterpret_cast<unsigned char const*>((*l_ent).d_name));
                continue;
            }

            if (0 == S_ISREG(l_stat.st_mode))
            {
                continue;
            }

            if (l_filter && l_filter[0])
            {
                l_rc= fnmatch(
                    reinterpret_cast<char const*>(l_filter),
                    (*l_ent).d_name,
                    0); 
                if (l_rc)
                {
                    continue;
                }
            }

            (*m_listbox_file).add(
                reinterpret_cast<unsigned char const*>((*l_ent).d_name));

        }while(1);

		closedir(l_dir);

    }while(0);

    return;
}

void
filedlg::configure_default()
{
    unsigned char                       l_buf[1+PATH_MAX];

    getcwd(reinterpret_cast<char *>(l_buf), sizeof(l_buf));

    (*m_label_path).set_text(&l_buf[0]);

    configure_listbox_file();

    return;
}

void
filedlg::directory_changed_internal()
{
    unsigned char                       l_buf[PATH_MAX+1];
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

        l_rc= chdir(reinterpret_cast<char const*>(l_item));

        if (l_rc)
        {
            (*m_label_message).set_text(reinterpret_cast<unsigned char const*>(
                "Error: Unable to change directory"));
            break;
        }

        l_ptr= getcwd(reinterpret_cast<char*>(l_buf), sizeof(l_buf));

        if (0 == l_ptr)
        {
            (*m_label_message).set_text(reinterpret_cast<unsigned char const*>(
                "Error: Unable to change directory"));
            break;
        }

        (*m_label_path).set_text(l_buf);
        configure_listbox_file();

    }while(0);

    wm_draw_widget(this);

    return;
}

void
filedlg::file_changed_internal()
{
    int                                 l_cur_x;
    int                                 l_cur_y;
    unsigned char const*                l_item;
    int                                 l_slot;

    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        getyx(stdscr, l_cur_y, l_cur_x);
    }

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

    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        ::move(l_cur_y, l_cur_x);
    }

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
    char*                               l_ptr;

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

        l_ptr= realpath(
            reinterpret_cast<char const*>(l_filename),
            reinterpret_cast<char*>(l_fullpath));

        if (0 == l_ptr)
        {
            strncpy(
                reinterpret_cast<char*>(l_fullpath),
                reinterpret_cast<char const*>(l_filename),
                PATH_MAX);
            l_fullpath[PATH_MAX]= 0;
        }

    }while(0);

    return l_fullpath;
}

void
filedlg::set_filename(
    unsigned char const*                i_filename)
{
    char*                               l_fname;
    char*                               l_fullpath;
    size_t                              l_len;
    char*                               l_ptr;
    int                                 l_rc;
    struct stat                         l_stat;

    l_fullpath= 0;
    l_fname= 0;
    (*m_entry_file).set_text(i_filename);

    do
    {

        l_fullpath= reinterpret_cast<char*>(malloc(PATH_MAX+1));

        l_ptr= realpath(
            reinterpret_cast<char const*>(i_filename),
            reinterpret_cast<char*>(l_fullpath));

        if (0 == l_ptr)
        {
            (*m_label_message).set_text(reinterpret_cast<unsigned char const*>(
                "Error: Unable to change directory"));
            break;
        }

        (*m_label_path).set_text(0);
        (*m_label_message).set_text(0);
        (*m_listbox_file).remove_all();
        (*m_combobox_directories).remove_all();

        l_rc= stat(l_fullpath, &l_stat);

        if (-1 == l_rc)
        {
            (*m_label_message).set_text(reinterpret_cast<unsigned char const*>(
                "Error: Unable to change directory"));
            break;
        }
    
        if (S_ISDIR(l_stat.st_mode))
        {
            l_fname= 0;
        }
        else if (S_ISREG(l_stat.st_mode))
        {
            l_len= strlen(l_fullpath);
            l_fname= l_fullpath + l_len - 1;
            do
            {
                if (0 == l_len)
                {
                    l_fname= 0;
                    break;
                }
                if ('/' == l_fname[0])
                {
                    *l_fname++= 0;
                    break; 
                }
                l_len--;
                l_fname--;
            }while(1);
        }
        else
        {
            (*m_label_message).set_text(reinterpret_cast<unsigned char const*>(
                "Error: Unable to change directory"));
            break;
        }

        (void)chdir(l_fullpath);
        (*m_label_path).set_text(reinterpret_cast<unsigned char const*>(l_fullpath));
        (*m_entry_file).set_text(reinterpret_cast<unsigned char const*>(l_fname));

        configure_listbox_file();

    }while(0);

    free(l_fullpath);

    return;
}

void
filedlg::refresh()
{
    int                                 l_cur_x;
    int                                 l_cur_y;
    unsigned char const*                l_filename;

    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        getyx(stdscr, l_cur_y, l_cur_x);
    }

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

    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        ::move(l_cur_y, l_cur_x);
    }

    return;
}
