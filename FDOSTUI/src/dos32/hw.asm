; HW.ASM
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright
; and related or neighboring rights to FDOSTUI Library. This work is published
; from: United States.

include DPMI.INC

.MODEL flat

.DATA

PUBLIC __hw_drives

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
hw_detect_drives_ PROC

    push ebp 
    mov ebp, esp 
    sub esp, SIZE realmode_regs ; setup stck for realmode regs
    
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    push es
   
    ; fcb needs 40 bytes
    ; filespec needs 3 bytes
    ; allocate 3 pages (3*16=48)
    ; int 31h function 100h Allocate DOS memory block
    ; bx = number of 16 byte paragahs desired
    mov ax, 0100h
    mov bx, 03h ; 3 * 16 = 48
    int 031h
    jc drive_error
    ; returns 
    ; cf clear
    ;  ax = real mode segment base address of allocated block
    ;  dx = selector for allocated block
    
    mov fs, dx ; fs:esi file spec 
    xor esi, esi
    mov [byte ptr fs:esi+0], 'A'
    mov [byte ptr fs:esi+1], ':'
    mov [byte ptr fs:esi+2],  0
    mov edx, eax  ; dx = real mode segment for filespec & fcb
    
    push ss ; es:edi realmode_regs
    pop es
    lea edi, [ebp - SIZE realmode_regs]
    
    xor ebx, ebx ; index into __hw_drives/drive number
    
drive_loop:

    ; zero out realmode_regs struct
    push edi
    mov ecx, SIZE realmode_regs
    xor eax, eax
    rep stosb
    pop edi
   
    mov [word ptr es:edi]._ds, dx ; filespec segment
    mov [word ptr es:edi]._es, dx ; fcb segment
    mov [dword ptr es:edi]._edi, 4 ; fcb offset

    push ebx
    
    ; simulate real mode interrupt
    mov [dword ptr es:edi]._eax, 02900h ; parse filename
    xor ebx, ebx
    mov bl, 021h  
    xor ecx, ecx
    mov ax, 0300h
    int 031h
    
    pop ebx
    
    mov eax, [dword ptr es:edi]._eax
    cmp al, 0ffh   ; ffh = invalid
    je drive_invalid
    mov ax, 1
    jmp drive_store 

drive_invalid:
    xor ax, ax
    
drive_store:

    push esi
    lea esi, __hw_drives
    add esi, ebx
    mov [byte ptr esi], al
    pop esi
    inc [byte ptr fs:esi]
    inc ebx
    cmp ebx, 25
    jle drive_loop    
    
    ; int 31h function 101h Free DOS memory block */
    ; dx = selecttor of block to be freed */
    mov dx, fs
    mov ax, 0101h
    int 031h
    
    ; a&b are always reported above
    ; check BIOS equipment word to see how many floppies there are
    mov ebx, 0410h; BIOS data area equipment word
    mov dx, [word ptr ebx]
    
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
    
drive_error:
    
drive_exit: 

    pop es
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax

    mov esp, ebp 
    pop ebp 
    
    ret
hw_detect_drives_  ENDP

END
