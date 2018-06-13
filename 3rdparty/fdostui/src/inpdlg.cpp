/*
 INPDLG.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States
*/
#include "inpdlg.hpp"
#include "strings.h"

inputdlg::inputdlg(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    window(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_label_prompt(0),
    m_entry_value(0),
    m_button_cancel(0),
    m_button_ok(0)
{

    set_attributes(window::BORDER|window::TITLE);

    m_label_prompt= new label(1,1,35,1);
    (*m_label_prompt).set_text(_text_enter_value);

    m_entry_value= new entry(1,2,35,1);

    m_button_cancel= new button(1,4,13,3);
    (*m_button_cancel).set_style(button::STYLE_BOX);
    (*m_button_cancel).set_text(_text_cancel);
    (*m_button_cancel).set_response(RESPONSE_CANCEL);

    m_button_ok= new button(16,4,13,3);
    (*m_button_ok).set_style(button::STYLE_BOX);
    (*m_button_ok).set_text(_text_ok);
    (*m_button_ok).set_response(RESPONSE_ACCEPT);

    add(m_label_prompt);
    add(m_entry_value);
    add(m_button_cancel);
    add(m_button_ok);

    return;
}

inputdlg::~inputdlg()
{
    return;
}

unsigned char*
inputdlg::get_value() const
{
    unsigned char const*                l_text;
    unsigned char*                      l_value= 0;

    l_text= (*m_entry_value).get_text();
    if (l_text && l_text[0])
    {
        size_t                          l_len= 1+(*m_entry_value).get_text_length();
        l_value= reinterpret_cast<unsigned char*>(malloc(l_len));
        if (l_value)
        {
            memcpy(l_value, l_text, l_len);
        }
    }

    return l_value;
}
