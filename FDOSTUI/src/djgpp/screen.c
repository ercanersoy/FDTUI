/*
 SCREEN.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#include "video.h"
#include "screen.h"
#include <sys/farptr.h>
#include <dpmi.h>
#include <go32.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define VIDEO_MAX_ROWS 50 /* maximum number of _video_rows */

static enum video_card                  video_card= 0;
static unsigned char                    video_restore_mode= 3;
static unsigned long                    video_addr= 0xb8000;
unsigned int                            _video_cols= 80;
unsigned int                            _video_rows= 25;
static unsigned long                    video_line_offsets[VIDEO_MAX_ROWS];

extern int
screen_init()
{
    int                                 l_exit;
    unsigned long                       l_offset;
    int                                 l_mode;
    __dpmi_regs                         l_regs;
    size_t                              l_row;

    l_exit= 0;

    do
    {

        video_card= video_detect();

        /* get current video mode */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x0f00;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);
        l_mode= 0xff & l_regs.x.ax;

        video_restore_mode= l_mode;

        if (3 != l_mode)
        {
            memset(&l_regs, 0, sizeof(l_regs));
            l_regs.x.ax= 0x03;
            __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

        }

        /* set active display page */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x0500; /* set page 0 */
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

        /* disable blinking for full background colors */
        /* works on EGA/VGA only */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x1003;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

        for (l_offset= video_addr, l_row= 0; VIDEO_MAX_ROWS > l_row; l_row++)
        {
            video_line_offsets[l_row]= l_offset;
            l_offset+= (2 * _video_cols);
        }

    }while(0);

    return l_exit;
}

extern void
screen_deinit()
{
    __dpmi_regs                         l_regs;

    memset(&l_regs, 0, sizeof(l_regs));
    l_regs.x.ax= video_restore_mode;
    __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

    return;
}

extern int
screen_set25lines()
{
    __dpmi_regs                         l_regs;

    memset(&l_regs, 0, sizeof(l_regs));
    l_regs.x.ax= 0x1202; /* 400 scan lines */
    l_regs.x.bx= 0x30;
    __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);
    
    memset(&l_regs, 0, sizeof(l_regs));
    l_regs.x.ax= 0x03;
    __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

    _video_rows= 25;

    return 0;
}

extern int
screen_set50lines()
{
    int                                 l_exit;
    __dpmi_regs                         l_regs;

    do
    {


        if (VIDEO_CARD_EGA > video_card)
        {
            l_exit= -1;
            break;
        }

        /*
          set vertical resolution
          al 12h 
          al vertical resolution
            00h 200 scan lines
            01h 350 scan lines
            02h 400 scan lines
        */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x1202; /* 400 scan lines */
        l_regs.x.bx= 0x30;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);
    
        /* set video mode */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x03;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);
    
        /* Load ROM 8x8 dbl dot patterns */
        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0x1112;
        __dpmi_simulate_real_mode_interrupt(0x10, &l_regs);

        _video_rows= 50;

    }while(0);

    return l_exit;
}

extern void 
screen_fill(
    screen_char_t const                 i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    unsigned long                       l_addr;
    size_t                              l_index;
    unsigned int                        l_word;
    size_t                              l_words; 

    l_word= i_bcolor;
    l_word<<= 4;
    l_word|= i_fcolor;
    l_word<<= 8;
    l_word|= i_char; 

    l_addr= video_addr;
    l_words= _video_rows * _video_cols;
    _farsetsel(_dos_ds);

    for (l_index= 0; l_words > l_index; l_index++)
    {
        _farnspokew(l_addr, l_word);
        l_addr++;
        l_addr++;
    }

    return;
}

