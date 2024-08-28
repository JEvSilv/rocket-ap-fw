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

void r_w_tests()
{
  /* Write and column selection test */
  for (int i = 0; i < 5; i++) {
    ap_set_cols(CAM_A, LEFT);
    ap_write(i, i);
    ap_set_cols(CAM_A, RIGHT);
    ap_write(i, i);

    ap_set_cols(CAM_B, LEFT);
    ap_write(i, i);
    ap_set_cols(CAM_B, RIGHT);
    ap_write(i, i);

    ap_set_cols(CAM_C, LEFT);
    ap_write(i, i);
    ap_set_cols(CAM_C, RIGHT);
    ap_write(i, i);
  }

  ap_set_cols(CAM_A, LEFT);

  uint8_t data = ap_read(4);
  volatile uint8_t *data_addr = (volatile uint8_t *)0x80000004;
  *data_addr = data;
}

void ap_computing_test()
{
  volatile uint8_t *flag = (volatile uint8_t *)0x80000004;
  uint8_t A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  uint8_t B[] = {11, 21, 31, 41, 51, 61, 71, 81, 91, 111};

  ap_computing(XOR, LEFT, A, B, 10);

  while (ap_irq_check() != 0) {
    flag = 0;
  }
  *flag = 1;
}

int main()
{
  /* AP initiatilation */
  warmup_ap();

  /* Test function calls */
  // r_w_tests();
  ap_computing_test();

  /* Break Vivado Simulation */
  sim_break();

  return 0;
}
