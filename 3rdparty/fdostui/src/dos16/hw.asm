; HW.ASM
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright
; and related or neighboring rights to FDOSTUI Library. This work is published
; from: United States.

.MODEL medium

.DATA

PUBLIC __hw_drives

filespec db '?', ':', 0 ; buffer to use as filespec
__hw_drives db 26 dup(0) ; drives 'A'-'Z' 0=not available 1=available

.CODE


; int 21 fun 29 parse file name
; used in testing if drive letter is valid
; ds:si null terminated string contains drive letter followed by colon
; es:di 37-byte dummy fcb buffer
; on return if al is ff the drive is invalid


; detect drives 
; inputs 
;  none
; ouputs
;  __hw_drives
; destroys:
;  none
;
PUBLIC hw_detect_drives_ 
hw_detect_drives_ PROC FAR

    push bp 
    mov bp,sp 
    sub sp, 028h  ; set up 40 bytes of stack for fcb
    
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    
    ; set up es:di for fcb bufffer 
    push ss  
    pop es
    lea  di, [bp-28h] 
    
    xor bx, bx ; index into __hw_drives/drive number
   
drive_loop:
    lea si, filespec ; ds:si filespec
    mov ax, bx
    add ax, 'A'
    mov [byte ptr si], al
    
    mov ax, 02900h ; parse filename
    int 21h
    cmp al, 0ffh   ; ffh = invalid
    je drive_invalid
    mov ax, 1
    jmp drive_store 
    
drive_invalid:
    xor ax, ax
    
drive_store:
    
    lea si, __hw_drives
    add si, bx
    mov [byte ptr si], al
    inc bx
    cmp bx, 25
    jle drive_loop    
    
    ; a&b are always reported above
    ; check BIOS equipment word to see how many floppies there are
    mov ax, 040h ; BIOS data area
    mov es, ax
    mov bx, 010h ; equipment word
    mov dx, es:[word ptr bx]
    
    ; bit 0 indictates if floppy installed
    ; 0 not installed
    ; 1 installed
    mov ax, dx
    and ax, 01h
    jnz drive_testflop
    
    mov [byte ptr __hw_drives], 0
    mov [byte ptr __hw_drives+1], 0
    jmp drive_exit 
   
drive_testflop:
    ; bits 6 & 7 number of floppies less 1
    mov cl, 05h
    shr dx, cl
    and dx, 011b
    dec dx
    cmp dx, 2 
    jge drive_exit
    
    mov [byte ptr __hw_drives+1], 0 ; no b drive
    
drive_exit: 
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    mov sp,bp 
    pop bp 
    retf
    
hw_detect_drives_  ENDP

END