extern void 
screen_fill_area(
    int const                           i_col,
    int const                           i_row,
    unsigned int const                  i_columns,
    unsigned int const                  i_rows,
    screen_char_t const                 i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    unsigned long                       l_addr;
    int                                 l_col;
    int                                 l_col_cur;
    unsigned int                        l_columns;
    int                                 l_row;
    unsigned int                        l_rows;
    int                                 l_row_cur;
    int                                 l_scratch;
    unsigned int                        l_word;

    l_col= i_col;
    l_columns= i_columns;
    l_row= i_row;
    l_rows= i_rows;

    do
    {

        if (0 == i_rows)
        {
            break;
        }

        if (0 > i_col)
        {
            l_scratch= l_columns - (0 - i_col);
            if (0 >= l_scratch)
            {
                break;
            }
            l_columns= l_scratch;
            l_col= 0;
        }

        if (l_col + l_columns > _video_cols)
        {
            l_columns= _video_cols - l_col;
        }

        if (0 == l_columns)
        {
            break;
        }

        if (0 > l_row)
        {
            l_scratch= l_rows - (0 - l_row);
            if (0 >= l_scratch)
            {
                break;
            }
            l_rows= l_scratch;
            l_row= 0;
        }

        l_word= i_bcolor;
        l_word<<= 4;
        l_word|= i_fcolor;
        l_word<<= 8;
        l_word|= i_char; 

        _farsetsel(_dos_ds);
        for (l_row_cur= 0; l_rows > l_row_cur; l_row_cur++)
        {
            l_addr= video_line_offsets[l_row_cur + l_row];
            l_addr+= (2 * l_col);
            for (l_col_cur= 0; l_columns > l_col_cur; l_col_cur++)
            {
                _farnspokew(l_addr, l_word);
                l_addr++;
                l_addr++;
            }
        }

    }while(0);

    return;
}

extern void 
screen_putc(
    int const                           i_col,
    int const                           i_row,
    screen_char_t const                 i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    unsigned long                       l_addr;
    int                                 l_col;
    int                                 l_col_cur;
    unsigned int                        l_columns;
    int                                 l_scratch;
    unsigned int                        l_word;

    l_col= i_col;
    l_columns= i_repeat_count;

    do
    {

        if (0 == l_columns)
        {
            break;
        }

        if (0 > i_row)
        {
            break;
        }

        if (_video_rows <= i_row)
        {
            break;
        }

        if (_video_cols <= i_col)
        {
            break;
        }

        if (0 > l_col)
        {
            l_scratch= l_columns - (0 - l_col);
            if (0 >= l_scratch)
            {
                break;
            }
            l_columns= l_scratch;
            l_col= 0;
        }
        
        if (_video_cols <= l_col + l_columns)
        {
            l_columns= _video_cols - l_col;
        }

        l_word= i_bcolor;
        l_word<<= 4;
        l_word|= i_fcolor;
        l_word<<= 8;
        l_word|= i_char; 

        l_addr= video_line_offsets[i_row];
        l_addr+= (2 * l_col);

        _farsetsel(_dos_ds);
        for (l_col_cur= 0; l_columns > l_col_cur; l_col_cur++)
        {
            _farnspokew(l_addr, l_word);
            l_addr++;
            l_addr++;
        }

    }while(0);

    return;
}

extern void 
screen_putvc(
    int const                           i_col,
    int const                           i_row,
    screen_char_t const                 i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    unsigned long                       l_addr;
    int                                 l_count;
    int                                 l_row;
    int                                 l_row_cur;
    unsigned int                        l_word;

    l_row= i_row;
    l_count= i_repeat_count;

    do
    {

        if (0 == l_count)
        {
            break;
        }

        if (0 > l_row)
        {
            l_count= i_repeat_count - (0 - i_row);
            if (0 >= l_count)
            {
                break;
            }
            l_row= 0;
        }

        if (_video_cols <= i_col)
        {
            break;
        }

        if (0 > i_col)
        {
            break;
        }

        l_word= i_bcolor;
        l_word<<= 4;
        l_word|= i_fcolor;
        l_word<<= 8;
        l_word|= i_char; 

        _farsetsel(_dos_ds);
        for (l_row_cur= l_row; l_count > l_row_cur; l_row_cur++)
        {
            l_addr= video_line_offsets[l_row_cur];
            l_addr+= (2 * i_col);
            _farnspokew(l_addr, l_word);
        }

    }while(0);

    return;
}

