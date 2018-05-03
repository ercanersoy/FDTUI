; SCREEN.ASM
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright
; and related or neighboring rights to FDOSTUI Library. This work is published
; from: United States.

include VIDEO.INC

.MODEL flat

.DATA

VIDEO_MAX_ROWS EQU 50 ; maximum number of __video_rows

PUBLIC __video_cols
PUBLIC __video_rows

video_card dw 0 ; type of video card detected
video_restore_mode db 3 ; mode to restore graphics state
video_addr dd 0b8000h ; video buffer linear address
__video_cols dd 80 ; number of columns
__video_rows dd 25 ; number of video rows
video_line_size dd (2 * 80)  ; size of a line (__video_cols * 2) attr+char
video_bytes dd (2 * 80 * 25) ; buffer size (video_line_size * __video_rows)
video_line_offsets dd VIDEO_MAX_ROWS dup(0) ; start of line video_line_offsets

.CODE

EXTERN video_detect_:proc

; initialize video to mode 3 text 80x25
; inputs:
;   none
; outputs:
;   ax 0 success
;   ax !0 failure
; destroys:
;   ax
PUBLIC screen_init_
screen_init_ PROC

    push ebx
    push ecx
    push edi
    
    call video_detect_
    mov [word ptr video_card], ax
    
    ; get current video mode
    mov ax, 0f00h
    int 10h
    
    ; save it to restore on exit
    mov [byte ptr video_restore_mode], al
    
    cmp al, 7 ; check if MDA
    jne notmda
    
    ; MDA not supported
    mov eax, -1
    jmp init_exit
    
notmda:

    cmp al, 3 ; check if mode 3 text 80x25
    je vidset
    
    ; switch to video mode 3
    mov ax, 03h
    int 10h

vidset:

    ; set active display page
    mov ax, 0500h ; set page 0
    int 10h

    ; disable blinking for full background colors
    ; works on EGA/VGA only
    mov ax, 1003h
    xor bl, bl
    int 10h

    ; fill line video_line_offsets
    xor eax, eax
    mov ecx, VIDEO_MAX_ROWS
    lea edi, video_line_offsets
    ; cld
    
line:
    stosd
    add eax, [dword ptr video_line_size]
    loop line
    
    xor eax, eax ; retun success
    
init_exit:

    pop edi
    pop ecx
    pop ebx
    
    ret
screen_init_ ENDP

; restore video mode back to default
PUBLIC screen_deinit_
screen_deinit_ PROC

    push eax
    
    ; restore video mode
    xor ah, ah
    mov al, [byte ptr video_restore_mode]
    int 10h
    
    pop eax
    
    ret
screen_deinit_ ENDP

PUBLIC screen_set25lines_ 
screen_set25lines_ PROC

    push ebx
    push ecx
    push edx

    ; set vertical resolution
    ; al 12h 
    ; al vertical resolution
    ;   00h 200 scan lines
    ;   01h 350 scan lines
    ;   02h 400 scan lines
    mov ax, 1202h ; 400 scan lines
    mov bl, 30h
    int 10h
    
    ; set video mode
    ; ah 00
    ; al video mode
    mov ax, 03h
    int 10h
    
    mov [dword ptr __video_rows], 25
    mov eax, [dword ptr __video_cols]
    shl eax, 1
    mov ecx, 25
    mul ecx
    mov [dword ptr video_bytes], eax
    
    xor eax, eax

    pop edx
    pop ecx
    pop ebx

    ret
screen_set25lines_ ENDP

PUBLIC screen_set50lines_ 
screen_set50lines_ PROC

    push ebx
    push ecx
    push edx

    cmp [video_card], VIDEO_CARD_EGA     
    jl  set50_unsupported
    
    ; set vertical resolution
    ; al 12h 
    ; al vertical resolution
    ;   00h 200 scan lines
    ;   01h 350 scan lines
    ;   02h 400 scan lines
    mov ax, 1202h ; 400 scan lines
    mov bl, 30h
    int 10h
    
    ; set video mode
    ; ah 00
    ; al video mode
    mov ax, 03h
    int 10h
    
    ; Load ROM 8x8 dbl dot patterns
    mov ax, 1112h
    xor bl, bl  ; block to load
    int 10h
    
    mov [dword ptr __video_rows], 50 
    mov eax, [dword ptr __video_cols]
    shl eax, 1
    mov ecx, 50
    mul ecx
    mov [dword ptr video_bytes], eax
    
    xor eax, eax
    jmp set50_exit
    
set50_unsupported:
    mov eax, -1
    
set50_exit:

    pop edx
    pop ecx
    pop ebx

    ret
screen_set50lines_ ENDP

