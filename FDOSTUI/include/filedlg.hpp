/*
 FILEDLG.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#ifndef __filedlg_hpp__

#include "window.hpp"
#include "label.hpp"
#include "entry.hpp"
#include "checkbtn.hpp"
#include "listbox.hpp"
#include "combobox.hpp"

class filedlg : public window
{

public:

    filedlg(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~filedlg();

    unsigned char *
    get_filename() const;

    void
    set_filename(
        unsigned char const*            i_filename);
    
protected:

    label*                              m_label_file;
    entry*                              m_entry_file;
    label*                              m_label_filter;
    entry*                              m_entry_filter;
#if defined(__DOS__) || defined(__DJGPP__) || defined(__NT__)
    checkbutton*                        m_checkbutton_readonly;
#endif
    checkbutton*                        m_checkbutton_hidden;
#if defined(__DOS__) || defined(__DJGPP__) || defined(__NT__)
    checkbutton*                        m_checkbutton_system;
#endif
    button*                             m_button_refresh;
    button*                             m_button_cancel;
    button*                             m_button_ok;
    label*                              m_label_path;
    label*                              m_label_message;
    listbox*                            m_listbox_file;
    combobox*                           m_combobox_directories;
#if defined(__DOS__) || defined(__DJGPP__) || defined(__NT__)
    listbox*                            m_listbox_drives;
#endif

    void
    configure_default();

    void
    configure_listbox_file();

#if defined(__DOS__) || defined(__DJGPP__) || defined(__NT__)
    void
    configure_listbox_directories();
#endif

    void
    configure_listbox_drives();

private:

    filedlg();

    filedlg(
    const filedlg&);

    filedlg&
    operator=(filedlg const&);

#if defined(__DOS__) || defined(__DJGPP__) || defined(__NT__)
    static void
    listbox_drive_cb(
        listbox const*                  i_listbox,
        void*                           io_user_data);
#endif

    static void
    combobox_directory_cb(
        entry const*                    i_entry,
        void*                           io_user_data);

    static void
    listbox_file_cb(
        listbox const*                  i_listbox,
        void*                           io_user_data);

    static void
    checkbutton_attribute_cb(
        button const*                   i_button,
        void*                           io_user_data);

    static void
    button_refresh_cb(
        button const*                   i_button,
        void*                           io_user_data);

public:

#if defined(__DOS__) || defined(__DJGPP__) || defined(__NT__)
    void
    drive_changed_internal();
#endif

    void
    directory_changed_internal();

    void
    file_changed_internal();

    void
    attribute_changed_internal();

    void
    refresh();

};

#define __filedlg_hpp__
#endif
