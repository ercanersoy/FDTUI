; SCREEN.ASM
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright
; and related or neighboring rights to FDOSTUI Library. This work is published
; from: United States.

include VIDEO.INC

.MODEL medium

.DATA

VIDEO_MAX_ROWS EQU 50 ; maximum number of __video_rows
VIDEO_BUFFER EQU 0b800h ; video buffer seqment

PUBLIC __video_cols
PUBLIC __video_rows

video_card dw 0 ; type of video card detected
video_restore_mode db 3 ; mode to restore graphics state
video_off dw 0 ; offscreen video buffer segment
video_seg dw VIDEO_BUFFER ; video buffer segment
__video_cols dw 80 ; number of columns
__video_rows dw 25 ; number of video rows
video_line_size dw (2 * 80)  ; size of a line (__video_cols * 2) attr+char
video_bytes dw (2 * 80 * 25) ; buffer size (video_line_size * __video_rows)
video_line_offsets dw VIDEO_MAX_ROWS dup(0) ; start of line video_line_offsets

.CODE

EXTERN video_detect_:far
; unsigned _dos_allocmem( unsigned size, unsigned *segment);
; unsigned _dos_freemem( unsigned segment );
EXTERN _dos_allocmem_:far, _dos_freemem_:far

; initialize video to mode 3 text 80x25
; inputs:
;   none
; outputs:
;   ax 0 success
;   ax !0 failure
; destroys:
;   ax
PUBLIC screen_init_
screen_init_ PROC FAR

    push bx
    push cx
    push di
    push es
    
    call far ptr video_detect_
    mov [word ptr video_card], ax
    
    ; get current video mode
    mov ax, 0f00h
    int 10h
    
    ; save it to restore on exit
    mov [byte ptr video_restore_mode], al
    
    cmp al, 7 ; check if MDA
    jne notmda
    
    ; MDA not supported
    mov ax, 0FFFFh
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
    push ds
    pop es
    xor ax, ax
    mov cx, VIDEO_MAX_ROWS
    lea di, video_line_offsets
    ; cld
    
line:
    stosw
    add ax, [video_line_size]
    loop line
    
    ; allocate an off screen buffer
    lea dx, video_off
    mov ax, ((2 * 80 * VIDEO_MAX_ROWS) + 15) / 16
    call far ptr _dos_allocmem_
    
init_exit:

    pop bx
    pop es
    pop di
    pop cx
    
    retf
screen_init_ ENDP

; restore video mode back to default
PUBLIC screen_deinit_
screen_deinit_ PROC FAR
    push ax
    
    mov ax, word ptr [video_off]
    or ax, ax
    jz restorevid
    call far ptr _dos_freemem_
    
restorevid:
    ; restore video mode
    xor ah,ah
    mov al, [byte ptr video_restore_mode]
    int 10h
    
    pop ax
    
    retf
screen_deinit_ ENDP

PUBLIC screen_set25lines_ 
screen_set25lines_ PROC FAR

    push bx
    push cx
    push dx

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
    
    mov [word ptr __video_rows], 25
    mov ax, [word ptr __video_cols]
    shl ax, 1
    mov cx, 25
    mul cx
    mov [word ptr video_bytes], ax
    
    xor ax, ax

    pop dx
    pop cx
    pop bx

    retf
screen_set25lines_ ENDP

PUBLIC screen_set50lines_ 
screen_set50lines_ PROC FAR

    push bx
    push cx
    push dx

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
    
    mov [word ptr __video_rows], 50 
    mov ax, [word ptr __video_cols]
    shl ax, 1
    mov cx, 50
    mul cx
    mov [word ptr video_bytes], ax
    
    xor ax, ax
    jmp set50_exit
    
set50_unsupported:
    mov ax, 0ffffh
    
set50_exit:

    pop dx
    pop cx
    pop bx

    retf
screen_set50lines_ ENDP

