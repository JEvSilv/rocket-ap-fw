.globl  get_cycles
.type   get_cycles, @function

get_cycles:
	csrr a1, mcycleh
	csrr a0, mcycle
	csrr a5, mcycleh
	bne a1, a5, get_cycles
	ret
