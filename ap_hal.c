#include "ap_hal.h"
#include <stdint.h>

void tiny_delay(uint32_t delay_in_nops)
{
  for (int i = 0; i < delay_in_nops; i++) asm volatile("nop");
}

void warmup_ap()
{
  *AP_MODE = 0x0;
  *AP_CONTROL = 0x10001;
  *AP_CONTROL = 0x10000;
  *AP_MODE = 0x1000000;
  *AP_CONTROL = 0x10001;
  *AP_CONTROL = 0x10000;
  *AP_MODE = 0x0;
  *AP_CONTROL = 0x00000;
}

void ap_trigger_op()
{
  *AP_CONTROL |= ((uint8_t)ASSOCIATIVE_PROCESSOR) << 8;
}

void ap_set_cols(APCollunm sel_col, APInternalCollunm internal_col)
{
  *AP_MODE |= ((uint8_t)sel_col) << 16;
  *AP_MODE |= ((uint8_t)internal_col) << 24;
}

void ap_write_vector(APCollunm col, APInternalCollunm internal_col, uint8_t V[],
                     size_t size)
{
  if (size > AP_COL_SIZE) return;

  uint8_t i_col = (uint8_t)internal_col;
  uint32_t final_addr = CAM_A_BASE_ADDR + (AP_COL_SIZE * 2 * ((uint8_t)col)) +
                        (i_col * AP_COL_SIZE);

  for (int i = 0; i < size; i++) {
    volatile uint8_t *cam = (volatile uint8_t *)(final_addr + i);
    *cam = V[i];
  }
}

void ap_read_vector(APCollunm col, APInternalCollunm internal_col, uint8_t V[],
                    size_t size)
{
  if (size > AP_COL_SIZE) return;

  uint8_t i_col = (uint8_t)internal_col;
  uint32_t final_addr = CAM_A_BASE_ADDR + (AP_COL_SIZE * 2 * ((uint8_t)col)) +
                        (i_col * AP_COL_SIZE);

  for (int i = 0; i < size; i++) {
    volatile uint8_t *cam = (volatile uint8_t *)(final_addr + i);
    V[i] = *cam;
  }
}

void ap_read_result_vector(APInternalCollunm internal_col, uint8_t V[],
                           size_t size)
{
  ap_read_vector(CAM_C, internal_col, V, size);
}

void set_op_direction(APOpDirection op_direction)
{
  *AP_MODE |= ((uint8_t)op_direction) << 8;
}

void trigger_ap_computing(APOperations op)
{
  // *AP_MODE = 0;
  *AP_MODE = ((uint8_t) op);
  *AP_CONTROL = 0x10100;// ap_trigger_op();
}

void ap_computing(APOperations op, APInternalCollunm internal_col, uint8_t A[],
                  uint8_t B[], size_t size)
{
  ap_write_vector(CAM_A, internal_col, A, size);
  ap_write_vector(CAM_B, internal_col, B, size);
  *AP_CONTROL = 0x10000;
  *AP_MODE = (((uint8_t) internal_col) << 24) | ((uint8_t) op);
  *AP_CONTROL = 0x10000 | (1 << 8);
}

/* extern inline void ap_set_if_state(APIfState ap_if_state) { */
/*   *AP_CONTROL |= ((uint8_t) ap_if_state) << 16; */
/* } */

volatile uint8_t ap_irq_check()
{
    volatile uint8_t ap_irq = *AP_IRQ;
    return ap_irq; 
}
