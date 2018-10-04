/*
 KEYBOARD.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright 
 and related or neighboring rights to FDOSTUI Library. This work is published 
 from: United States.
*/
#ifndef __keyboard_h__

#if defined(__CURSES__)
#include <curses.h>
#endif

enum asciicodes
{
    ASCII_BS                            = 0x08,
    ASCII_TAB                           = 0x09,
    ASCII_ENTER                         = 0x0d,
    ASCII_ESC                           = 0x1b,
    ASCII_SPACE                         = 0x20
};

#if (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__)
enum scancodes
{
    SCAN_NONE                           =0x00,
    SCAN_F1                             =0x3b,
    SCAN_F2                             =0x3c,
    SCAN_F3                             =0x3d,
    SCAN_F4                             =0x3e,
    SCAN_F5                             =0x3f,
    SCAN_F6                             =0x40,
    SCAN_F7                             =0x41,
    SCAN_F8                             =0x42,
    SCAN_F9                             =0x43,
    SCAN_F10                            =0x44,
    SCAN_HOME                           =0x47,
    SCAN_UP                             =0x48,
    SCAN_PGUP                           =0x49,
    SCAN_LEFT                           =0x4b,
    SCAN_RIGHT                          =0x4d,
    SCAN_END                            =0x4f,
    SCAN_DOWN                           =0x50,
    SCAN_PGDN                           =0x51,
    SCAN_INS                            =0x52,
    SCAN_DEL                            =0x53,
    SCAN_SHIFT_TAB                      =0xf,
    SCAN_SHIFT_F1                       =0x54,
    SCAN_SHIFT_F2                       =0x55,
    SCAN_SHIFT_F3                       =0x56,
    SCAN_SHIFT_F4                       =0x57,
    SCAN_SHIFT_F5                       =0x58,
    SCAN_SHIFT_F6                       =0x59,
    SCAN_SHIFT_F7                       =0x5a,
    SCAN_SHIFT_F8                       =0x5b,
    SCAN_SHIFT_F9                       =0x5c,
    SCAN_SHIFT_F10                      =0x5d,
    SCAN_ALT_1                          =0x78,
    SCAN_ALT_2                          =0x79,
    SCAN_ALT_3                          =0x7A,
    SCAN_ALT_4                          =0x7B,
    SCAN_ALT_5                          =0x7C,
    SCAN_ALT_6                          =0x7D,
    SCAN_ALT_7                          =0x7E,
    SCAN_ALT_8                          =0x7F,
    SCAN_ALT_9                          =0x80,
    SCAN_ALT_0                          =0x81,
    SCAN_ALT_MINUS                      =0x82,
    SCAN_ALT_PLUS                       =0x83,
    SCAN_ALT_Q                          =0x10,
    SCAN_ALT_W                          =0x11,
    SCAN_ALT_E                          =0x12,
    SCAN_ALT_R                          =0x13,
    SCAN_ALT_T                          =0x14,
    SCAN_ALT_Y                          =0x15,
    SCAN_ALT_U                          =0x16,
    SCAN_ALT_I                          =0x17,
    SCAN_ALT_O                          =0x18,
    SCAN_ALT_P                          =0x19,
    SCAN_ALT_A                          =0x1e,
    SCAN_ALT_S                          =0x1f,
    SCAN_ALT_D                          =0x20,
    SCAN_ALT_F                          =0x21,
    SCAN_ALT_G                          =0x22,
    SCAN_ALT_H                          =0x23,
    SCAN_ALT_J                          =0x24,
    SCAN_ALT_K                          =0x25,
    SCAN_ALT_L                          =0x26,
    SCAN_ALT_Z                          =0x2c,
    SCAN_ALT_X                          =0x2d,
    SCAN_ALT_C                          =0x2e,
    SCAN_ALT_V                          =0x2f,
    SCAN_ALT_B                          =0x30,
    SCAN_ALT_N                          =0x31,
    SCAN_ALT_M                          =0x32,
    SCAN_ALT_F1                         =0x68,
    SCAN_ALT_F2                         =0x69,
    SCAN_ALT_F3                         =0x6a,
    SCAN_ALT_F4                         =0x6b,
    SCAN_ALT_F5                         =0x6c,
    SCAN_ALT_F6                         =0x6d,
    SCAN_ALT_F7                         =0x6e,
    SCAN_ALT_F8                         =0x6f,
    SCAN_ALT_F9                         =0x70,
    SCAN_ALT_F10                        =0x71,
    SCAN_CTL_F1                         =0x5e,
    SCAN_CTL_F2                         =0x5f,
    SCAN_CTL_F3                         =0x60,
    SCAN_CTL_F4                         =0x61,
    SCAN_CTL_F5                         =0x62,
    SCAN_CTL_F6                         =0x63,
    SCAN_CTL_F7                         =0x64,
    SCAN_CTL_F8                         =0x65,
    SCAN_CTL_F9                         =0x66,
    SCAN_CTL_F10                        =0x67,
    SCAN_CTL_HOME                       =0x77,
    SCAN_CTL_PGUP                       =0x84,
    SCAN_CTL_LEFT                       =0x73,
    SCAN_CTL_RIGHT                      =0x74,
    SCAN_CTL_END                        =0x75,
    SCAN_CTL_PGDN                       =0x76
}; 

