#include <stdint.h>

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

void ap_computing_test()
{
  volatile uint8_t *flag = (volatile uint8_t *)0x80000004;
  uint8_t A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  uint8_t B[] = {11, 21, 31, 41, 51, 61, 71, 81, 91, 111};

  ap_computing(XOR, LEFT, A, B, 10);

  while (ap_irq_check() != 0) {
    *flag = 0; 
  }
  // ap_set_if_state(DYNAMIC);
  *flag = 1;
}

void test() {
  volatile uint8_t *flag = (volatile uint8_t *)0x80000000;
  *flag = 1;
  volatile uint8_t x = *flag;
  flag = (volatile uint8_t *)0x80004004;
  *flag = 10;
}

void pointer_ap_rw_test() {
  uint32_t addr = 0x80004000;
  uint32_t quant_internal_cols = 6;
  uint32_t dtim_addr = 0x80000004;

  volatile uint8_t *ptr = (volatile uint8_t *) addr;
  volatile uint8_t *ptr_dtim = (volatile uint8_t *) dtim_addr; 

  // Write
  for(int i = 0; i < AP_COL_SIZE*quant_internal_cols; i++) {
    ptr = (volatile uint8_t *) (addr + i);
    *ptr = i % 128; 
  }

  // Direct Read
  for(int i = 0; i < AP_COL_SIZE*quant_internal_cols; i++) {
    ptr = (volatile uint8_t *) (addr + i);
    ptr_dtim = (volatile uint8_t *) (dtim_addr + i*4);
    *ptr_dtim = *ptr;
  }

  // Passing to a register and then to DTIM
  for(int i = 0; i < AP_COL_SIZE*quant_internal_cols; i++) {
    ptr = (volatile uint8_t *) (addr + i);
    ptr_dtim = (volatile uint8_t *) (dtim_addr + i*4);
    uint8_t tmp = *ptr;
    *ptr_dtim = tmp + 1;
  }
}

void api_ap_test() {
  uint8_t A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  uint8_t B[] = {11, 21, 31, 41, 51, 61, 71, 81, 91, 111};

  ap_write_vector(CAM_A, LEFT, A, 10);
  ap_write_vector(CAM_B, LEFT, B, 10);
}

int main()
{

  /* AP initiatilation */
  warmup_ap();

  /* Test function calls */
  /* pointer_ap_rw_test(); */
  ap_computing_test();
  /* api_ap_test(); */

  /* Break Vivado Simulation */
  sim_break();

  return 0;
}