; fill the screen
; inputs:
;  ax/al character
;  dx/dl foreground color
;  bx/bl background color
; ouptuts:
;   none    
; destroys:
;   none
PUBLIC screen_fill_
screen_fill_ PROC FAR

    push ax
    push bx
    push cx
    push dx
    push di
    push es
    
    mov cl, 4
    shl bx, cl
    or dl, bl
    mov ah, dl
    
    mov cx, [word ptr video_seg]
    mov es, cx
    xor di, di
    mov cx, [word ptr video_bytes]
    shr cx, 1 ; convert size to words
    ; cld
    rep stosw
    
    pop es
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    
    retf
screen_fill_ ENDP

; fill an area of the screen
; ax col coordinate
; dx row coordinate
; bx number of columns
; cx number of rows
; [bp+6] char
; [bp+8] foreground color
; [bp+10] background color
PUBLIC screen_fill_area_ 
screen_fill_area_ PROC FAR

    push bp 
    mov bp,sp 
    
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push ds
    push es

    ; sanity checks
    or bx, bx
    jz  area_exit
    or cx, cx
    jz  area_exit
    cmp ax, [__video_cols]
    jge area_exit
    cmp dx, [__video_rows]
    jge area_exit
    cmp ax, 0
    jge area_check_row
    
    add bx, ax
    xor ax, ax
    cmp bx, 0
    jle area_exit
    
area_check_row:
    cmp dx, 0
    jge area_start
    
    add dx, cx
    xor dx, dx
    cmp dx, 0
    jle area_exit

area_start:

    push ax  ; save col
    push bx  ; save number of columns
    push cx  ; save number of rows
    push dx  ; save row

    shl ax, 1 ; each column is 2 bytes (char+attr)
    
    ; set es:di to precomputed line address
    mov bx, dx ; row coordinate
    shl bx, 1 ; index into video_line_offsets (row * 2)
    mov di, [word ptr video_line_offsets][bx]
    add di, ax ; add column
    mov ax, [word ptr video_seg]
    mov es, ax
    
    mov ax, [word ptr bp+8]; foreground color
    mov cl, 8 ; make it high word (move into ah)
    shl ax, cl
    mov bx, [word ptr bp+10] ; background color
    mov cl, 4
    shl bx, cl
    or ah, bl
    mov bx, [word ptr bp+6] ; character
    mov al, bl
    
    pop dx  ; restore row
    pop cx  ; restore number of rows
    pop bx  ; restore number of columns
    pop si  ; restore col
    
    ; cld    
    
area_loop: 
    push bx
    push si
    push di
area_store:
    stosw
    dec bx
    jz area_next_row
    inc si
    cmp si, [__video_cols]
    jl area_store
area_next_row:
    pop di
    pop si
    pop bx
    dec cx
    jz  area_exit
    add di, [video_line_size]
    inc dx
    cmp dx, [__video_rows]
    jl area_loop

area_exit:

    pop es
    pop ds
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    mov sp, bp
    pop bp 
    retf 6
screen_fill_area_ ENDP

; write a character to the screen repeating horizontally
; inputs:
;  ax col coordinate
;  dx row coordinate
;  bx/bl character
;  cx repeat count
;  [bp+6] foreground color
;  [bp+8] background color  
; outputs:
;  none
; destroys:
;  none
public screen_putc_
screen_putc_ PROC FAR

    push bp 
    mov bp,sp 
    
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    
    ; sanity checks 
    or cx, cx 
    jz putc_exit
    cmp ax, [word ptr __video_cols] 
    jge putc_exit
    cmp dx, [word ptr __video_rows]
    jge putc_exit
   
    cmp ax, 0
    jge putc_start
 
    add cx, ax 
    xor ax, ax
    cmp cx, 0
    jle putc_exit
    
putc_start:

    mov si, ax ; column
    shl ax, 1 ; each column is 2 bytes (char+attr)
    
    ; calculate precomputed line address
    xchg bx, dx ; swap so dx is char and bx is row
    shl bx, 1 ; index into video_line_offsets (row * 2)
    mov di, [word ptr video_line_offsets][bx]
    add di, ax
    
    push cx ; save repeat count
    mov ax, [word ptr bp+6] ; foreground color
    mov cl, 8 
    shl ax, cl ; move into ah
    
    mov al, dl ; character to write
    mov bx, [word ptr bp+8] ; background color
    mov cl, 4
    shl bx, cl
    or ah, bl
    
    mov cx, [word ptr video_seg]
    mov es, cx
    pop cx ; restore repeat count
    
    ; cld