#endif /* #if defined(__DOS__) && !defined(__CURSES__) */

#if defined(__NT__) && !defined(__CURSES__)
#include <windows.h>
enum scancodes
{
    SCAN_NONE                           =0x00,
    SCAN_F1                             =VK_F1,
    SCAN_F2                             =VK_F2,
    SCAN_F3                             =VK_F3,
    SCAN_F4                             =VK_F4,
    SCAN_F5                             =VK_F5,
    SCAN_F6                             =VK_F6,
    SCAN_F7                             =VK_F7,
    SCAN_F8                             =VK_F8,
    SCAN_F9                             =VK_F9,
    SCAN_F10                            =VK_F10,
    SCAN_HOME                           =VK_HOME,
    SCAN_UP                             =VK_UP,
    SCAN_PGUP                           =VK_PRIOR,
    SCAN_LEFT                           =VK_LEFT,
    SCAN_RIGHT                          =VK_RIGHT,
    SCAN_END                            =VK_END,
    SCAN_DOWN                           =VK_DOWN,
    SCAN_PGDN                           =VK_NEXT,
    SCAN_INS                            =VK_INSERT,
    SCAN_DEL                            =VK_DELETE,
    SCAN_SHIFT_TAB                      =0xf,
    SCAN_ALT_MASK                       =1000,
    SCAN_ALT_1                          =('1' | SCAN_ALT_MASK),
    SCAN_ALT_2                          =('2' | SCAN_ALT_MASK),
    SCAN_ALT_3                          =('3' | SCAN_ALT_MASK),
    SCAN_ALT_4                          =('4' | SCAN_ALT_MASK),
    SCAN_ALT_5                          =('5' | SCAN_ALT_MASK),
    SCAN_ALT_6                          =('6' | SCAN_ALT_MASK),
    SCAN_ALT_7                          =('7' | SCAN_ALT_MASK),
    SCAN_ALT_8                          =('8' | SCAN_ALT_MASK),
    SCAN_ALT_9                          =('9' | SCAN_ALT_MASK),
    SCAN_ALT_0                          =('0' | SCAN_ALT_MASK),
    SCAN_ALT_MINUS                      =('-' | SCAN_ALT_MASK),
    SCAN_ALT_PLUS                       =('=' | SCAN_ALT_MASK),
    SCAN_ALT_Q                          =('q' | SCAN_ALT_MASK),
    SCAN_ALT_W                          =('w' | SCAN_ALT_MASK),
    SCAN_ALT_E                          =('e' | SCAN_ALT_MASK),
    SCAN_ALT_R                          =('r' | SCAN_ALT_MASK),
    SCAN_ALT_T                          =('t' | SCAN_ALT_MASK),
    SCAN_ALT_Y                          =('y' | SCAN_ALT_MASK),
    SCAN_ALT_U                          =('u' | SCAN_ALT_MASK),
    SCAN_ALT_I                          =('i' | SCAN_ALT_MASK),
    SCAN_ALT_O                          =('o' | SCAN_ALT_MASK),
    SCAN_ALT_P                          =('p' | SCAN_ALT_MASK),
    SCAN_ALT_A                          =('a' | SCAN_ALT_MASK),
    SCAN_ALT_S                          =('s' | SCAN_ALT_MASK),
    SCAN_ALT_D                          =('d' | SCAN_ALT_MASK),
    SCAN_ALT_F                          =('f' | SCAN_ALT_MASK),
    SCAN_ALT_G                          =('g' | SCAN_ALT_MASK),
    SCAN_ALT_H                          =('h' | SCAN_ALT_MASK),
    SCAN_ALT_J                          =('j' | SCAN_ALT_MASK),
    SCAN_ALT_K                          =('k' | SCAN_ALT_MASK),
    SCAN_ALT_L                          =('l' | SCAN_ALT_MASK),
    SCAN_ALT_Z                          =('z' | SCAN_ALT_MASK),
    SCAN_ALT_X                          =('x' | SCAN_ALT_MASK),
    SCAN_ALT_C                          =('c' | SCAN_ALT_MASK),
    SCAN_ALT_V                          =('v' | SCAN_ALT_MASK),
    SCAN_ALT_B                          =('b' | SCAN_ALT_MASK),
    SCAN_ALT_N                          =('n' | SCAN_ALT_MASK),
    SCAN_ALT_M                          =('m' | SCAN_ALT_MASK)
}; 

