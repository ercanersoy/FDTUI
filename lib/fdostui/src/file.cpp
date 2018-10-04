/*
 FILE.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "file.hpp"
#include "strings.h"
#include "filedlg.hpp"
#include "screen.h"
#include "wm.hpp"

unsigned char*
file_dialog_open()
{
    filedlg*                            l_dialog;
    unsigned char*                      l_filename;
    enum event_response                 l_response;

    l_filename= 0;
    // _mjo fixme: center x&y
    l_dialog= new filedlg(11, 1, 58, 23);
    (*l_dialog).set_title(_text_open_file);

    wm_draw_widget(l_dialog);

    l_response= wm_run_modal(*l_dialog);

    if (RESPONSE_ACCEPT == l_response)
    {
        l_filename= (*l_dialog).get_filename();
    }

    delete l_dialog;

    wm_draw(0);

    return l_filename;
}