putc_repeat:
    stosw
    inc si
    cmp si, [word ptr __video_cols]
    jge putc_exit
    dec cx
    jnz putc_repeat
   
putc_exit:

    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    
    mov sp, bp
    pop bp 
    retf 4
screen_putc_ ENDP

; write a character to the screen repeating vertically
; inputs:
;  ax col coordinate
;  dx row coordinate
;  bx/bl character
;  cx repeat count
;  [bp+6] foreground color
;  [bp+8] background color  
; outputs:
;  none
; destroys:
;  none
public screen_putvc_
screen_putvc_ PROC FAR

    push bp 
    mov bp,sp 
    
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    
    or cx, cx 
    jz putvc_exit
    cmp ax, [word ptr __video_cols] 
    jge putvc_exit
    cmp ax, 0
    jl  putvc_exit
    cmp dx, [word ptr __video_rows]
    jge putvc_exit
    
    cmp dx, 0
    jge putvc_start
    
    add cx, dx
    xor dx, dx
    cmp cx, 0
    jle putvc_exit
    
putvc_start:
    
    mov si, dx ; row
    shl ax, 1 ; each column is 2 bytes (char+attr)
    
    ; calculate precomputed line address
    xchg bx, dx ; swap so dx is char and bx is row
    shl bx, 1 ; index into video_line_offsets (row * 2)
    mov di, [word ptr video_line_offsets][bx]
    add di, ax
    
    push cx ; save repeat count
    mov ax, [word ptr bp+6] ; foreground color
    mov cl, 8 
    shl ax, cl ; move into ah
    
    mov al, dl ; character to write
    mov bx, [word ptr bp+8] ; background color
    mov cl, 4
    shl bx, cl
    or ah, bl
    
    mov cx, [word ptr video_seg]
    mov es, cx
    pop cx ; restore repeat count
    
putvc_repeat:
    mov [word ptr es:di], ax
    add di, [word ptr video_line_size]
    inc si
    cmp si, [word ptr __video_rows]
    jge putvc_exit
    dec cx
    jnz putvc_repeat
   
putvc_exit:

    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    mov sp, bp
    pop bp 
    retf 4
screen_putvc_ ENDP

; writes a string to the screen
;  ax col coordinate
;  dx row coordinate
;  bx address of string
;  cx bytes to write
;  [bp+6] foreground color 
;  [bp+8] background color 
; outputs:
;  none
; destroys:
;  none
PUBLIC screen_puts_
screen_puts_ PROC FAR

    push bp 
    mov bp,sp 
    
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push ds
    push es
    
    ; sanity checks
    or cx, cx
    jz  puts_exit
    cmp ax, [__video_cols]
    jge puts_exit
    cmp dx, [__video_rows]
    jge puts_exit
    cmp ax, 0
    jge puts_start
    
puts_adjust:
    dec cx 
    jz puts_exit
    inc bx
    inc ax
    cmp ax, 0
    jl puts_adjust
    
puts_start:
    mov si, bx ; ds:si string 
    
    push ax ; save column
    
    shl ax, 1 ; each column is 2 bytes (char+attr)
    
    ; set es:di to precomputed line address
    mov bx, dx ; row coordinate
    shl bx, 1 ; index into video_line_offsets (row * 2)
    mov di, [word ptr video_line_offsets][bx]
    add di, ax ; add column
    mov ax, [word ptr video_seg]
    mov es, ax
    
    pop dx; restore column
    
    push cx ; save bytes to write
    
    mov ax, [word ptr bp+6] ; foreground color
    mov cl, 8 ; make it high word (move into ah)
    shl ax, cl
    mov bx, [word ptr bp+8] ; background color
    mov cl, 4
    shl bx, cl
    or ah, bl
    
    pop cx ; restore bytes to write
    
    ; cld
    
puts_store:
    lodsb
    stosw
    dec cx
    jz puts_exit
    inc dx
    cmp dx, [__video_cols]
    jl puts_store

puts_exit:

    pop es
    pop ds
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    mov sp, bp
    pop bp 
    retf 4