#endif /* #if defined(__NT__) && !defined(__CURSES__) */

#if defined(__CURSES__) && !defined(__PDCURSES__)
enum scancodes
{
    SCAN_NONE                           =0x00,
    SCAN_F1                             =KEY_F(1),
    SCAN_F2                             =KEY_F(2),
    SCAN_F3                             =KEY_F(3),
    SCAN_F4                             =KEY_F(4),
    SCAN_F5                             =KEY_F(5),
    SCAN_F6                             =KEY_F(6),
    SCAN_F7                             =KEY_F(7),
    SCAN_F8                             =KEY_F(8),
    SCAN_F9                             =KEY_F(9),
    SCAN_F10                            =KEY_F(10),
    SCAN_HOME                           =KEY_HOME,
    SCAN_UP                             =KEY_UP,
    SCAN_PGUP                           =KEY_NPAGE,
    SCAN_LEFT                           =KEY_LEFT,
    SCAN_RIGHT                          =KEY_RIGHT,
    SCAN_END                            =KEY_END,
    SCAN_DOWN                           =KEY_DOWN,
    SCAN_PGDN                           =KEY_PPAGE,
    SCAN_INS                            =KEY_IC,
    SCAN_DEL                            =KEY_DC,
    SCAN_SHIFT_TAB                      =KEY_BTAB,
    SCAN_ESC_MASK                       =(1+KEY_MAX),
    SCAN_ALT_1                          =('1' | SCAN_ESC_MASK),
    SCAN_ALT_2                          =('2' | SCAN_ESC_MASK),
    SCAN_ALT_3                          =('3' | SCAN_ESC_MASK),
    SCAN_ALT_4                          =('4' | SCAN_ESC_MASK),
    SCAN_ALT_5                          =('5' | SCAN_ESC_MASK),
    SCAN_ALT_6                          =('6' | SCAN_ESC_MASK),
    SCAN_ALT_7                          =('7' | SCAN_ESC_MASK),
    SCAN_ALT_8                          =('8' | SCAN_ESC_MASK),
    SCAN_ALT_9                          =('9' | SCAN_ESC_MASK),
    SCAN_ALT_0                          =('0' | SCAN_ESC_MASK),
    SCAN_ALT_MINUS                      =('-' | SCAN_ESC_MASK),
    SCAN_ALT_PLUS                       =('=' | SCAN_ESC_MASK),
    SCAN_ALT_Q                          =('q' | SCAN_ESC_MASK),
    SCAN_ALT_W                          =('w' | SCAN_ESC_MASK),
    SCAN_ALT_E                          =('e' | SCAN_ESC_MASK),
    SCAN_ALT_R                          =('r' | SCAN_ESC_MASK),
    SCAN_ALT_T                          =('t' | SCAN_ESC_MASK),
    SCAN_ALT_Y                          =('y' | SCAN_ESC_MASK),
    SCAN_ALT_U                          =('u' | SCAN_ESC_MASK),
    SCAN_ALT_I                          =('i' | SCAN_ESC_MASK),
    SCAN_ALT_O                          =('o' | SCAN_ESC_MASK),
    SCAN_ALT_P                          =('p' | SCAN_ESC_MASK),
    SCAN_ALT_A                          =('a' | SCAN_ESC_MASK),
    SCAN_ALT_S                          =('s' | SCAN_ESC_MASK),
    SCAN_ALT_D                          =('d' | SCAN_ESC_MASK),
    SCAN_ALT_F                          =('f' | SCAN_ESC_MASK),
    SCAN_ALT_G                          =('g' | SCAN_ESC_MASK),
    SCAN_ALT_H                          =('h' | SCAN_ESC_MASK),
    SCAN_ALT_J                          =('j' | SCAN_ESC_MASK),
    SCAN_ALT_K                          =('k' | SCAN_ESC_MASK),
    SCAN_ALT_L                          =('l' | SCAN_ESC_MASK),
    SCAN_ALT_Z                          =('z' | SCAN_ESC_MASK),
    SCAN_ALT_X                          =('x' | SCAN_ESC_MASK),
    SCAN_ALT_C                          =('c' | SCAN_ESC_MASK),
    SCAN_ALT_V                          =('v' | SCAN_ESC_MASK),
    SCAN_ALT_B                          =('b' | SCAN_ESC_MASK),
    SCAN_ALT_N                          =('n' | SCAN_ESC_MASK),
    SCAN_ALT_M                          =('m' | SCAN_ESC_MASK)
}; 

#endif /* #if defined(__CURSES__) && !defined(__PDCURSES__) */

