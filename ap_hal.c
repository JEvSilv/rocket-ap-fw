#include "ap_hal.h"

void tiny_delay(uint32_t delay_in_nops)
{
  for (int i = 0; i < delay_in_nops; i++) asm volatile("nop");
}

void reset_ap()
{
  *AP_RST = 0;
  // Maybe some delay here
  *AP_RST = 1;
  // Maybe some delay here
  *AP_RST = 0;
}

void warmup_ap()
{
  *AP_ADDR_IN = 0;
  *AP_MODE = 0;
  *AP_READ_EN = 0;
  *AP_OP_DIRECTION = 0;

  /* Zeroing all the CAMs */
  ap_set_cols(CAM_A, LEFT);
  reset_ap();
  ap_set_cols(CAM_A, RIGHT);
  reset_ap();
  ap_set_cols(CAM_A, LEFT);
}
void ap_set_mode(APMode mode) { *AP_MODE = (uint8_t)mode; }

void ap_set_cols(APCollunm sel_col, APInternalCollunm internal_col)
{
  *AP_SEL_COL = (uint8_t)sel_col;
  *AP_SEL_INT_COL = (uint8_t)internal_col;
}

void ap_write(uint32_t addr, uint8_t data)
{
  if (addr < 0 || addr > AP_CAM_SIZE) return;
  *AP_ADDR_IN = addr;
  *AP_DATA_IN = data;
  *AP_WRITE_EN = 1;
  *AP_WRITE_EN = 0;
}

void ap_write_vector(APCollunm col, APInternalCollunm internal_col,
                     uint8_t V[], size_t size)
{
  if (size > AP_CAM_SIZE) return;

  /* Setting collumns */
  ap_set_cols(col, internal_col);

  /* Starting from the zero address */
  for (int i = 0; i < size; i++) {
    *AP_ADDR_IN = i;
    *AP_DATA_IN = V[i];
    *AP_WRITE_EN = 1;
    *AP_WRITE_EN = 0;
  }
}

uint8_t ap_read(uint32_t addr)
{
  if (addr < 0 || addr > AP_CAM_SIZE) return 0;

  *AP_ADDR_IN = addr;
  *AP_READ_EN = 1;
  *AP_READ_EN = 0;
  /* This delay must be performed */
  tiny_delay(1);
  return *AP_DATA_OUT;
}

void ap_read_vector(APCollunm col, APInternalCollunm internal_col, uint8_t V[],
                    size_t size)
{
  if (size > AP_CAM_SIZE) return;

  /* Setting collumns */
  ap_set_cols(col, internal_col);

  /* Starting from the zero address */
  for (int i = 0; i < size; i++) {
    *AP_ADDR_IN = i;
    *AP_READ_EN = 1;
    *AP_READ_EN = 0;
    /* This delay must be performed */
    tiny_delay(1);
    V[i] = *AP_DATA_OUT;
  }
}

void ap_read_result_vector(APInternalCollunm internal_col, uint8_t V[],
                           size_t size)
{
  ap_read_vector(CAM_C, internal_col, V, size);
}

void set_op_direction(APOpDirection op_direction)
{
  *AP_OP_DIRECTION = (uint8_t)op_direction;
}

void trigger_ap_computing(APOperations op)
{
  *AP_CMD = op;
  *AP_MODE = (uint8_t)ASSOCIATIVE_PROCESSOR;
}

void ap_computing(APOperations op, APInternalCollunm internal_col,
                  uint8_t A[], uint8_t B[], size_t size)
{
  ap_write_vector(CAM_A, internal_col, A, size);
  ap_write_vector(CAM_B, internal_col, B, size);
  trigger_ap_computing(op);
}

uint8_t ap_irq_check() { return *AP_IRQ; }
