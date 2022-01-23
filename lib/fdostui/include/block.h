/*
 BLOCK.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __block_h__

#include "box.h"
#include <stdlib.h>
#if defined(__WATCOMC__) 
#include <malloc.h>
#endif

#ifndef __FAR
#   if defined(__FLAT__) || defined(__LARGE__)
#       define __FAR
#   else
#       define __FAR __far
#   endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void __FAR*
block_read(
    struct box const*const              i_box);

extern void
block_write(
    void const __FAR*                   i_block,
    struct box const*const              i_box);

extern void
block_free(
    void __FAR*                         i_block);

#if defined(__FLAT__) || defined(__LARGE__)
#   define block_free(_block) free((_block))
#else
#   define block_free(_block) _ffree((_block))
#endif

#ifdef __cplusplus
}
#endif

#define __block_h__
#endif
