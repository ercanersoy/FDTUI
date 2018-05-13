/*
 WIDGET.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "group.hpp"
#include "wm.hpp"

widget::widget(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    m_damage(0),
    m_flags(widget::ENABLED|
        widget::VISIBLE|
        widget::CAN_FOCUS|
        widget::CAN_CLOSE),
    m_box(),
    m_parent(0),
    m_user_data(0)
{

    m_box.m_pos_x= i_pos_x;
    m_box.m_pos_y= i_pos_y;
    m_box.m_len_x= i_len_x;
    m_box.m_len_y= i_len_y;

    return;
}

widget::~widget()
{
    return;
}

bool
widget::close(
    bool const                          i_shutting_down)
{
    bool                                l_can_close= true;

    if (false == i_shutting_down && 0 == (CAN_CLOSE & m_flags))
    {
        if (get_visible())
        {
            set_hidden();
            wm_draw(0);
        }
        l_can_close= false;
    }

    return l_can_close;
}

void
widget::translate_screen(
    int*const                           o_pos_x,
    int*const                           o_pos_y) const
{
    group const*                        l_parent;

    (*o_pos_x)= get_pos_x();
    (*o_pos_y)= get_pos_y();

    do
    {

        l_parent= get_parent();

        do
        {

            if (0 == l_parent)
            {
                break;
            }

            (*o_pos_x)+= (*l_parent).get_pos_x()+1;
            (*o_pos_y)+= (*l_parent).get_pos_y()+1;

            l_parent= (*l_parent).get_parent();

        }while(1);

    }while(0);

    return;
}