; fill the screen
; inputs:
;  al character
;  dl foreground color
;  bl background color
; ouptuts:
;   none    
; destroys:
;   none
PUBLIC screen_fill_
screen_fill_ PROC 

    push eax
    push ebx
    push ecx
    push edx
    push edi
    
    shl ebx, 4
    or dl, bl
    mov ah, dl
    
    mov edi, [dword ptr video_addr]
    mov ecx, [dword ptr video_bytes]
    shr ecx, 1 ; convert size to words
    rep stosw
    
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
    
    ret
screen_fill_ ENDP

; fill an area of the screen
; eax col coordinate
; edx row coordinate
; ebx number of columns
; ecx number of rows
; [bp+8] char
; [bp+12] foreground color
; [bp+16] background color
PUBLIC screen_fill_area_ 
screen_fill_area_ PROC 

    push ebp 
    mov ebp,esp 
    
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi

    ; sanity checks
    or ebx, ebx
    jz  area_exit
    or ecx, ecx
    jz  area_exit
    cmp eax, [dword ptr __video_cols]
    jge area_exit
    cmp edx, [dword ptr __video_rows]
    jge area_exit
    cmp eax, 0
    jge area_check_row
    
    add ebx, eax
    xor eax, eax
    cmp ebx, 0
    jle area_exit
    
area_check_row:
    cmp edx, 0
    jge area_start
    
    add edx, ecx
    xor edx, edx
    cmp edx, 0
    jle area_exit

area_start:

    push eax  ; save col
    push ebx  ; save number of columns
    push ecx  ; save number of rows
    push edx  ; save row

    shl eax, 1 ; each column is 2 bytes (char+attr)
    
    ; set edi to precomputed line address
    mov ebx, edx ; row coordinate
    shl ebx, 2 ; index into video_line_offsets (row * 4)
    mov edi, [dword ptr video_line_offsets][ebx]
    add edi, eax ; add column
    or edi, [dword ptr video_addr]
    
    mov eax, [dword ptr ebp+12]; foreground color
    shl eax, 8 ; make it high word (move into ah)
    mov ebx, [dword ptr ebp+16] ; background color
    shl ebx, 4
    or ah, bl
    mov ebx, [dword ptr ebp+8] ; character
    mov al, bl
    
    pop edx  ; restore row
    pop ecx  ; restore number of rows
    pop ebx  ; restore number of columns
    pop esi  ; restore col
    
area_loop: 
    push ebx
    push esi
    push edi
area_store:
    stosw
    dec ebx
    jz area_next_row
    inc esi
    cmp esi, [dword ptr __video_cols]
    jl area_store
area_next_row:
    pop edi
    pop esi
    pop ebx
    dec ecx
    jz  area_exit
    add edi, [dword ptr video_line_size]
    inc edx
    cmp edx, [dword ptr __video_rows]
    jl area_loop

area_exit:

    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax

    mov esp, ebp
    pop ebp 
    ret 12 
screen_fill_area_ ENDP

; write a character to the screen repeating horizontally
; inputs:
;  eax col coordinate
;  edx row coordinate
;  ebx/bl character
;  ecx repeat count
;  [bp+8] foreground color
;  [bp+12] background color  
; outputs:
;  none
; destroys:
;  none
public screen_putc_
screen_putc_ PROC 

    push ebp 
    mov ebp,esp 
    
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    
    ; sanity checks 
    or ecx, ecx 
    jz putc_exit
    cmp eax, [dword ptr __video_cols] 
    jge putc_exit
    cmp edx, [dword ptr __video_rows]
    jge putc_exit
   
    cmp ax, 0
    jge putc_start
 
    add ecx, eax 
    xor eax, eax
    cmp ecx, 0
    jle putc_exit
    
putc_start:

    mov esi, eax ; column
    shl eax, 1 ; each column is 2 bytes (char+attr)
    
    ; calculate precomputed line address
    xchg ebx, edx ; swap so edx is char and ebx is row
    shl ebx, 2 ; index into video_line_offsets (row * 4)
    mov edi, [dword ptr video_line_offsets][ebx]
    add edi, eax
    or edi, [dword ptr video_addr]
    
    mov eax, [dword ptr ebp+8] ; foreground color
    shl eax, 8 ; move into ah
    
    mov al, dl ; character to write
    mov ebx, [dword ptr ebp+12] ; background color
    shl ebx, 4
    or ah, bl
    
putc_repeat:
    stosw
    inc esi
    cmp esi, [dword ptr __video_cols]
    jge putc_exit
    dec ecx
    jnz putc_repeat
   
putc_exit:

    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    
    mov esp, ebp
    pop ebp 
    ret 8
screen_putc_ ENDP

