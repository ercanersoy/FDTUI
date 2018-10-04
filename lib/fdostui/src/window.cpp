/*
 WINDOW.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "window.hpp"
#include "wmhelper.hpp"

window::window(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    group(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_title(0),
    m_attributes(
	static_cast<enum attributes>(
        (window::BORDER|
        window::TITLE|
        window::MINMAX|
        window::RESIZE|
        window::SYSTEM)))
{
    return;
}

window::~window()
{

    free(m_title);

    return;
}

void
window::draw() const
{
    
    wmhelper::draw_decorate(m_box, m_attributes, m_title);
    group::draw();

    return;
}
