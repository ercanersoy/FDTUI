/*
 GAPBUF.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __gapbuf_h__

#include <stddef.h>

#ifndef __FAR
#   if defined( __FLAT__) || defined(__LARGE__)
#       define __FAR
#   else
#       define __FAR __far
#   endif
#endif

struct gapbuf
{
    unsigned char __FAR*                m_block;
    size_t                              m_block_size;
    size_t                              m_start;
    size_t                              m_end;
};

#ifdef __cplusplus
extern "C" {
#endif

extern void
gapbuf_assign(
    struct gapbuf*const                 o_gap);

extern void
gapbuf_discharge(
    struct gapbuf*const                 io_gap);

extern int
gapbuf_backspace(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset);

extern int
gapbuf_insert(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset,
    unsigned char const                 i_char);

extern int
gapbuf_delete(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset);

extern unsigned char __FAR*
gapbuf_char_next(
    size_t*                             io_offset,
    struct gapbuf const*const           i_gap);

extern unsigned char __FAR*
gapbuf_char_prev(
    size_t*                             io_offset,
    struct gapbuf const*const           i_gap);

extern unsigned char __FAR*
gapbuf_line_next(
    size_t*                             io_offset,
    struct gapbuf const*const           i_gap);

extern unsigned char __FAR*
gapbuf_line_prev(
    size_t*                             io_offset,
    struct gapbuf const*const           i_gap);

extern int
gapbuf_overwrite(
    struct gapbuf*const                 io_gap,
    size_t const                        i_offset,
    unsigned char const                 i_char);

extern unsigned char const __FAR*
gapbuf_text(
    struct gapbuf *const                io_gap);

extern unsigned char __FAR*
gapbuf_ptr(
    struct gapbuf const*const           i_gap,
    size_t const                        i_offset);

#ifdef __cplusplus
}
#endif

#define __gapbuf_h__
#endif
