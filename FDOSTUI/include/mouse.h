/*
 MOUSE.H

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#ifndef __mouse_h__

#ifndef __FAR
#   if defined(__FLAT__) || defined(__LARGE__)
#       define __FAR
#   else
#       define __FAR __far
#   endif
#endif

#if defined(__DOS__) || defined(__DJGPP__)

#if defined(__WATCOMC__)
#pragma pack(push, 1);
#endif

struct mouse_event 
{
    short int                           m_btn_state;
    short int                           m_curs_col;
    short int                           m_curs_row;
};

#if defined(__WATCOMC__)
#pragma pack(pop);
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__DOS__) || defined(__DJGPP__)

extern short int mouse_initialized;

extern int mouse_get_event(struct mouse_event __FAR* o_event);

extern int mouse_init();

extern void mouse_deinit();

extern void mouse_show();

extern void mouse_hide();

#if defined(__WATCOMC__)

#pragma aux mouse_hide= \
    "mov ax, [mouse_initialized]" \
    "or ax, ax" \
    "jz mhnomouse" \
    "mov ax, 02h" \
    "int 33h" \
    "mhnomouse:" \
    modify [ AX ];

#pragma aux mouse_show= \
    "mov ax, [mouse_initialized]" \
    "or ax, ax" \
    "jz msnomouse" \
    "mov ax, 01h" \
    "int 33h" \
    "msnomouse:" \
    modify [ AX ];

#endif

#endif


#if defined(__NT__) || defined(__CURSES__)

#define mouse_init() 0
#define mouse_deinit()
#define mouse_hide()
#define mouse_show()

#endif

#ifdef __cplusplus
}
#endif

#define __mouse_h__
#endif
