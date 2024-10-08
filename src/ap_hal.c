#include "ap_hal.h"
#include <stdint.h>

void tiny_delay(uint32_t delay_in_nops)
{
  for (int i = 0; i < delay_in_nops; i++) asm volatile("nop");
}

void warmup_ap()
{
  // Init Mode reg - setting left internal col
  *AP_MODE = 0x0;

  // Block AP Wraper if and set and release reset
  *AP_CONTROL = 0x10001;
  *AP_CONTROL = 0x10000;

  // Init Mode reg - Set right internal col
  *AP_MODE = 0x1000000;

  // Block AP Wraper if and set and release reset
  *AP_CONTROL = 0x10001;
  *AP_CONTROL = 0x10000;

  // Relase AP Wrapper interface and put a default value for control
  *AP_MODE = 0x0;
  *AP_CONTROL = 0x00000;
}

void ap_write_vector(APCollunm col, APInternalCollunm internal_col, uint8_t *V,
                     size_t size)
{
  if (size > AP_COL_SIZE) return;

  *AP_MODE = (internal_col << 24) | (col << 16);

  uint8_t i_col = (uint8_t)internal_col;
  uint32_t final_addr = CAM_A_BASE_ADDR + (AP_COL_SIZE * 2 * ((uint8_t)col)) +
                        (i_col * AP_COL_SIZE);

  volatile uint8_t *cam = (volatile uint8_t *)(final_addr);

  for (int i = 0; i < size; i++) {
    cam[i] = V[i];
    tiny_delay(10);
  }
}

void ap_read_vector(APCollunm col, APInternalCollunm internal_col, uint8_t *V,
                    size_t size)
{
  if (size > AP_COL_SIZE) return;

  *AP_MODE = (internal_col << 24) | (col << 16);

  uint8_t i_col = (uint8_t)internal_col;
  uint32_t final_addr = CAM_A_BASE_ADDR + (AP_COL_SIZE * 2 * ((uint8_t)col)) +
                        (i_col * AP_COL_SIZE);

  volatile uint8_t *cam = (volatile uint8_t *)(final_addr);

  for (int i = 0; i < size; i++) {
    V[i] = cam[i];
  }
}

void ap_read_result_vector(APInternalCollunm internal_col, uint8_t *V,
                           size_t size)
{
  ap_read_vector(CAM_C, internal_col, V, size);
}

void ap_computing(APOperations op, APInternalCollunm internal_col, APOpDirection op_direction, uint8_t *A,
                  uint8_t *B, size_t size)
{
  // Load Vectors
  ap_write_vector(CAM_A, internal_col, A, size);
  ap_write_vector(CAM_B, internal_col, B, size);


  // Block interface
  *AP_CONTROL = 0x10000;

  // Setting internal collumn
  set_mode_reg(internal_col, op_direction, op);

  // Trigger ap computation - bug here
  *AP_CONTROL = 0x10000 | (1 << 8);
}

void release_ap_if() {
  set_control_reg(FALSE, FALSE, FALSE);
}

volatile uint8_t ap_irq_check()
{
    volatile uint8_t ap_irq = *AP_IRQ;
    return ap_irq; 
}