screen_puts_ ENDP

; set color 
;  ax col coordinate
;  dx row coordinate
;  bx repeat count 
;  cx foreground color
;  [bp+6] background color 
PUBLIC screen_set_color_
screen_set_color_ PROC FAR

    push bp 
    mov bp,sp 
    
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push ds
    push es

    ; sanity checks
    or bx, bx
    jz  setc_exit
    cmp ax, [__video_cols]
    jge setc_exit
    cmp dx, [__video_rows]
    jge setc_exit
    cmp ax, 0
    jge setc_check_count
    
    add bx, ax
    xor ax, ax
    cmp bx, 0
    jle setc_exit
    
setc_check_count:
    mov si, bx
    add si, ax
    cmp si, [__video_cols]
    jle setc_start
    mov bx, [__video_cols]
    sub bx, ax

setc_start:

    push bx ; save repeat count

    shl ax, 1 ; each column is 2 bytes (char+attr)
    
    ; set es:di to precomputed line address
    mov bx, dx ; row coordinate
    shl bx, 1 ; index into video_line_offsets (row * 2)
    mov di, [word ptr video_line_offsets][bx]
    add di, ax ; add column
    mov ax, [word ptr video_seg]
    mov es, ax
    
    mov ax, cx; foreground color
    mov cl, 8 ; make it high word (move into ah)
    shl ax, cl
    mov bx, [word ptr bp+6] ; background color
    mov cl, 4
    shl bx, cl
    or ah, bl
    
    mov bx, ax ; save attribute
    
    pop cx ; restore repeat count
    push di ; set ds:si to es:di
    pop si
    push es
    pop ds
    
setc_loop:
    lodsw
    mov ah, bh
    stosw
    loop setc_loop

setc_exit:

    pop es
    pop ds
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    mov sp, bp
    pop bp 
    retf 2
screen_set_color_ ENDP

; read character and attribute from screen
; inputs
; ax column
; dx row
; outputs
; ax
; destroys
; ax
PUBLIC screen_getc_ PROC
screen_getc_ PROC FAR

    push bx
    push si
    push ds
    
    cmp ax, 0
    jl getc_error
    cmp ax, [__video_cols]
    jge getc_error
    cmp dx, 0
    jl getc_error
    cmp dx, [__video_rows]
    jge getc_error

    shl ax, 1 ; each column is 2 bytes (char+attr)
    
    ; set ds:si to precomputed line address
    mov bx, dx ; row coordinate
    shl bx, 1 ; index into video_line_offsets (row * 2)
    mov si, [word ptr video_line_offsets][bx]
    add si, ax ; add column
    mov ax, [word ptr video_seg]
    mov ds, ax
    ; cld
    lodsw
    jmp getc_exit
    
getc_error:
    xor ax, ax
    
getc_exit:
    
    push ds
    push si
    push bx
    
    retf
screen_getc_ ENDP

; read from screen
; ax offset of output
; dx segment of output
; bx number of columns
; cx column
; [bp+6] row
; outputs:
; ax:dx ending address
; destroys
; ax dx
PUBLIC screen_read_
screen_read_ PROC FAR

    push bp 
    mov bp,sp 
    
    push bx
    push cx
    push si
    push di
    push ds
    push es
    
    mov es, dx ; es:di address
    mov di, ax 
    
    mov ax, [word ptr bp+6]
    cmp ax, [word ptr __video_rows]
    jge read_exit
    
    or bx, bx
    jz read_exit
    
    ; fill buffer
    push di
    push cx
    mov ax, 0ffffh
    mov cx, bx
    rep stosw
    pop cx
    pop di
    
    ; adjust until column is 0
read_adjust:
    cmp cx, 0
    jge read_start
    inc cx
    inc di
    inc di
    dec bx
    jz read_exit
    jmp read_adjust
   
read_start:
    
    push bx ; save number of columns
    push cx ; save column
    mov dx, [__video_cols]

    mov ax, cx
    shl ax, 1 ; each column is 2 bytes (char+attr)
    
    ; set ds:si to precomputed line address
    mov bx, [word ptr bp+6] ; row coordinate
    shl bx, 1 ; index into video_line_offsets (row * 2)
    mov si, [word ptr video_line_offsets][bx]
    add si, ax ; add column
    mov ax, [word ptr video_seg]
    mov ds, ax
    
    pop cx; save column
    pop bx ; number of columns
    
    ;cld 
