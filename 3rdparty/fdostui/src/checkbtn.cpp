/*
 CHECKBTN.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "checkbtn.hpp"

checkbutton::checkbutton(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    button(i_pos_x, i_pos_y, i_len_x, i_len_y)
{
    m_type= TYPE_CHECK;
    return;
}
    
checkbutton::~checkbutton()
{
    return;
}
