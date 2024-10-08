#include <stdint.h>
#include <stdlib.h>

#include "ap_hal.h"

#define SIM_BREAK (volatile uint8_t *)(0x80000000)

void sim_break() { *SIM_BREAK = 0xff; }

void basic_tests()
{
  int a = 10;
  int b = 30;
  volatile int c = a + b;
  volatile uint8_t *ap_reset_addr = (volatile uint8_t *)0x80003FEC;

  *ap_reset_addr = 1;
  *ap_reset_addr = 0;
}

uint8_t ap_unit_test_sw(APOperations op, uint8_t x, uint8_t y) {
	switch (op) {
		case OR:  return x | y;
			break;
		case XOR: return x ^ y;
			break;
		case AND: return x & y;
			break;
		case NOT: return ~x;
			break;
		case ADD: return x + y;
			break;
		case SUB: return x - y;
			break;
		case MULT:return x * y;
			break;
		default:
			break;
	}
	return 0;
}

int ap_computing_horizontal_test(APOperations op, APInternalCollunm internal_col, uint8_t seed)
{
  uint8_t *A = (uint8_t *) malloc(AP_COL_SIZE);
  uint8_t *B = (uint8_t *) malloc(AP_COL_SIZE);
  uint8_t *C = (uint8_t *) malloc(AP_COL_SIZE);

  uint32_t mask = 0xff;

  if(op > 3) {
	  mask = 0xf;
  }

  for(int i = 0; i < AP_COL_SIZE; i++) {
	  A[i] = (seed-i * 1234 + i + 4321) & mask;
	  B[i] = (seed+i * 4321 - i + 1234) & mask;
  }

  ap_computing(op, internal_col, HORIZONTAL, A, B, AP_COL_SIZE);

  while (ap_irq_check() == 0) {}

  *AP_CONTROL = 0x0;

  ap_read_vector(CAM_C, internal_col, C, AP_COL_SIZE);

  int index_error = 0;

  for(int i = 0; i < AP_COL_SIZE; i++) {
	  uint8_t a = A[i];
	  uint8_t b = B[i];

	  uint8_t ap_op = C[i];
	  uint8_t sw_op = ap_unit_test_sw(op, a, b);

	  if(ap_op != sw_op) {
		  return 0;
	  }
  }

  free(A);
  free(B);
  free(C);

  return 1;
}

void test()
{
  volatile uint8_t *flag = (volatile uint8_t *)0x80000000;
  *flag = 1;
  volatile uint8_t x = *flag;
  flag = (volatile uint8_t *)0x80004004;
  *flag = 10;
}

void pointer_ap_rw_test()
{
  uint32_t addr = 0x80004000;
  uint32_t quant_internal_cols = 6;
  uint32_t dtim_addr = 0x80000000;

  volatile uint8_t *ptr = (volatile uint8_t *)addr;
  volatile uint8_t *ptr_dtim = (volatile uint8_t *)dtim_addr;

  // Write
  for (int i = 0; i < AP_COL_SIZE * quant_internal_cols; i++) {
    ptr = (volatile uint8_t *)(addr + i);
    *ptr = i % 128;
    tiny_delay(1);
  }

  // Direct Read
  for (int i = 0; i < AP_COL_SIZE * quant_internal_cols; i++) {
    ptr = (volatile uint8_t *)(addr + i);
    ptr_dtim = (volatile uint8_t *)(dtim_addr + i * 4);
    *ptr_dtim = *ptr;
    tiny_delay(1);
  }

  // Passing to a register and then to DTIM
  for (int i = 0; i < AP_COL_SIZE * quant_internal_cols; i++) {
    ptr = (volatile uint8_t *)(addr + i);
    ptr_dtim = (volatile uint8_t *)(dtim_addr + i * 4);
    uint8_t tmp = *ptr;
    *ptr_dtim = tmp + 1;
    tiny_delay(1);
  }
}

void testing_internal_cols() {
	uint32_t addr = 0x80004000;
	volatile uint8_t *ptr_v = (volatile uint8_t *)addr;
	uint8_t result[10];

	for (int i = 0; i < 10; i++) {
		ptr_v[i] = i+1;
		tiny_delay(1);
	}

	for (int i = 0; i < 10; i++) {
		result[i] = ptr_v[i];
		tiny_delay(1);
	}

	return;
}

void api_r_w_ap_test()
{
  // Put the breakpoint on the loops and
  // will see the values storage on the buffer read

  uint8_t buffer[AP_COL_SIZE];
  uint8_t buffer_read[AP_COL_SIZE];

  for(int i = 0; i < AP_COL_SIZE; i++) {
	  buffer[i] = i;
  }

  ap_write_vector(CAM_A, LEFT, buffer, AP_COL_SIZE);
  ap_read_vector(CAM_A, LEFT, buffer_read, AP_COL_SIZE);

  for(int i = 0; i < AP_COL_SIZE; i++) {
  	  buffer[i] = i + 1;
  }

  ap_write_vector(CAM_A, RIGHT, buffer, AP_COL_SIZE);
  ap_read_vector(CAM_A, RIGHT, buffer_read, AP_COL_SIZE);

  for(int i = 0; i < AP_COL_SIZE; i++) {
  	  buffer[i] = i+2;
  }

  ap_write_vector(CAM_B, LEFT, buffer, AP_COL_SIZE);
  ap_read_vector(CAM_B, LEFT, buffer_read, AP_COL_SIZE);

  for(int i = 0; i < AP_COL_SIZE; i++) {
      buffer[i] = i+3;
  }

  ap_write_vector(CAM_B, RIGHT, buffer, AP_COL_SIZE);
  ap_read_vector(CAM_B, RIGHT, buffer_read, AP_COL_SIZE);

  for(int i = 0; i < AP_COL_SIZE; i++) {
	  buffer[i] = i+4;
  }

  ap_write_vector(CAM_C, LEFT, buffer, AP_COL_SIZE);
  ap_read_vector(CAM_C, LEFT, buffer_read, AP_COL_SIZE);

  for(int i = 0; i < AP_COL_SIZE; i++) {
	  buffer[i] = i+5;
  }

  ap_write_vector(CAM_C, RIGHT, buffer, AP_COL_SIZE);
  ap_read_vector(CAM_C, RIGHT, buffer_read, AP_COL_SIZE);

  return;
}

int main()
{
  /* AP initialization */
  warmup_ap();

  /* Test function calls */
  //pointer_ap_rw_test();
  //api_r_w_ap_test();
  //specific_test();
  volatile int test = ap_computing_horizontal_test(XOR, RIGHT, 14);

  asm volatile ("WFI");

  return 0;
}