; write a character to the screen repeating vertically
; inputs:
;  eax col coordinate
;  edx row coordinate
;  ebx/bl character
;  ecx repeat count
;  [bp+8] foreground color
;  [bp+12] background color  
; outputs:
;  none
; destroys:
;  none
public screen_putvc_
screen_putvc_ PROC 

    push ebp 
    mov ebp,esp 
    
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    
    or ecx, ecx 
    jz putvc_exit
    cmp eax, [dword ptr __video_cols] 
    jge putvc_exit
    cmp eax, 0
    jl  putvc_exit
    cmp edx, [dword ptr __video_rows]
    jge putvc_exit
    
    cmp edx, 0
    jge putvc_start
    
    add ecx, edx
    xor edx, edx
    cmp ecx, 0
    jle putvc_exit
    
putvc_start:
    
    mov esi, edx ; row
    shl eax, 1 ; each column is 2 bytes (char+attr)
    
    ; calculate precomputed line address
    xchg ebx, edx ; swap so edx is char and ebx is row
    shl ebx, 2 ; index into video_line_offsets (row * 4)
    mov edi, [dword ptr video_line_offsets][ebx]
    add edi, eax
    or edi, [dword ptr video_addr]
    
    mov eax, [dword ptr ebp+8] ; foreground color
    shl eax, 8; move into ah
    
    mov al, dl ; character to write
    mov ebx, [dword ptr ebp+12] ; background color
    shl ebx, 4
    or ah, bl
    
putvc_repeat:
    mov [word ptr edi], ax
    add edi, [dword ptr video_line_size]
    inc esi
    cmp esi, [dword ptr __video_rows]
    jge putvc_exit
    dec ecx
    jnz putvc_repeat
   
putvc_exit:

    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax

    mov esp, ebp
    pop ebp 
    ret 8
screen_putvc_ ENDP

; writes a string to the screen
;  eax col coordinate
;  edx row coordinate
;  ebx address of string
;  ecx bytes to write
;  [bp+8] foreground color 
;  [bp+12] background color 
; outputs:
;  none
; destroys:
;  none
PUBLIC screen_puts_
screen_puts_ PROC 

    push ebp 
    mov ebp,esp 
    
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    
    ; sanity checks
    or ecx, ecx
    jz  puts_exit
    cmp eax, [dword ptr __video_cols]
    jge puts_exit
    cmp edx, [dword ptr __video_rows]
    jge puts_exit
    cmp eax, 0
    jge puts_start
    
puts_adjust:
    dec ecx 
    jz puts_exit
    inc ebx
    inc eax
    cmp eax, 0
    jl puts_adjust
    
puts_start:
    mov esi, ebx ; si string 
    
    push eax ; save column
    
    shl eax, 1 ; each column is 2 bytes (char+attr)
    
    ; set edi to precomputed line address
    mov ebx, edx ; row coordinate
    shl ebx, 2 ; index into video_line_offsets (row * 4)
    mov edi, [dword ptr video_line_offsets][ebx]
    add edi, eax ; add column
    or edi, [dword ptr video_addr]
    
    pop edx; restore column
    
    push ecx ; save bytes to write
    
    mov eax, [dword ptr ebp+8] ; foreground color
    shl eax, 8 ; make it high word (move into ah)
    mov ebx, [dword ptr ebp+12] ; background color
    shl ebx, 4
    or ah, bl
    
    pop ecx ; restore bytes to write
    
puts_store:
    lodsb
    stosw
    dec ecx
    jz puts_exit
    inc edx
    cmp edx, [dword ptr __video_cols]
    jl puts_store

puts_exit:

    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax

    mov esp, ebp
    pop ebp 
    ret 8
screen_puts_ ENDP

; set color 
;  eax col coordinate
;  edx row coordinate
;  ebx repeat count 
;  ecx foreground color
;  [ebp+8] background color 
PUBLIC screen_set_color_
screen_set_color_ PROC 

    push ebp 
    mov ebp,esp 
    
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi

    ; sanity checks
    or ebx, ebx
    jz  setc_exit
    cmp eax, [dword ptr __video_cols]
    jge setc_exit
    cmp edx, [dword ptr __video_rows]
    jge setc_exit
    cmp eax, 0
    jge setc_check_count
    
    add ebx, eax
    xor eax, eax
    cmp ebx, 0
    jle setc_exit
    
setc_check_count:
    mov esi, ebx
    add esi, eax
    cmp esi, [dword ptr __video_cols]
    jle setc_start
    mov ebx, [dword ptr __video_cols]
    sub ebx, eax

setc_start:

    push ebx ; save repeat count

    shl eax, 1 ; each column is 2 bytes (char+attr)
    
    ; set edi to precomputed line address
    mov ebx, edx ; row coordinate
    shl ebx, 2 ; index into video_line_offsets (row * 4)
    mov edi, [dword ptr video_line_offsets][ebx]
    add edi, eax ; add column
    or edi, [dword ptr video_addr]
    
    mov eax, ecx; foreground color
    shl eax, 8 ; make it high word (move into ah)
    mov ebx, [dword ptr ebp+8] ; background color
    shl ebx, 4
    or ah, bl
    
    mov ebx, eax ; save attribute
    
    pop ecx ; restore repeat count
   
    push edi ; set esi to edi
    pop esi
    
