; VIDEO.ASM
;
; Adapted from the work of Jerome Shidel INFO.PAS.
;
; Copyright 1990-2015, Jerome Shidel.
;
; This project and related files are subject to the terms of the Mozilla 
; Public License,  v. 2.0. If a copy of the MPL was not distributed with 
; this file, You can obtain one at http://mozilla.org/MPL/2.0/.
;

; This code is untested. Need to test on real adapter hardware. 

include VIDEO.INC

.MODEL medium

.CODE

; check for vesa
; NOTE: requires 512+ bytes of stack space
; inputs:
;  none
; outputs:
;  ax 0 success vesa card
;  ax !0 not vesa card
; destroys:
;  ax
video_detect_vesa PROC FAR

    push bp 
    mov bp,sp 
    sub sp, 0200h  ; set up 512 bytes of stack
    
    push di
    push es
    
    ; Get SuperVGA information
    ; es:di 512 byte buffer to store SuperVGA information
    push ss  
    pop es
    lea  di, [bp-200h] 
    mov ax, 4f00h
    int 10h
    
    ; returns 
    ; al 4fh function supported
    ; ah status
    ;   00h success es:di buffer filled
    ;   01h failed
    ; for VBE 2.0
    ;   02h function not supported by current hardware configuration
    ;   03h function invalid in current video mode
  
    cmp  al, 4fh
    jne  notvesa
    xor  ax, ax
    jmp  vesa_exit
    
notvesa:
    mov ax, 0ffffh

vesa_exit:

    pop es
    pop di
    
    mov sp,bp 
    pop bp 
    retf
video_detect_vesa ENDP

PUBLIC video_detect_
video_detect_ PROC FAR

    push bx
    push cx
    push dx

    call far ptr video_detect_vesa 
    xor ax, ax
    jnz detect_vga
    mov ax, VIDEO_CARD_VESA
    jmp detect_exit
 
detect_vga:

    ; Video refresh control (VGA)
    ; inputs:
    ; al 00h enable refresh
    ; al 01h disable refresh
    ; returns al = 12h if function not supported
    mov ax, 1200h
    mov bl, 36h
    int 10h
    cmp al, 12h
    jne detect_mcga
    mov ax, VIDEO_CARD_VGA
    jmp detect_exit
    
detect_mcga:

    ; video addressing (VGA,MCGA)
    ; inputs:
    ; al 00h enable video addressing
    ; al 01h disable video addressing
    ; returns al=12h if function not supported
    mov ax, 1200h
    mov bl, 32h
    int 10h
    cmp al, 12h
    jne detect_ega
    mov ax, VIDEO_CARD_MCGA
    jmp detect_exit
  
detect_ega:

    ; get EGA info
    ; returns:
    ; bh video state
    ;   00h color mode in effect
    ;   01h mono mode in effect
    ; bl installed memory 
    ;   00h 64K
    ;   01h 128K
    ;   02h 192K
    ;   03h 256K
    ; ch feature connector bits
    ; cl switch settings
    mov ax, 1200h
    mov bl, 10h
    int 10h
    cmp bh, 10h ; ? not sure if this is correct
    jne detect_cga
    mov ax, VIDEO_CARD_EGA
    jmp detect_exit
  
detect_cga:

    mov  dx, 3D4h
    mov  al, 0Ah
    out  dx, al
    inc  dx
    in   al, dx
    xchg ah, al
    mov  al, 6
    out  dx, al
    mov  cx, 50h
loop_cga:
    loop loop_cga
    in   al, dx
    xchg ah, al
    out  dx, al
    cmp  ah, 6
    jne  detect_mono
    mov  ax, VIDEO_CARD_CGA
    jmp  detect_exit

detect_mono:
    mov  dx, 3b4h
    mov  al, 0Ah
    out  dx, al
    inc  dx
    in   al, dx
    xchg ah, al
    mov  al, 6
    out  dx, al
    mov  cx, 50h
    
loop_mono2:
    loop loop_mono2
    in   al, dx
    xchg ah, al
    out  dx, al
    cmp  ah, 6
    jne  detect_unkown
    mov  cx, 50000
    mov  bx, 80h
    xor  ax, ax
    mov  dx, 3bah
    
loop_mono3:
    in     al, dx
    test   ax, bx
    loopnz loop_mono3
    jz     detect_testincolor
    mov    ax, VIDEO_CARD_MDA
    jmp    detect_exit
    
detect_testincolor:
    and    al, 01110000b
    cmp    al, 01010000b
    jne    detect_hgcplus 
    mov    ax, VIDEO_CARD_INCOLOR
    jmp    detect_exit
    
detect_hgcplus:
    cmp    al, 00010000b
    jne    detect_hgc
    mov    ax, VIDEO_CARD_HGCPLUS
    jmp    detect_exit
    
detect_hgc:
    mov    ax, VIDEO_CARD_HGC
    jmp    detect_exit
    
detect_unkown:
    xor ax, ax
   
detect_exit:

    pop dx
    pop cx
    pop bx

    retf

video_detect_ ENDP
    
END
