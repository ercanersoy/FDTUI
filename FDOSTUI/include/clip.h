/*
 CLIP.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __clip_h__

#include "box.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void
clip(
    struct box*const                    io_area,
    struct box const*const              i_clip);

extern void
clip_cleanup();

extern void
clip_get(
    struct box*const                    o_box);

extern void
clip_push(
    struct box const*const              i_box);

extern void
clip_pop();

#ifdef __cplusplus
}
#endif

#define __clip_h__
#endif
