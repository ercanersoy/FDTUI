/*
 CURSOR.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#ifndef __cursor_h__

#if defined(__CURSES__)

#include <curses.h>

enum cursor_features
{
    CURSOR_CAN_HIDE                     = (1 << 0)
};

#endif

enum cursor_mode
{
    CURSOR_OVERWRITE                    =0,
    CURSOR_INSERT                       =1
};

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CURSES__)
extern unsigned int                     _cursor_features;
#endif

extern enum cursor_mode                 _cursor_mode;

extern void
cursor_set_position(
    unsigned char const                 i_row,
    unsigned char const                 i_column);

extern void
cursor_hide();

extern void
cursor_underline();

extern void
cursor_block();

#ifdef __cplusplus
}
#endif

#if defined(__DOS__) && !defined(__CURSES__)

#ifdef __cplusplus
extern "C" {
#endif

extern void
cursor_set_type(
    char const                          i_scan_start,
    char const                          i_scan_end);

extern void
cursor_get_position(
    char*const                          o_scan_start,
    char*const                          o_scan_end,
    char*const                          o_row,
    char*const                          o_column);

#ifdef __cplusplus
}
#endif

#pragma aux cursor_set_position =\
    "xor bh, bh"\
    "mov ah, 02h"\
    "int 010h"\
    modify [AH BH]\
    parm [DL] [DH];

#pragma aux cursor_set_type= \
    "mov ah, 01h"\
    "int 010h" \
    modify [ AH ] \
    parm [CH] [CL] ;

#if defined(__FLAT__)

#pragma aux cursor_get_position=\
    "push ebx"\
    "push eax"\
    "xor bh,bh"\
    "mov ah, 03h"\
    "int 010h"\
    "pop ebx"\
    "mov [byte ptr ebx], ch"\
    "pop ebx"\
    "mov [byte ptr ebx], cl"\
    "mov [byte ptr esi], dh"\
    "mov [byte ptr edi], dl"\
    modify [ AH ] \
    parm [AX] [BX] [SI] [DI];

#else

#pragma aux cursor_get_position=\
    "push bx"\
    "push ax"\
    "xor bh,bh"\
    "mov ah, 03h"\
    "int 010h"\
    "pop bx"\
    "mov [byte ptr bx], ch"\
    "pop bx"\
    "mov [byte ptr bx], cl"\
    "mov [byte ptr ds:si], dh"\
    "mov [byte ptr ds:di], dl"\
    modify [ AH ] \
    parm [AX] [BX] [SI] [DI];

#endif /* #ifdef __FLAT__ */

#define CURSOR_MASK_HIDE                0x20

#define cursor_hide()                   cursor_set_type(CURSOR_MASK_HIDE, 0x0)

#define cursor_underline()              cursor_set_type(0x06, 0x07)

#define cursor_block()                  cursor_set_type(0x00, 0x07)

#endif /* #if defined(__DOS__) && !defined(__CURSES__) */

#define __cursor_h__
#endif
