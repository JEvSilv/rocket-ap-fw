#ifndef AP_HAL
#define AP_HAL

#include <stddef.h>
#include <stdint.h>

/* Defines */
#define AP_COL_SIZE 128
#define COL_QUANT 6
#define AP_BASE_ADDR 0x80004000

#define CAM_A_BASE_ADDR AP_BASE_ADDR
#define CAM_A_0_BASE_ADDR CAM_A_BASE_ADDR
#define CAM_A_1_BASE_ADDR (CAM_A_BASE_ADDR + AP_COL_SIZE)

#define CAM_B_BASE_ADDR AP_BASE_ADDR + (AP_COL_SIZE * 2)
#define CAM_B_0_BASE_ADDR CAM_B_BASE_ADDR
#define CAM_B_1_BASE_ADDR (CAM_B_BASE_ADDR + AP_COL_SIZE)

#define CAM_C_BASE_ADDR AP_BASE_ADDR + (AP_COL_SIZE * 4)
#define CAM_C_0_BASE_ADDR CAM_C_BASE_ADDR
#define CAM_C_1_BASE_ADDR (CAM_C_BASE_ADDR + AP_COL_SIZE)

#define AP_CONTROL_REGISTERS_BASE_ADDR \
  (AP_BASE_ADDR + (AP_COL_SIZE * COL_QUANT))
#define AP_MODE_CONFIGURATION_ADDR AP_CONTROL_REGISTERS_BASE_ADDR

#define AP_CONTROL_SETTINGS_ADDR (AP_CONTROL_REGISTERS_BASE_ADDR + 4)

#define AP_IRQ_ADDR (AP_CONTROL_REGISTERS_BASE_ADDR + 8)

#define AP_CONTROL ((volatile uint32_t *)AP_CONTROL_SETTINGS_ADDR)
#define AP_MODE ((volatile uint32_t *)AP_MODE_CONFIGURATION_ADDR)
#define AP_IRQ ((volatile uint32_t *)AP_IRQ_ADDR)

/* Enums */
typedef enum {
  OR = 0,
  XOR = 1,
  AND = 2,
  NOT = 3,
  ADD = 4,
  SUB = 5,
  MULT = 6
} APOperations;

typedef enum {
  CAM_A = 0,
  CAM_B = 1,
  CAM_C = 2,
} APCollunm;

typedef enum {
  LEFT = 0,
  RIGHT = 1,
} APInternalCollunm;

typedef enum { MEMORY = 0, ASSOCIATIVE_PROCESSOR = 1 } APMode;

typedef enum { HORIZONTAL = 0, VERTICAL = 1 } APOpDirection;

typedef enum { DYNAMIC = 0, STATIC = 1 } APIfState;

typedef enum { TRUE = 1, FALSE = 0 } BitState;

/* Macro functions */
#define set_mode_reg(sel_internal_col, op_direction, cmd) \
  {                                                                \
    *AP_MODE = (sel_internal_col << 24) | \
               (op_direction << 8) | (cmd);                        \
  }

#define set_control_reg(ap_if_state, ap_trigger_ap, ap_rst)              \
  {                                                                      \
    *AP_CONTROL = (ap_if_state << 16) | (ap_trigger_ap << 8) | (ap_rst); \
  }

/* Prototypes */
// Utilities
void tiny_delay(uint32_t delay_in_nops);

// Reset both internal collumns
// Base startup
void warmup_ap();

void flush_col_ap();

// R/W functions
void ap_write_vector(APCollunm col, APInternalCollunm internal_col, uint8_t V[],
                     size_t size);

void ap_read_vector(APCollunm col, APInternalCollunm internal_col, uint8_t V[],
                    size_t size);

void ap_read_result_vector(APInternalCollunm internal_col, uint8_t V[],
                           size_t size);

// AP computing
void ap_computing(APOperations op, APInternalCollunm internal_col, APOpDirection op_direction, uint8_t A[],
                  uint8_t B[], size_t size);

void release_ap_if();

// AP IRQ check
volatile uint8_t ap_irq_check();

#endif