setc_loop:
    lodsw
    mov ah, bh
    stosw
    loop setc_loop

setc_exit:

    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax

    mov esp, ebp
    pop ebp 
    ret 4
screen_set_color_ ENDP

; read character and attribute from screen
; inputs
; eax column
; edx row
; outputs
; eax
; destroys
; eax
PUBLIC screen_getc_ PROC
screen_getc_ PROC 

    push ebx
    push esi
    
    cmp eax, 0
    jl getc_error
    cmp eax, [dword ptr __video_cols]
    jge getc_error
    cmp edx, 0
    jl getc_error
    cmp edx, [dword ptr __video_rows]
    jge getc_error

    shl eax, 1 ; each column is 2 bytes (char+attr)
    
    ; set esi to precomputed line address
    mov ebx, edx ; row coordinate
    shl ebx, 2 ; index into video_line_offsets (row * 4)
    mov esi, [dword ptr video_line_offsets][ebx]
    add esi, eax ; add column
    or esi, [dword ptr video_addr]
    lodsw
    jmp getc_exit
    
getc_error:
    xor eax, eax
    
getc_exit:
    
    pop esi
    pop ebx
    
    ret
screen_getc_ ENDP

; read from screen
; eax offset of output
; edx number of columns
; ebx column
; ecx row
; outputs:
; ax:dx ending address
; destroys
; ax dx
PUBLIC screen_read_
screen_read_ PROC 

    push ebx
    push ecx
    push edx
    push esi
    push edi
    
    mov edi, eax; edi address of output buffer
    
    cmp ecx, [dword ptr __video_rows]
    jge read_exit
    
    or edx, edx
    jz read_exit
    
    ; adjust until column is 0
read_adjust:
    cmp ebx, 0
    jge read_start
    inc ebx
    inc edi
    inc edi
    dec edx
    jz read_exit
    jmp read_adjust
   
read_start:
    
    push ebx ; save column

    mov eax, ebx
    shl eax, 1 ; each column is 2 bytes (char+attr)
    
    ; set esi to precomputed line address
    mov ebx, ecx ; row coordinate
    shl ebx, 2 ; index into video_line_offsets (row * 4)
    mov esi, [dword ptr video_line_offsets][ebx]
    add esi, eax ; add column
    or esi, [dword ptr video_addr]
    
    pop ebx; restore column
    mov ecx, [dword ptr __video_cols]
    
read_loop:
    lodsw
    stosw
    inc ebx
    cmp ebx, ecx
    jge read_exit
    dec edx
    jnz read_loop
 
read_exit:

    mov eax, edi
    
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    
    ret
screen_read_ ENDP

; write to screen
; inputs:
; eax column
; edx row
; ebx buffer address
; ecx number of columns
; outputs:
; eax ending address
; destroys
; eax 
PUBLIC screen_write_
screen_write_ PROC 
    
    push ebx
    push ecx
    push esi
    push edi
    
    mov esi, ebx ; buffer offset
    
    cmp eax, [dword ptr __video_cols]
    jge write_ret1
    cmp edx, 0 ; check row
    jl write_ret1
    cmp edx, [dword ptr __video_rows]
    jge write_ret1
    
    or ecx, ecx
    jz write_ret1
    
write_adjust:
    cmp eax, 0 ; adjust video column
    jge write_start
    inc eax
    inc esi
    inc esi
    loop write_adjust
    jmp write_ret1
 
write_start:

    ; esi address
    ; eax video column   
    ; ecx repeat count
    ; edx video row
    
    push eax ; video column
    
    shl eax, 1 ; each column is 2 bytes (char+attr)
    ; set edi to precomputed line address
    mov ebx, edx ; row coordinate
    shl ebx, 2 ; index into video_line_offsets (row * 4)
    mov edi, [dword ptr video_line_offsets][ebx]
    add edi, eax ; add column
    or edi, [dword ptr video_addr]
    
    mov edx, [dword ptr __video_cols]
    pop ebx ; restore video column
    
    ; esi source address
    ; edi destination address
    ; ebx video column
    ; ecx repeat count
    ; edx video columns
write_loop:
    lodsw
    stosw
    inc ebx
    cmp ebx, edx
    jge write_ret1 
    loop write_loop
    
write_ret1:
    mov eax, esi
    
write_exit:

    pop edi
    pop esi
    pop ecx
    pop ebx

    ret
    
screen_write_ ENDP

END
