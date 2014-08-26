
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               print.asm
;							wzq
;                                                       2013/5/21
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


%include "k_const.inc"

; 导出函数
global	disp_str
global  disp_return


[SECTION .text]

; ========================================================================
;                  void disp_str(char * str);
; ========================================================================
disp_str:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	; str
	mov	edi, [disp_pos]
	mov	ah, 0Fh
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; 是回车吗?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos], edi

	pop	ebp
	ret
; ========================================================================
;                  void disp_return();
; ========================================================================
disp_return:
	push    ebx

	mov	eax, [disp_pos]
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	[disp_pos], eax

	pop	ebx

	ret

