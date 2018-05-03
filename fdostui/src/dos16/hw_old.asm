; HW.ASM
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright
; and related or neighboring rights to FDOSTUI Library. This work is published
; from: United States.

.MODEL medium

.DATA

PUBLIC __hw_floppies
PUBLIC __hw_drives

__hw_floppies db 0 ; number of floppy drives
__hw_drives db 24 dup(0) ; drives 'C'-'Z' 0=not available 1=available

.CODE


; www.seasip.info/VintagePC/floppies.html
; it is possible to have up to 4 floppy drives.
; MS-DOS 3.3 and earlier assign floppy drives first then hard drives
; e.g. floppies a,b,c,d followed by hard drives 'e'-'z.'
; MS=DOS 4.0 + assign floppies a and b, then hard drives followed by
; remaining floppies.

; detect number of floppy drives
; inputs:
;   none
; outputs:
;  __hw_floppies
; destroys:
;  none

public hw_detect_floppies_ 
hw_detect_floppies_ PROC FAR

    push ax
    push bx
    push cx
    push dx
    push es

    mov ax, 040h ; BIOS data area
    mov es, ax
    mov bx, 010h ; equipment word
    mov dx, es:[word ptr bx]
    
    ; bit 0 indictates if floppy installed
    ; 0 not installed
    ; 1 installed
    mov ax, dx
    and ax, 01h
    jz flop_exit    
    
    ; bits 6 & 7 number of floppies less 1
    mov cl, 06h
    shr dx, cl
    and dx, 011b
    inc dx
    mov [byte ptr __hw_floppies], dl
    
flop_exit:

    pop es
    pop dx
    pop cx
    pop bx
    pop ax
    
    retf
hw_detect_floppies_ ENDP

; detect hard drives 
; inputs 
;  none
; ouputs
;  __hw_drives
; destroys:
;  none
;
PUBLIC hw_detect_drives_ 
hw_detect_drives_ PROC FAR

    push ax
    push cx
    push dx
    push di
    push es

    mov ah, 019h ; get current drive
    int 21h
    push ax 
    
    mov cx, 24 ; number of drives to check 'C'-'Z'
    mov dx, 02h  ; starting drive number 'C'
    push ds
    pop es
    lea di, __hw_drives
    
drive_loop:
    mov ah, 0eh ; select drive dl=drive (0=a,1=b,...)
    int 21h
    mov ah, 019h ; get current drive
    int 21h
    cmp dl, al ; see if the drives are the same
    jz  drive_exist
    xor ax, ax
    jmp drive_next
    
drive_exist:
    mov ax, 1
    
drive_next:
    stosb
    inc dx
    loop drive_loop
    
    pop dx 
    mov ah, 0eh
    int 21h
    
    pop es
    pop di
    pop dx
    pop cx
    pop ax
   
    retf
hw_detect_drives_  ENDP

END
