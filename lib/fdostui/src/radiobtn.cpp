/*
 RADIOBTN.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "radiobtn.hpp"

radiobutton::radiobutton(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    checkbutton(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_next(0),
    m_prev(0)
{
    m_type= TYPE_RADIO;
    return;
}
    
radiobutton::~radiobutton()
{
    return;
}

void
radiobutton::toggled()
{
#if defined(__CURSES__)
    int                                 l_cur_x;
    int                                 l_cur_y;
#endif
    radiobutton*                        l_btn;

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        getyx(stdscr, l_cur_y, l_cur_x);
    }
#endif

    do
    {

        l_btn= m_prev;

        do
        {

            if (0 == l_btn)
            {
                break;
            }

            (*l_btn).clear_state();

            l_btn= (*l_btn).m_prev;

        }while(1);

        l_btn= m_next;

        do
        {

            if (0 == l_btn)
            {
                break;
            }

            (*l_btn).clear_state();

            l_btn= (*l_btn).m_next;

        }while(1);

    }while(0);

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        ::move(l_cur_y, l_cur_x);
    }
#endif

    return;
}

bool
radiobutton::join(
    radiobutton*const                   i_radiobutton)
{
    bool                                l_joined;
    radiobutton*                        l_tail;

    l_joined= false;

    do
    {

        if (this == i_radiobutton)
        {
            break;
        }

        if ((*i_radiobutton).m_next || (*i_radiobutton).m_prev)
        {
            break;
        }

        if (0 == m_next)
        {
            m_next= i_radiobutton;
            (*i_radiobutton).m_prev= this;
            break;
        }

        l_tail= m_next;

        do
        {

            if (0 == (*l_tail).m_next)
            {
                (*l_tail).m_next= i_radiobutton;
                (*i_radiobutton).m_prev= l_tail;
                break;
            }

            l_tail= (*l_tail).m_next;

        }while(1);

        l_joined= 1;

    }while(0);

    return l_joined;
}
