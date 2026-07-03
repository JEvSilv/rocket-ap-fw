.globl  ap_store_data
.type   ap_store_data, @function

.globl  ap_get_data
.type   ap_get_data, @function

.globl  ap_get_data_to
.type   ap_get_data_to, @function

#define OUTPUT_AP_ADDR 0x80002000

ap_store_data:
	sw a1, 0(a0)
	nop
	ret

ap_get_data:
	lw a0, 0(a0)
	nop
	ret

ap_get_data_to:
	lw t1, 0(a1)
	nop
	nop
	sb t1, 0(a0)
	ret

// Offload C col procedure in assembly addr, size
