/*
 MOUSE.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "mouse.h"
#include <sys/farptr.h>
#include <dpmi.h>
#include <go32.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

short int                               mouse_initialized= 0;

#define MOUSE_QUEUE_SIZE                24

struct  mouse_buffer
{
    unsigned int                        qstart;
    unsigned int                        qend;
    unsigned int                        qactive;
    struct mouse_event                  queue[MOUSE_QUEUE_SIZE];
};

static struct mouse_buffer              buffer= {0,0,0};

struct mouse_callback
{
    __dpmi_regs                         regs;
    _go32_dpmi_seginfo                  info;
};

static struct mouse_callback            callback;

static void
mouse_handler(
    __dpmi_regs*                        io_regs)
{
    struct mouse_event*                 l_event;

    l_event= &buffer.queue[buffer.qend];

    (*l_event).m_btn_state= (*io_regs).x.bx;
    (*l_event).m_curs_col= (*io_regs).x.cx;
    (*l_event).m_curs_row= (*io_regs).x.dx;

    buffer.qend= (buffer.qend + 1) % MOUSE_QUEUE_SIZE;

    if (buffer.qactive < MOUSE_QUEUE_SIZE)
    {
        buffer.qactive++;
    } else {
        buffer.qstart = (buffer.qstart + 1) % MOUSE_QUEUE_SIZE;
    }

}

extern int
mouse_get_event(
    struct mouse_event __FAR*           o_event)
{
    int                                 l_exit;
    struct mouse_event*                 l_event;

    if (buffer.qactive)
    {
        l_event= &buffer.queue[buffer.qstart];
        (*o_event).m_btn_state= (*l_event).m_btn_state;
        (*o_event).m_curs_col= (*l_event).m_curs_col;
        (*o_event).m_curs_row= (*l_event).m_curs_row;
        buffer.qstart = (buffer.qstart + 1) % MOUSE_QUEUE_SIZE;
        buffer.qactive--;
        l_exit= 1;
    }
    else
    {
        (*o_event).m_btn_state= 0;
        (*o_event).m_curs_col= 0;
        (*o_event).m_curs_row= 0;
        l_exit= 0;
    }

    return l_exit;
}

extern int
mouse_init()
{
    int                                 l_exit;
    __dpmi_regs                         l_regs;
    int                                 l_rc;

    l_exit= -1;
    mouse_initialized= 0;
    memset(&buffer, 0, sizeof(buffer));
    memset(&callback, 0, sizeof(callback));

    do
    {

        memset(&l_regs, 0, sizeof(l_regs));
        __dpmi_int(0x33, &l_regs);

        if (0 == l_regs.x.ax)
        {
            break;
        }

        _go32_dpmi_lock_data(&callback, sizeof(&callback));
        _go32_dpmi_lock_data(&buffer, sizeof(buffer));
        _go32_dpmi_lock_code(
            (void*)(long)&mouse_handler,
            (long)mouse_get_event - (long)mouse_handler);

        callback.info.pm_selector= _my_cs();
        callback.info.pm_offset= (long)mouse_handler;

        l_rc= _go32_dpmi_allocate_real_mode_callback_retf(
            &callback.info,
            &callback.regs);

        if (l_rc)
        {
            break;
        }

        memset(&l_regs, 0, sizeof(l_regs));
        l_regs.x.ax= 0xc;
        l_regs.x.cx= 0xf;
        l_regs.x.es= callback.info.rm_segment;
        l_regs.x.dx= callback.info.rm_offset;
        __dpmi_int(0x33, &l_regs);

        l_exit= 0;
        mouse_initialized= 1;

    }while(0);

    return l_exit;
}

extern void
mouse_deinit()
{
    __dpmi_regs                         l_regs;

    memset(&l_regs, 0, sizeof(l_regs));
    __dpmi_int(0x33, &l_regs);

    if (mouse_initialized)
    {
        _go32_dpmi_free_real_mode_callback(&callback.info);
    }

    mouse_initialized= 0;

    return;

}
extern void
mouse_show()
{
    __dpmi_regs                         l_regs;

    if (mouse_initialized)
    {
	memset(&l_regs, 0, sizeof(l_regs));
	l_regs.x.ax= 0x1;
	__dpmi_int(0x33, &l_regs);
    }

    return;
}

extern void
mouse_hide()
{
    __dpmi_regs                         l_regs;

    if (mouse_initialized)
    {
	memset(&l_regs, 0, sizeof(l_regs));
	l_regs.x.ax= 0x2;
	__dpmi_int(0x33, &l_regs);
    }

    return;
}
