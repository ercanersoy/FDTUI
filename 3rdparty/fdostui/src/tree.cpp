/*
 TREE.CPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "tree.hpp"
#include "wm.hpp"
#include "skin.h"
#include "matrix.h"
#include "clip.h"
#include "mouse.h"
#include "draw.h"

tree::tree(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_root(0),
    m_selected(0),
    m_signal_selected(0),
    m_horizontal(i_pos_x+1, i_pos_y+i_len_y-1, i_len_x-2, 1),
    m_vertical(i_pos_x+i_len_x-1, i_pos_y+1, 1, i_len_y-2)
{

    m_horizontal.set_debounce(true);
    m_horizontal.set_orientation(slider::HORIZONTAL);
    m_horizontal.set_range(0, 0);
    m_horizontal.set_user_data(static_cast<void*>(this));
    m_horizontal.set_hidden();

    m_vertical.set_debounce(true);
    m_vertical.set_orientation(slider::VERTICAL);
    m_vertical.set_range(0, 0);
    m_vertical.set_user_data(static_cast<void*>(this));
    m_vertical.set_hidden();

    return;
}
    
tree::~tree()
{

    delete m_root;

    return;
}

int
tree::draw(
    treeitem*const                      io_item,
    int const                           i_pos_x,
    int const                           i_pos_y) const
{
    treeitem*                           l_item;
    int                                 l_pos_x;
    int                                 l_pos_y;
    unsigned int                        l_slot;
    unsigned char                       l_symbol;
    enum foreground                     l_fcolor;
    enum background                     l_bcolor;

    l_pos_x= i_pos_x;
    l_pos_y= i_pos_y;
    (*io_item).m_pos_y= i_pos_y;

    if (m_selected != io_item)
    {
        l_fcolor= _skin_tree.m_fcolor;
        l_bcolor= _skin_tree.m_bcolor;
    }
    else
    {
        l_fcolor= _skin_tree.m_selected_fcolor;
        l_bcolor= _skin_tree.m_selected_bcolor;
    }

    if ((*io_item).m_child.m_taken)
    {
        l_symbol= ((*io_item).m_open) ? 
            _skin_buttons.m_collapse : _skin_buttons.m_expand;
        draw_putc(
            l_pos_x,
            l_pos_y,
            l_symbol,
            1,
            _skin_tree.m_fcolor,
            _skin_tree.m_bcolor);
    }

    l_pos_x++;

    if ((*io_item).m_text)
    {
        draw_puts(
            l_pos_x,
            l_pos_y,
            (*io_item).m_text,
            (*io_item).m_len,
            l_fcolor,
            l_bcolor);
    }

    l_pos_y++;

    if ((*io_item).m_open)
    {
        for (l_slot= 0; (*io_item).m_child.m_taken > l_slot; l_slot++)
        {
            l_item= reinterpret_cast<treeitem*>(
                (*io_item).m_child.m_base[l_slot]);
            l_pos_y= draw(l_item, l_pos_x+1, l_pos_y);
        }
    }

    return l_pos_y;
}

void
tree::draw() const
{
    struct box                          l_clip;
    struct box                          l_box;
    int                                 l_pos_y;
    int                                 l_pos_x;

    l_pos_y= 0;
    l_pos_x= 0;

    mouse_hide();
    screen_push(1);

    draw_fill_area(
        m_box.m_pos_x,
        m_box.m_pos_y,
        m_box.m_len_x,
        m_box.m_len_y,
        0,
        _skin_tree.m_fcolor,
        _skin_tree.m_bcolor);

    draw_frame(
        &m_box,
        &_skin_frame_single,
        _skin_tree.m_fcolor,
        _skin_tree.m_bcolor);

    do
    {

        if (0 == m_root)
        {
            break;
        }

        if (m_horizontal.get_visible())
        {
            l_pos_x= -static_cast<int>(m_horizontal.get_value());
        }

        if (m_vertical.get_visible())
        {
            l_pos_y= -static_cast<int>(m_vertical.get_value());
        }

        l_clip.m_pos_x= m_box.m_pos_x+1;
        l_clip.m_pos_y= m_box.m_pos_y+1;
        l_clip.m_len_x= m_box.m_len_x-2;
        l_clip.m_len_y= m_box.m_len_y-2;
        matrix_transform(&l_clip.m_pos_x, &l_clip.m_pos_y);
        clip_get(&l_box);
        clip(&l_clip, &l_box);
        clip_push(&l_clip);
        draw(m_root, l_pos_x+1, l_pos_y+1);
        clip_pop();

        if (m_horizontal.get_visible())
        {
            m_horizontal.draw();
        }

        if (m_vertical.get_visible())
        {
            m_vertical.draw();
        }

    }while(0);

    screen_pop();
    mouse_show();

#if defined(__CURSES__)
    if (0 == (CURSOR_CAN_HIDE & _cursor_features))
    {
        int                             l_cur_x;
        int                             l_cur_y;
        translate_screen(&l_cur_x, &l_cur_y);
        l_cur_y+= (*m_selected).m_pos_y;
        ::move(l_cur_y, l_cur_x+1);
    }
#endif

    return;
}

void
tree::scrollbar_configure(
    unsigned int&                       io_height,
    unsigned int&                       io_width,
    treeitem const&                     i_item,
    unsigned int const                  i_pos_x)
{
    treeitem const*                     l_item;
    unsigned int                        l_len;
    unsigned int                        l_pos_x;
    unsigned int                        l_slot;

    l_pos_x= i_pos_x + 2;

    do
    {

        io_height++;
        l_len= i_item.m_len + l_pos_x;

        if (l_len > io_width)
        {
            io_width= l_len;
        }

        if (0 == i_item.m_open)
        {
            break;
        }

        for (l_slot=0; i_item.m_child.m_taken > l_slot; l_slot++)
        {
            l_item= reinterpret_cast<treeitem*>(i_item.m_child.m_base[l_slot]);
            scrollbar_configure(io_height, io_width, *l_item, l_pos_x);
        }

    }while(0);

    return;
}

void
tree::scrollbar_configure()
{
    unsigned int                        l_height;
    unsigned int                        l_width;

    l_height= 0;
    l_width= 0;

    if (m_root)
    {
        scrollbar_configure(l_height, l_width, *m_root, 0);
    }

    if ((m_box.m_len_y - 2) >= l_height)
    {
        m_vertical.set_hidden();
    }
    else
    {
        l_height-= m_box.m_len_y - 2;
        m_vertical.set_range(l_height, 0);
        m_vertical.set_visible();
    }

    if ((m_box.m_len_x - 2) > l_width)
    {
        m_horizontal.set_hidden();
    }
    else
    {
        l_width-= m_box.m_len_x - 2;
        m_horizontal.set_range(0, l_width);
        m_horizontal.set_visible();
    }

    return;
}

bool
tree::event_hit(
    treeitem*const                      io_item,
    unsigned int const                  i_pos_y)
{
    bool                                l_hit;
    treeitem*                           l_item;
    unsigned int                        l_slot;

    do
    {

        l_hit= (static_cast<int>(i_pos_y) == (*io_item).m_pos_y);

        if (l_hit)
        {
            m_selected= io_item;

            if ((*io_item).m_child.m_taken)
            {
                (*io_item).m_open= !(*io_item).m_open;
                scrollbar_configure();
            }
            else
            {
                emit_selected();
            }

            break;
        }

        if (0 == (*io_item).m_open)
        {
            break;
        }

        for (l_slot=0; 
            false == l_hit && (*io_item).m_child.m_taken > l_slot;
            l_slot++)
        {
            l_item= reinterpret_cast<treeitem*>(
                (*io_item).m_child.m_base[l_slot]);
            l_hit= event_hit(l_item, i_pos_y);
        }

    }while(0);

    return l_hit;
}

void
tree::key_down()
{
    treeitem*                           l_next;

    l_next= (*m_selected).next();

    if (l_next)
    {

        if (static_cast<int>((m_box.m_pos_y + m_box.m_len_y - 2)) < 
            (*l_next).m_pos_y)
        {
            m_vertical.set_value(
                m_vertical.clamp(
                    m_vertical.increment(m_vertical.get_value(), -1.0)));
        }

        if (0 == (*l_next).m_child.m_taken)
        {
            emit_selected();
        }

        m_selected= l_next;
        wm_draw_widget(this);
    }

    return;
}

void
tree::key_up()
{
    treeitem*                           l_prev;

    l_prev= (*m_selected).prev();

    if (l_prev)
    {

        if (0 >= (*l_prev).m_pos_y)
        {
            m_vertical.set_value(
                m_vertical.clamp(
                    m_vertical.increment(m_vertical.get_value(), 1.0)));
        }

        if (0 == (*l_prev).m_child.m_taken)
        {
            emit_selected();
        }

        m_selected= l_prev;
        wm_draw_widget(this);
    }

    return;
}

enum event_response
tree::event_key(
    struct event_key const&             i_event)
{
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    do
    {

        if (0 == m_root)
        {
            break;
        }

        if (ASCII_ENTER == i_event.m_ascii)
        {
            if ((*m_selected).m_child.m_taken)
            {
                (*m_selected).m_open= !(*m_selected).m_open;
                scrollbar_configure();
                wm_draw_widget(this);
            }
            l_response= RESPONSE_HANDLED;
            break;
        }

        if (SCAN_DOWN == i_event.m_scan)
        {
            key_down();
            l_response= RESPONSE_HANDLED;
            break;
        }

        if (SCAN_UP == i_event.m_scan)
        {
            key_up();
            l_response= RESPONSE_HANDLED;
            break;
        }

    }while(0);

    return l_response;
}
    
enum event_response
tree::event_mouse(
    struct event_mouse const&           i_event)
{
    bool                                l_hit;
    enum event_response                 l_response;

    l_response= RESPONSE_NONE;

    do
    {

        if (0 == m_root)
        {
            break;
        }

        if (i_event.m_pos_y == (m_box.m_pos_y + m_box.m_len_y - 1))
        {
            if (m_horizontal.get_visible())
            {
                l_response= m_horizontal.event_mouse(i_event);
            }
            break;
        }

        if (i_event.m_pos_x == (m_box.m_pos_x + m_box.m_len_x - 1))
        {
            if (m_vertical.get_visible())
            {
                l_response= m_vertical.event_mouse(i_event);
            }
            break;
        }

        if (MOUSE_BUTTON_CLICKED != i_event.m_state)
        {
            break;
        }

        l_hit= event_hit(m_root, i_event.m_pos_y);

        if (l_hit)
        {
            l_response= RESPONSE_HANDLED;
        }

    }while(0);

    if (RESPONSE_HANDLED == l_response)
    {
        wm_draw_widget(this);
    }

    return l_response;
}

void
tree::move(
    int const                           i_pos_x,
    int const                           i_pos_y)
{

    widget::move(i_pos_x, i_pos_y);
    m_horizontal.move(m_box.m_pos_x+1, m_box.m_pos_y+m_box.m_len_y-1);
    m_vertical.move(m_box.m_pos_x+m_box.m_len_x-1, m_box.m_pos_y+1);

    return;
}

void
tree::resize(
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y)
{

    widget::resize(i_len_x, i_len_y);
    m_horizontal.resize(m_box.m_len_x-2, 1);
    m_vertical.resize(1, m_box.m_len_y-2);

    return;
}