#if defined(__PDCURSES__)
enum scancodes
{
    SCAN_NONE                           =0x00,
    SCAN_F1                             =(KEY_F0 + 1),
    SCAN_F2                             =(KEY_F0 + 2),
    SCAN_F3                             =(KEY_F0 + 3),
    SCAN_F4                             =(KEY_F0 + 4),
    SCAN_F5                             =(KEY_F0 + 5),
    SCAN_F6                             =(KEY_F0 + 6),
    SCAN_F7                             =(KEY_F0 + 7),
    SCAN_F8                             =(KEY_F0 + 8),
    SCAN_F9                             =(KEY_F0 + 9),
    SCAN_F10                            =(KEY_F0 + 10),
    SCAN_HOME                           =KEY_HOME,
    SCAN_UP                             =KEY_UP,
    SCAN_PGUP                           =KEY_NPAGE,
    SCAN_LEFT                           =KEY_LEFT,
    SCAN_RIGHT                          =KEY_RIGHT,
    SCAN_END                            =KEY_END,
    SCAN_DOWN                           =KEY_DOWN,
    SCAN_PGDN                           =KEY_PPAGE,
    SCAN_INS                            =KEY_IC,
    SCAN_DEL                            =KEY_DC,
    SCAN_SHIFT_TAB                      =KEY_BTAB,
    SCAN_ALT_1                          = ALT_1,
    SCAN_ALT_2                          = ALT_2,
    SCAN_ALT_3                          = ALT_3,
    SCAN_ALT_4                          = ALT_4,
    SCAN_ALT_5                          = ALT_5,
    SCAN_ALT_6                          = ALT_6,
    SCAN_ALT_7                          = ALT_7,
    SCAN_ALT_8                          = ALT_8,
    SCAN_ALT_9                          = ALT_9,
    SCAN_ALT_0                          = ALT_0,
    SCAN_ALT_MINUS                      = ALT_MINUS,
    SCAN_ALT_PLUS                       = ALT_EQUAL,
    SCAN_ALT_Q                          = ALT_Q,
    SCAN_ALT_W                          = ALT_W,
    SCAN_ALT_E                          = ALT_E,
    SCAN_ALT_R                          = ALT_R,
    SCAN_ALT_T                          = ALT_T,
    SCAN_ALT_Y                          = ALT_Y,
    SCAN_ALT_U                          = ALT_U,
    SCAN_ALT_I                          = ALT_I,
    SCAN_ALT_O                          = ALT_O,
    SCAN_ALT_P                          = ALT_P,
    SCAN_ALT_A                          = ALT_A,
    SCAN_ALT_S                          = ALT_S,
    SCAN_ALT_D                          = ALT_D,
    SCAN_ALT_F                          = ALT_F,
    SCAN_ALT_G                          = ALT_G,
    SCAN_ALT_H                          = ALT_H,
    SCAN_ALT_J                          = ALT_J,
    SCAN_ALT_K                          = ALT_K,
    SCAN_ALT_L                          = ALT_L,
    SCAN_ALT_Z                          = ALT_Z,
    SCAN_ALT_X                          = ALT_X,
    SCAN_ALT_C                          = ALT_C,
    SCAN_ALT_V                          = ALT_V,
    SCAN_ALT_B                          = ALT_B,
    SCAN_ALT_N                          = ALT_N,
    SCAN_ALT_M                          = ALT_M
}; 

#endif /* #if defined(__PDCURSES__) */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__CURSES__)

short int
keyboard_query();

void
keyboard_read(
    unsigned char *const                o_scan,
    unsigned char *const                o_ascii);

#endif

#if defined(__DOS__) && !defined(__CURSES__)

#pragma aux keyboard_query = \
    "mov ah, 01h" \
    "int 16h" \
    "jz qnokey"\
    "mov ax, 0xffff"\
    "jmp qdone"\
    "qnokey:"\
    "xor ax, ax"\
    "qdone:"\
    value [ax];

#ifdef __FLAT__

#pragma aux keyboard_read= \
    "mov esi, eax" \
    "xor ah, ah" \
    "int 16h" \
    "mov [byte ptr esi], ah"\
    "mov [byte ptr ecx], al"\
    parm [eax][ecx]\
    modify [ esi ];

#else

#pragma aux keyboard_read= \
    "mov si, ax" \
    "xor ah, ah" \
    "int 16h" \
    "mov [byte ptr si], ah"\
    "mov si, cx"\
    "mov [byte ptr si], al"\
    parm [ax][cx]\
    modify [ si ];

#endif /* #ifdef __FLAT__ */

#endif /* #if defined(__DOS__) && !defined(__CURSES__) */

#ifdef __cplusplus
}
#endif

#define __keyboard_h__
#endif