read_loop:
    lodsw
    stosw
    inc cx
    cmp cx, dx
    jge read_exit
    dec bx
    jnz read_loop
 
read_exit:

    mov ax, di
    mov dx, es
    
    pop es
    pop ds
    pop di
    pop si
    pop cx
    pop bx

    mov sp, bp
    pop bp 
    retf 2
screen_read_ ENDP

; write to screen
; inputs:
; ax column
; dx row
; cx buffer segment
; bx buffer offset
; [bp+6] number of columns
; outputs:
; ax:dx ending address
; destroys
; ax dx
PUBLIC screen_write_
screen_write_ PROC FAR
    push bp 
    mov bp,sp 
    
    push bx
    push cx
    push si
    push di
    push ds
    push es
    
    mov si, bx ; buffer offset
    mov di, cx ; buffer segment
    
    cmp ax, [__video_cols]
    jge write_ret1
    cmp dx, 0 ; check row
    jl write_ret1
    cmp dx, [__video_rows]
    jge write_ret1
    
    mov cx, [word ptr bp+6] ; repeat count
    or cx, cx
    jz write_ret1
    
write_adjust:
    cmp ax, 0 ; adjust video column
    jge write_start
    inc ax
    inc si
    inc si
    loop write_adjust
    jmp write_ret1
 
write_start:

    ; di:si address
    ; ax video column   
    ; cx repeat count
    ; dx video row
    
    push ax ; video column
    push di ; save segment
    
    shl ax, 1 ; each column is 2 bytes (char+attr)
    ; set es:di to precomputed line address
    mov bx, dx ; row coordinate
    shl bx, 1 ; index into video_line_offsets (row * 2)
    mov di, [word ptr video_line_offsets][bx]
    add di, ax ; add column
    mov ax, [word ptr video_seg]
    mov es, ax
    
    mov dx, [word ptr __video_cols]
    pop ds ; restore segment ds:si points to source string
    pop bx ; restore video column
    
    ; ds:si source address
    ; es:di destination address
    ; bx video column
    ; cx repeat count
    ; dx video columns
write_loop:
    lodsw
    stosw
    inc bx
    cmp bx, dx
    jge write_end_loop 
    loop write_loop
    
write_end_loop:    
    mov ax, si
    mov dx, ds
    jmp write_exit
    
write_ret1:
    mov ax, si  ;ax dx offset/segment
    mov dx, bx
    
write_exit:

    pop es
    pop ds
    pop di
    pop si
    pop cx
    pop bx

    mov sp, bp
    pop bp 
    retf 2
screen_write_ ENDP

public screen_push_
screen_push_ PROC FAR

    push ax
    push cx
    push si
    push di
    push ds
    push es

    cmp word ptr [video_seg], VIDEO_BUFFER
    jne screen_push_exit

    mov cx, word ptr [video_bytes]
    shr cx, 1
    mov ax, word ptr [video_off]
    mov word ptr [video_seg], ax
    mov es, ax
    mov ax, VIDEO_BUFFER
    mov ds, ax
    xor si, si
    xor di, di
    rep movsw

screen_push_exit:

    pop es
    pop ds
    pop di
    pop si
    pop cx
    pop ax

    retf
screen_push_ ENDP

public screen_pop_
screen_pop_ PROC FAR
    
    push ax
    push cx
    push si
    push di
    push ds
    push es

    cmp word ptr [video_seg], VIDEO_BUFFER
    je screen_push_exit

    mov word ptr [video_seg], VIDEO_BUFFER
    mov cx, word ptr [video_bytes]
    shr cx, 1
    mov ax, word ptr [video_off]
    mov ds, ax
    mov ax, VIDEO_BUFFER
    mov es, ax
    xor si, si
    xor di, di
    rep movsw

    pop es
    pop ds
    pop di
    pop si
    pop cx
    pop ax

    retf
screen_pop_ ENDP

END
