; DPMI.INC
;
; License CC0 PUBLIC DOMAIN
;
; To the extent possible under law, Mark J. Olesen has waived all copyright 
; and related or neighboring rights to FDOSTUI Library. This work is published 
; from: United States.

realmode_regs STRUC
    _edi dd ?
    _esi dd ? 
    _ebp dd ? 
    _resv dd ?
    _ebx dd ?
    _edx dd ?
    _ecx dd ?
    _eax dd ?
    _cpu dw ?
    _es dw ?
    _ds dw ?
    _fs dw ?
    _gs dw ?
    _ip dw ?
    _cs dw ?
    _sp dw ?
    _ss dw ?
realmode_regs ENDS