extern void 
screen_puts(
    int const                           i_col,
    int const                           i_row,
    unsigned char const*                i_string,
    unsigned int const                  i_length,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    unsigned long                       l_addr;
    unsigned int                        l_attr;
    int                                 l_col;
    int                                 l_col_cur;
    unsigned int                        l_length;
    unsigned char const*                l_ptr;
    unsigned int                        l_word;

    l_col= i_col;
    l_length= i_length;
    l_ptr= i_string;

    do
    {

        if (0 == i_length)
        {
            break;
        }

        if (0 > i_row)
        {
            break;
        }

        if (_video_rows <= i_row)
        {
            break;
        }

        if (_video_cols <= i_col)
        {
            break;
        }

        if (0 > l_col)
        {
            do
            {
                l_ptr++;
                l_length--;
                l_col++;

                if (0 == l_length)
                {
                    break;
                }

                if (0 == l_col)
                {
                    break;
                }

            }while(1);
        }

        if (0 == l_length)
        {
            break;
        }

        if (_video_cols <= l_col+l_length)
        {
            l_length= _video_cols - l_col;
        }

        l_attr= i_bcolor;
        l_attr<<= 4;
        l_attr|= i_fcolor;
        l_attr<<= 8;

        l_addr= video_line_offsets[i_row];
        l_addr+= (2 * l_col);

        _farsetsel(_dos_ds);
        for (l_col_cur= 0; l_length > l_col_cur; l_col_cur++)
        {
            l_word= l_attr;
            l_word|= *l_ptr;
            _farnspokew(l_addr, l_word);
            l_ptr++;
            l_addr++;
            l_addr++;
        }

    }while(0);

    return;
}

extern void 
screen_set_color(
    int const                           i_col,
    int const                           i_row,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
{
    unsigned long                       l_addr;
    int                                 l_col;
    int                                 l_col_cur;
    unsigned int                        l_columns;
    int                                 l_scratch;
    unsigned int                        l_word;

    l_col= i_col;
    l_columns= i_repeat_count;

    do
    {

        if (0 == l_columns)
        {
            break;
        }

        if (0 > i_row)
        {
            break;
        }

        if (_video_rows <= i_row)
        {
            break;
        }

        if (_video_cols <= i_col)
        {
            break;
        }

        if (0 > l_col)
        {
            l_scratch= l_columns - (0 - l_col);
            if (0 >= l_scratch)
            {
                break;
            }
            l_columns= l_scratch;
            l_col= 0;
        }
        
        if (_video_cols <= l_col + l_columns)
        {
            l_columns= _video_cols - l_col;
        }

        l_word= i_bcolor;
        l_word<<= 4;
        l_word|= i_fcolor;

        l_addr= video_line_offsets[i_row];
        l_addr+= (2 * l_col);

        _farsetsel(_dos_ds);
        for (l_col_cur= 0; l_columns > l_col_cur; l_col_cur++)
        {
            l_addr++;
            _farnspokeb(l_addr, l_word);
            l_addr++;
        }

    }while(0);

    return;
}

extern screen_block_t
screen_getc(
    int const                           i_col,
    int const                           i_row)
{
    unsigned long                       l_addr;
    screen_block_t                      l_word;

    l_word= 0;

    do
    {

        if (0 > i_col)
        {
            break;
        }

        if (_video_cols <= i_col)
        {
            break;
        }

        if (0 > i_row)
        {
            break;
        }

        if (_video_rows <= i_row)
        {
            break;
        }

        l_addr= video_line_offsets[i_row];
        l_addr+= (2 * i_col);
        l_word= _farpeekw(_dos_ds, l_addr);

    }while(0);

    return l_word;
}

extern screen_block_t __FAR*
screen_read(
    screen_block_t __FAR*               o_block,
    unsigned int const                  i_columns,
    int const                           i_col,
    int const                           i_row)
{
    screen_block_t __FAR*               l_ptr;
    unsigned long                       l_addr;
    unsigned int                        l_col;

    l_ptr= o_block;
    l_addr= video_line_offsets[i_row];
    l_addr+= (2 * i_col);

    _farsetsel(_dos_ds);
    for (l_col= 0; i_columns > l_col; l_col++)
    {
        *l_ptr++= _farnspeekw(l_addr);
        l_addr++;
        l_addr++;
    }

    return l_ptr;
}

extern screen_block_t const __FAR*
screen_write(
    int const                           i_col,
    int const                           i_row,
    screen_block_t const __FAR*         i_block,
    unsigned int const                  i_columns)
{
    unsigned long                       l_addr;
    unsigned int                        l_col;
    screen_block_t const __FAR*         l_ptr;

    l_ptr= i_block;
    l_addr= video_line_offsets[i_row];
    l_addr+= (2 * i_col);

    _farsetsel(_dos_ds);
    for (l_col= 0; i_columns > l_col; l_col++)
    {
        _farnspokew(l_addr, *l_ptr++);
        l_addr++;
        l_addr++;
    }

    return l_ptr;
}
