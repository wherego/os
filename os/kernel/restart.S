.include "../include/k_const.s"
	
.global _restart
.global p_proc_ready

.text
.code32
.align 4
_restart:
	#movl p_proc_ready, %eax
	#movl $0x20, %ebx
	ljmp $0x38, $0
	iret
