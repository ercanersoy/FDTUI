/*
 RADIOBTN.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __radiobtn_hpp__

#include "checkbtn.hpp"

class radiobutton : public checkbutton
{

public:

    radiobutton(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~radiobutton();

    bool
    join(
        radiobutton*const               i_radiobutton);

protected:

    virtual void
    toggled();

private:

    radiobutton();
    
    radiobutton(
    const radiobutton&);

    radiobutton&
    operator=(radiobutton const&);

public:

    radiobutton*                        m_next;
    radiobutton*                        m_prev;

};

#define __radiobtn_hpp__
#endif
