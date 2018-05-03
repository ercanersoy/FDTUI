/*
 CHECKBTN.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __checkbtn_hpp__

#include "button.hpp"
#include "wm.hpp"

class checkbutton : public button
{

public:

    checkbutton(
        int const                       i_pos_x,
        int const                       i_pos_y,
        unsigned int const              i_len_x,
        unsigned int const              i_len_y);
    
    virtual
    ~checkbutton();

    void
    clear_state();

    bool
    get_state() const;

    void
    set_state();

protected:

private:

    checkbutton();
    
    checkbutton(
    const checkbutton&);

    checkbutton&
    operator=(checkbutton const&);

};

inline void
checkbutton::clear_state()
{

    if (m_state)
    {
        m_state= false;
        set_damage(DAMAGE_VALUE);
        wm_draw_widget(this);
    }

    return;
}

inline bool
checkbutton::get_state() const
{
    return m_state;
}

inline void
checkbutton::set_state()
{

    if (false == m_state)
    {
        m_state= true;
        set_damage(DAMAGE_VALUE);
        wm_draw_widget(this);
        toggled();
    }

    return;
}

#define __checkbtn_hpp__
#endif
