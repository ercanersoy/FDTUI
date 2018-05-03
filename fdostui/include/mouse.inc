; MOUSE.INC
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright
; and related or neighboring rights to FDOSTUI Library. This work is published
; from: United States.

mouse_event STRUC
    m_btn_state  dw ?
    m_curs_col	 dw ?
    m_curs_row   dw ?
mouse_event ENDS

; reset mouse 
; performs a hardware and software reset
; inputs:
;   none
; outputs:
;   ax 0 mouse not installed
;   bx number of mouse buttons
;       0h: other than two buttons
;	2h: two buttons
;       03h: three buttons
;       FFFFh: two buttons
; destroys:
;   ax, bx
;
; Do not use this routine to test if a mouse is present.
; Use mouse_query_has_mouse, which automatically performs a reset 
;
; see dosmouse_soft_reset
dosmouse_reset MACRO 0
    xor ax, ax
    int 33h
ENDM

; show mouse pointer
; inputs:
;   none
; ouputs:
;   none
; destroys
;   ax
dosmouse_show MACRO 0
    mov ax, 01h
    int 33h
ENDM

; hide mouse pointer
; inputs:
;   none
; ouputs:
;   none
; destroys
;   ax
dosmouse_hide MACRO 0
    mov ax, 02h
    int 33h
ENDM

; query mouse status
;
; inputs:
;   none
; outputs:
;   bx button status
;       bit 0 left button down (bx & 1) == 1
;       bit 1 right button down (bx & 2) == 2
;       bit 2 center button down (bx & 4) == 4
;   cx x coordinate (horizontal); divide by 8 for text clm
;   dx y coordinate (vertical); divide by 8 for text line
; destroys:
;   ax, bx, cx, dx
;
dosmouse_query_status MACRO 0
    mov ax, 03h
    int 33h
ENDM

; set mouse pointer position
; the pointer is set even if it is hidden
; if x,y is outside the rnage the pointer will be pinned within the 
; range rectangle
; inputs:
;   cx x coordinate (horizontal); multiply text clm by 8
;   dx y coordinate (vertical); multiply text line by 8
; outputs:
;   none  
; destroys:
;   ax
dosmouse_set_position MACRO
    mov ax, 04h
    int 33h
ENDM

; query button press
; inputs:
;   bx button to query (0 = left, 1 = right, 2 = center)
; outputs:
;   ax button status
;       bit 0 left button down (bx & 1) == 1
;       bit 1 right button down (bx & 2) == 2
;       bit 2 center button down (bx & 4) == 4
;   bx times button was presssed since last call
;   cx x coordinate (horizontal) of last press; multiply text clm by 8
;   dx y coordinate (vertical) of last press; multiply text line by 8
; destroys:
;   ax, bx, cx, dx
dosmouse_query_button_press MACRO
    mov ax, 05h
    int 33h
ENDM

; query button release
; inputs:
;   bx button to query (0 = left, 1 = right, 2 = center)
; outputs:
;   ax button status
;       bit 0 left button down (bx & 1) == 1
;       bit 1 right button down (bx & 2) == 2
;       bit 2 center button down (bx & 4) == 4
;   bx times button was released since last call
;   cx x coordinate (horizontal) of last press; multiply text clm by 8
;   dx y coordinate (vertical) of last press; multiply text line by 8
; destroys:
;   ax, bx, cx, dx
dosmouse_query_button_release MACRO
    mov ax, 06h
    int 33h
ENDM

; set horizontal range
; set horizontal range to limit the mouse pointer
; inputs:
;   cx minimum x coordinate (horizontal pixel position)
;   dx maximum x coordinate (horizontal pixel position)
; outputs:
;   none
; destroys:
;   ax, cx, dx
dosmouse_set_horizontal_range MACRO
    mov ax, 07h
    int 33h
ENDM

; set veritical range
; set vertical range to limit the mouse pointer
; inputs:
;   cx minimum y coordinate (vertical pixel position)
;   dx maximum y coordinate (vertical pixel position)
; outputs:
;   none
; destroys:
;   ax, cx, dx
dosmouse_set_vertical_range MACRO
    mov ax, 08h
    int 33h
ENDM

; set graphics mouse pointer shape
; inputs:
;   bx pointer hot spot (horizontal) (-16 to 16)
;   cx pointer hot spot (vertical) (-16 to 16)
;   es:dx address of pointer shape bit field (64 byte data area)
; outputs:
;   none
; destroys:
;   ax
;
; The cursor bitmap is a 16x16 pixel image 
;    offset   size      description
;     0h      16 words  screen mask
;     20h     16 words  cursor mask
; 
dosmouse_set_graphics_pointer MACRO
    mov ax, 09h
    int 33h
ENDM

; set text cursor
; inputs:
;   bx 0 software cursor
;      1 hardware cursor
;   cx start of screen mask or scan line
;   dx end of screen mask or scan line
; returns:
;   none
; destroys:
;   ax
dosmouse_set_text_cursor MACRO
    mov ax, 0ah
    int 33h
ENDM


