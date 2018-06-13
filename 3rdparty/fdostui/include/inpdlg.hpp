/*
 INPDLG.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States
*/
#ifndef __inpdlg_hpp__

#include "window.hpp"
#include "label.hpp"
#include "entry.hpp"
#include "button.hpp"

class inputdlg : public window
{

public:

    inputdlg(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);

    virtual
    ~inputdlg();

    unsigned char*
    get_value() const;
    
    void
    set_prompt(
        unsigned char const*            i_prompt);

    void
    set_value(
        unsigned char const*            i_value);

protected:

    label*                              m_label_prompt;
    entry*                              m_entry_value;
    button*                             m_button_cancel;
    button*                             m_button_ok;

private:

    inputdlg();

    inputdlg(
    const inputdlg&);

    inputdlg&
    operator=(inputdlg const&);

};

inline void
inputdlg::set_prompt(
    unsigned char const*                i_prompt)
{
    (*m_label_prompt).set_text(i_prompt);
    return;
}

inline void
inputdlg::set_value(
    unsigned char const*                i_value)
{
    (*m_entry_value).set_text(i_value);
    return;
}

#define __inpdlg_hpp__
#endif
