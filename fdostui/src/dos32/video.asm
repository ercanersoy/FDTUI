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
include DPMI.INC
include VIDEO.INC

.MODEL flat

.CODE

; check for vesa
; inputs:
;  none
; outputs:
;  ax 0 success vesa card
;  ax !0 not vesa card
; destroys:
;  ax
video_detect_vesa PROC

    push ebp 
    mov ebp, esp 
    sub esp, SIZE realmode_regs ; setup stack for realmode regs
    
    push ebx
    push ecx
    push edx
    push edi
    push es

    ; int 31h function 100h Allocate DOS memory block
    ; bx = number of 16 byte paragahs desired
    mov ax, 0100h
    mov bx, 020h ; 32 * 16 = 512
    int 031h
    ; returns 
    ; cf clear
    ;  ax = real mode segment base address of allocated block
    ;  dx = selector for allocated block
    jc vesa_error
    
    push ss
    pop es
    
    push eax
    ; set realmode_regs elements to zero
    lea  edi, [ebp - SIZE realmode_regs]
    push edi
    mov ecx, SIZE realmode_regs
    xor eax, eax
    rep stosb
    pop edi
    pop eax
    
    ; Get SuperVGA information
    ; es:edi 512 byte buffer to store SuperVGA information
    mov [dword ptr es:edi]._eax, 04f00h
    mov [word ptr es:edi]._es, ax
    xor ebx, ebx
    mov bl, 010h
    xor ecx, ecx
    mov ax, 0300h
    int 031h

    ; int 31h function 101h Free DOS memory block */
    ; dx = selecttor of block to be freed */
    mov ax, 0101h
    int 031h
    
    ; returns 
    ; al 4fh function supported
    ; ah status
    ;   00h success buffer filled
    ;   01h failed
    ; for VBE 2.0
    ;   02h function not supported by current hardware configuration
    ;   03h function invalid in current video mode
    mov eax, [dword ptr es:edi]._eax
    cmp  al, 04fh
    jne  vesa_error
    xor  eax, eax
    jmp  vesa_exit
    
vesa_error:
    mov eax, -1

vesa_exit:

    pop es
    pop edi
    pop edx
    pop ecx
    pop ebx

    mov esp,ebp 
    pop ebp 
    ret

video_detect_vesa ENDP

PUBLIC video_detect_
video_detect_ PROC

    push ebx
    push ecx
    push edx

    call video_detect_vesa 
    xor eax, eax
    jnz detect_vga
    mov eax, VIDEO_CARD_VESA
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
    mov eax, VIDEO_CARD_VGA
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
    mov eax, VIDEO_CARD_MCGA
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
    mov eax, VIDEO_CARD_EGA
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
    mov  eax, VIDEO_CARD_CGA
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
    mov    eax, VIDEO_CARD_MDA
    jmp    detect_exit
    
detect_testincolor:
    and    al, 01110000b
    cmp    al, 01010000b
    jne    detect_hgcplus 
    mov    eax, VIDEO_CARD_INCOLOR
    jmp    detect_exit
    
detect_hgcplus:
    cmp    al, 00010000b
    jne    detect_hgc
    mov    eax, VIDEO_CARD_HGCPLUS
    jmp    detect_exit
    
detect_hgc:
    mov    eax, VIDEO_CARD_HGC
    jmp    detect_exit
    
detect_unkown:
    xor eax, eax
   
detect_exit:

    pop edx
    pop ecx
    pop ebx

    ret

video_detect_ ENDP

END
