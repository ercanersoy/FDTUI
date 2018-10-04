/*
 POLL.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __poll_h__

#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif

enum poll_event_type
{
    EVENT_KEY,
    EVENT_MOUSE
#ifndef __DOS__
    ,EVENT_SCREEN_RESIZE
#endif
};

union poll_event_record
{
    struct event_key                    m_key;
    struct event_mouse                  m_mouse;
#ifndef __DOS__
    struct event_resize                 m_resize;
#endif
};

struct poll_event
{
    enum poll_event_type                m_type;
    union poll_event_record             m_record;
};

extern void
poll_keyboard_unget(
    struct event_key const*const        i_event);

extern int
poll_block(
    struct poll_event *const            o_event);

#ifdef __cplusplus
}
#endif

#define __poll_h__
#endif
