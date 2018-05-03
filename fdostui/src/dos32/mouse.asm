; MOUSE.ASM
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright 
; and related or neighboring rights to FDOSTUI Library. This work is published 
; from: United States.

include MOUSE.INC

.MODEL flat

.DATA?

PUBLIC queue
PUBLIC qstart
PUBLIC qend
PUBLIC qactive

QUEUE_SIZE EQU 24
queue mouse_event QUEUE_SIZE dup(?) ; mouse event circular buffer
QUEUE_BYTES EQU $ - queue ; event queue size

qstart dw 0 ; where the next element will be dequeued
qend dw 0 ; where the next element will be queued
qactive dw 0; number of occupied slots

UDATA_BYTES EQU $ - queue 

.DATA

PUBLIC _mouse_initialized
_mouse_initialized dd 0 ; indicator if mouse is enabled

.CODE

; get mouse event from queue
; 
; inputs:
;  eax address of struct mouse_event 
; ouputs:
;   eax 0 no event
;   eax !0 event
; destroys:
;  eax
public mouse_get_event_
mouse_get_event_ PROC

    push ebx
    push ecx
    push edx
    push esi
    push edi

    ; eax is struct to fill
    ; set edi address of struct
    mov edi, eax
    
    ; zero data structure 
    mov [word ptr edi].m_btn_state, 0
    mov [word ptr edi].m_curs_col, 0
    mov [word ptr edi].m_curs_row, 0

    ; check if any slots are active
    cli
    mov ax, [word ptr qactive]
    movzx eax, ax
    or ax, ax
    jz evexit
    
    dec ax
    mov [word ptr qactive], ax 
    
    ; set address of slot
    mov ax, [word ptr qstart]
    movzx eax, ax
    mov ebx, SIZE mouse_event
    mul ebx
    lea esi, queue
    add esi, eax
    
    ; qstart= (qstart+1) % QUEUE_SIZE
    mov ax, [word ptr qstart]
    inc ax
    cwd
    mov bx, QUEUE_SIZE
    div bx 
    mov [word ptr qstart], dx
    
    ; copy event into data structure
    mov ecx, 3
    rep movsw
    
    mov eax, -1

evexit:
    sti
    
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    
    ret
mouse_get_event_ ENDP


; handle mouse interrupt
; place event in queue
; input:
;   ax condition mask causing call
;   bx button state
;   cx horizontal cursor position
;   dx vertical cursor position
;   di horizontal counts
;   si vertical counts
;   ds mouse driver segment
mouse_handler PROC FAR
; handler for mouse events
; store event in circular buffer queue

    push eax
    push ds
    
    mov ax, seg queue
    mov ds, ax
    
    ; insert into end of queue
    mov ax, [word ptr qend]
    movzx eax, ax
    push bx
    push dx
    mov bx, SIZE mouse_event
    mul bx
    pop dx
    pop bx
    lea esi, queue
    add esi, eax
    
    mov [word ptr esi].m_btn_state, bx
    mov [word ptr esi].m_curs_col, cx
    mov [word ptr esi].m_curs_row, dx
    
    ; end = (end + 1) % QUEUE_SIZE;
    mov ax, [word ptr qend]
    inc ax
    cwd
    mov bx, QUEUE_SIZE
    div bx 
    mov [word ptr qend], dx
    
    ; test if we reached maximum capacity
    ; if qactive < QUEUE_SIZE) 
    ;    qactive++;
    ; else
    ;   qstart= (qstart + 1) % QUEUE_SIZE;
    mov ax, [word ptr qactive]
    cmp ax, QUEUE_SIZE
    jge hndoverflow
    inc ax
    mov [word ptr qactive], ax
    jmp hndexit
    
hndoverflow:    
    ; move start to the next oldest
    mov ax, [word ptr qstart]
    inc ax
    cwd
    mov bx, QUEUE_SIZE
    div bx 
    mov [word ptr qstart], dx
    jmp hndexit
    
hndexit:    

    pop ds
    pop eax
    
    retf

mouse_handler ENDP

; query if a mouse is present
; inputs:
;   none
; outputs:
;   eax -1  mouse not supported (driver not installed)
;   eax 0 mouse is installed
;   ebx number of mouse buttons
; destroys: 
;   eax
mouse_query_has_mouse PROC 
;
; testing mouse presence
; 1. test interrupt vector 33H to see if it contains a value
;    other than zero. Zero indicates mouse driver is not installed.
; 2. Check if vector points to in IRET (cfh) instruction
; 3. Check for presence of mouse via int 33h

    push es

    ; get interrupt vector
    ; ah = 35
    ; al = interrupt vector to get
    ; returns:
    ; es:bx pointer to interrupt handler
    mov ah, 035h
    mov al, 033h
    int 021h
    mov ax, es
    or  ax, ax
    jz  nomouse
    shl ax, 4
    movzx ebx, bx
    or bx, ax
    cmp [byte ptr ebx], 0cfh
    jz  nomouse
    ; Reset/Query installed state
    ; AX = 0
    ; int 33h
    ; returns:
    ; AX= 0 mouse not installed
    ; AX= FFFFh mouse is installed
    ; BX= number of mouse buttons
    xor ax, ax
    int 033h
    or  ax, ax
    jz  nomouse
    xor eax, eax
    jmp qryexit
    
nomouse:
    mov eax, -1
    xor ebx, ebx
    
qryexit:

    pop es
    
    ret
mouse_query_has_mouse ENDP

; initialize mouse 
;
; outputs:
;   eax -1  mouse not supported (driver not installed)
;   eax 0 mouse has been initialized
; destroys:
;   eax
;
; int mouse_init();
;
public mouse_init_
mouse_init_ PROC 

    push ebx
    push ecx
    push edx
    push esi
    push edi
    push es
    
    mov [dword ptr _mouse_initialized], 0
    mov [dword ptr qstart], 0
    mov [dword ptr qend], 0
    mov [dword ptr qactive], 0
    
    call mouse_query_has_mouse
    or eax, eax
    jnz iniexit
    
    ; lock linear region 
    ; ax = 0600h
    ; bx:cx starting linear address
    ; si:di size of region to lock (bytes)
    
    ; lock data region
    mov ebx, offset queue 
    shr ebx, 16 
    mov ecx, offset queue
    and ecx, 0ffffh
    mov esi, UDATA_BYTES 
    shr esi, 16
    mov edi, UDATA_BYTES
    and edi, 0ffffh
    mov ax, 0600h
    int 031h
    
    ; lock handler
    mov ebx, offset mouse_handler 
    shr ebx, 16
    mov ecx, offset mouse_handler
    and ecx, 0ffffh
    mov esi, offset mouse_query_has_mouse - offset mouse_handler
    mov edi, esi
    shr esi, 16
    and edi, 0ffffh
    mov ax, 0600h
    int 031h
    
    ; int33h 0ch Set user defined subroutine
    ; ax 0ch
    ; cx call mask
    ; es:[e]dx offset of subroutine 
    mov ax, seg mouse_handler
    mov es, ax
    mov edx, offset mouse_handler 
    mov cx, 0fh
    mov ax, 0ch
    int 33h
    
    mov [dword ptr _mouse_initialized], 1
    xor eax, eax
   
iniexit:
    
    pop es
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx

    ret
mouse_init_ ENDP

public mouse_deinit_
mouse_deinit_ PROC

    ; int 33h 00h mouse installed flag and reset
    xor ax, ax
    int 033h
    mov [dword ptr _mouse_initialized], 0
    ret

mouse_deinit_ ENDP

END
