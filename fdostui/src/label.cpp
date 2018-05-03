/*
 LABEL.HPP

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "label.hpp"
#include <ctype.h>

label::label(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y) :
    widget(i_pos_x, i_pos_y, i_len_x, i_len_y),
    m_label(0)
{
    m_flags&= ~widget::CAN_FOCUS;
    return;
}
    
label::~label()
{
    free(m_label);
    return;
}

unsigned char const*
chunk(
    size_t*                             o_size,
    unsigned char *                     io_block,
    size_t const                        i_block_size,
    unsigned char const*                i_chunk)
{
    unsigned char                       l_chr;

    *o_size= 0;
    *io_block= 0;

    do
    {

        if (0 == (*i_chunk))
        {
            break;
        }

        l_chr= *i_chunk++;
        *io_block++= l_chr;
        (*o_size)++;

        if (i_block_size <= *o_size ||
            ispunct(l_chr) ||
            isspace(l_chr) ||
            32 > l_chr)
        {
            break;
        }

    }while(1);

    return i_chunk;
}

void
label::draw() const
{
    size_t                              l_bytes;
    unsigned char                       l_buf[80];
    unsigned char const*                l_chunk;
    unsigned char                       l_str[80];
    unsigned int                        l_pos_y;
    size_t                              l_buf_bytes;
    size_t                              l_total;
  
    draw_fill_area(
        m_box.m_pos_x,
        m_box.m_pos_y,
        m_box.m_len_x,
        m_box.m_len_y,
        0,
        _skin_label.m_fcolor,
        _skin_label.m_bcolor);

    do
    {

        if (0 == m_label)
        {
            break;
        }

        l_bytes= strlen(reinterpret_cast<char*>(m_label));

        if (1 == m_box.m_len_y)
        {
            if (m_box.m_len_x < l_bytes)
            {
                l_bytes= m_box.m_len_x;
            }
        }

        if (m_box.m_len_x >= l_bytes) 
        {
            draw_puts(
                m_box.m_pos_x,
                m_box.m_pos_y,
                m_label,
                l_bytes,
                _skin_label.m_fcolor,
                _skin_label.m_bcolor);
            break;
        }

        l_buf_bytes= 0;
        l_chunk= m_label;
        l_pos_y= m_box.m_pos_y;

        do
        {

            l_chunk= chunk(&l_bytes, l_str, sizeof(l_str), l_chunk);

            l_total= l_bytes + l_buf_bytes;

            if ((0 == l_bytes) ||
                (sizeof(l_buf) <= l_total) ||
                (m_box.m_len_x <= l_total))
            {
                if (0 == l_buf_bytes)
                {
                    break;
                }
                draw_puts(
                    m_box.m_pos_x,
                    l_pos_y,
                    l_buf,
                    l_buf_bytes,
                    _skin_label.m_fcolor,
                    _skin_label.m_bcolor);
                memcpy(l_buf, l_str, l_bytes);
                l_buf_bytes= l_bytes;
                l_pos_y++;
                if (m_box.m_len_y <= l_pos_y)
                {
                    break;
                }
            }
            else
            {
                memcpy(l_buf+l_buf_bytes, l_str, l_bytes);
                l_buf_bytes+= l_bytes;
            }

        }while(1);

    }while(0);

    return;
}
