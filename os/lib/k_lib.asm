
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               k_lib.asm
;						      wzq
;                                                     2013/6/8
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


%include "k_const.inc"

global	disable_irq
global	enable_irq
global	disable_int
global	enable_int

[SECTION .text]

; =================================================================================
;                  void disable_irq(int irq_nb); (irq_nb=0,...,15)
; =================================================================================
disable_irq:
        mov     ecx, [esp + 4]          ; irq
        pushf
        cli
        mov     ah, 1
        rol     ah, cl                  ; ah = (1 << (irq % 8))
        cmp     cl, 8
        jae     disable_8               ; disable irq >= 8 at the slave 8259
disable_0:
        in      al, PORT_8259A_MAS_CTRLMASK
        test    al, ah
        jnz     dis_already             	  ; already disabled?
        or      al, ah
        out     PORT_8259A_MAS_CTRLMASK, al       ; set bit at master 8259
        popf
        mov     eax, 1                  ; disabled by this function
        ret
disable_8:
        in      al, PORT_8259A_SLA_CTRLMASK
        test    al, ah
        jnz     dis_already             	  ; already disabled?
        or      al, ah
        out     PORT_8259A_SLA_CTRLMASK, al       ; set bit at slave 8259
        popf
        mov     eax, 1                  ; disabled by this function
        ret
dis_already:
        popf
        xor     eax, eax                ; already disabled
        ret

; =================================================================================
;                  void enable_irq(int irq_nb); (irq_nb=0,...,15)
; =================================================================================
enable_irq:
        mov     ecx, [esp + 4]          ; irq
        pushf
        cli
        mov     ah, ~1
        rol     ah, cl                  ; ah = ~(1 << (irq % 8))
        cmp     cl, 8
        jae     enable_8                ; enable irq >= 8 at the slave 8259
enable_0:
        in      al, PORT_8259A_MAS_CTRLMASK
        and     al, ah
        out     PORT_8259A_MAS_CTRLMASK, al       ; clear bit at master 8259
        popf
        ret
enable_8:
        in      al, PORT_8259A_SLA_CTRLMASK
        and     al, ah
        out     PORT_8259A_SLA_CTRLMASK, al       ; clear bit at slave 8259
        popf
        ret

; ========================================================================
;		   void disable_int();
; ========================================================================
disable_int:
	cli
	ret

; ========================================================================
;		   void enable_int();
; ========================================================================
enable_int:
	sti
	ret
