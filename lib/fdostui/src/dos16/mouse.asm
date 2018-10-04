; MOUSE.ASM
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright 
; and related or neighboring rights to FDOSTUI Library. This work is published 
; from: United States.


include MOUSE.INC

.MODEL medium

.DATA?

public queue

QUEUE_SIZE EQU 24
queue mouse_event QUEUE_SIZE dup(?) ; mouse event circular buffer
QUEUE_BYTES EQU $ - queue ; event queue size

.DATA

PUBLIC _mouse_initialized

public qstart
public qend
public qactive

qstart dw 0 ; where the next element will be dequeued
qend dw 0 ; where the next element will be queued
qactive dw 0; number of occupied slots

_mouse_initialized dw 0 ; indicator if mouse is enabled
saved_mask dw 0  ; saved interrupt chain mask
saved_routine_dx dw 0 ; saved interrupt dx value
saved_routine_es dw 0 ; saved interrupt es value

.CODE
        
; get mouse event from queue
; 
; inputs:
;  ax:dx address of struct mouse_event (ax offset, dx segment)
; ouputs:
;   ax 0 no event
;   ax !0 event
; destroys:
;  ax
;
; int mouse_get_event(struct mouse_event far* ev);
;
public mouse_get_event_
mouse_get_event_ PROC FAR

    push bx
    push cx
    push dx
    push si
    push di
    push es

    ; ax(offset):dx(segment) is struct to fill
    ; set es:di address of struct
    mov di, ax
    push dx
    pop es
    
    ; zero data structure 
    mov cx, SIZE mouse_event
    xor ax, ax
    push di
    cld
    rep stosb
    pop di

    ; check if any slots are active
    cli
    mov ax, [word ptr qactive]
    or ax, ax
    jz evexit
    
    dec ax
    mov [word ptr qactive], ax 
    
    ; set address of slot
    mov ax, [word ptr qstart]
    mov bx, SIZE mouse_event
    mul bx
    lea si, queue
    add si, ax
    
    ; qstart= (qstart+1) % QUEUE_SIZE
    mov ax, [word ptr qstart]
    inc ax
    cwd
    mov bx, QUEUE_SIZE
    div bx 
    mov [qstart], dx
    
    ; copy event into data structure
    ;mov ax, [ds:si].m_btn_state
    ;mov [es:di].m_btn_state, ax
    ;mov ax, [ds:si].m_curs_col
    ;mov [es:di].m_curs_col, ax
    ;mov ax, [ds:si].m_curs_row
    ;mov [es:di].m_curs_row, ax
    cld
    mov cx, 3
    rep movsw
    
    mov ax, 0FFFFh

evexit:
    sti
    
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    
    retf
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

    push ax
    push ds
    
    mov ax, seg queue
    mov ds, ax
    
    ; insert into end of queue
    mov ax, [word ptr qend]
    push bx
    push dx
    mov bx, SIZE mouse_event
    mul bx
    pop dx
    pop bx
    lea si, queue
    add si, ax
    
    mov [word ptr si].m_btn_state, bx
    mov [word ptr si].m_curs_col, cx
    mov [word ptr si].m_curs_row, dx
    
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
    pop ax
    
    retf
mouse_handler ENDP

; query if a mouse is present
; inputs:
;   none
; outputs:
;   ax -1  mouse not supported (driver not installed)
;   ax 0 mouse is installed
;   bx number of mouse buttons
; destroys: 
;   ax
mouse_query_has_mouse PROC FAR
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
    mov ah, 0035h
    mov al, 0033h
    int 0021h
    mov ax, es
    or  ax, ax
    jz  nomouse
    cmp byte ptr es:[bx], 0cfh
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
    xor ax, ax
    jmp qryexit
    
nomouse:
    mov ax, -1
    xor bx, bx
    
qryexit:

    pop es
    
    retf
mouse_query_has_mouse ENDP 

; initialize mouse 
;
; outputs:
;   ax -1  mouse not supported (driver not installed)
;   ax 0 mouse has been initialized
; destroys:
;   ax
;
; int mouse_init();
;
public mouse_init_
mouse_init_ PROC FAR

    push bx
    push cx
    push dx
    push es

    mov [word ptr qstart], 0
    mov [word ptr qend], 0
    mov [word ptr qactive], 0

    call mouse_query_has_mouse 
    or ax, ax
    jnz iniexit
    
    ; exchange interrupt routines
    mov ax, seg mouse_handler
    mov es, ax
    mov dx, offset mouse_handler
    mov cx, 000fh ; mask for all events
    mov ax, 0014h
    int 033h
  
    mov [word ptr saved_mask], cx
    mov [word ptr saved_routine_dx], dx
    mov [word ptr saved_routine_es], es
    mov [word ptr _mouse_initialized], 1
    xor ax, ax
   
iniexit:

    pop es
    pop dx
    pop cx
    pop bx
    
    retf
mouse_init_ ENDP

; deinitialize mouse
; inputs:
;   none
; outputs:
;   none
; destroys:
;   none
; 
public mouse_deinit_
mouse_deinit_ PROC FAR

    push ax
    push bx
    push cx
    push dx
    push es

    cmp [word ptr _mouse_initialized], 0
    jz dexit
    
    dosmouse_hide
    
    ; restore interrupt handler 
    mov cx, [word ptr saved_mask]
    mov dx, [word ptr saved_routine_dx]
    mov ax, [word ptr saved_routine_es]
    mov es, ax
    mov ax, 00ch
    int 033h
    
    mov [word ptr saved_mask], 0
    mov [word ptr saved_routine_dx], 0
    mov [word ptr saved_routine_es], 0
    mov [word ptr _mouse_initialized], 0
    
dexit:

    pop es
    pop dx
    pop cx
    pop bx
    pop ax
    
    retf
mouse_deinit_ ENDP

END
