#ifndef AP_HAL
#define AP_HAL

#include <stdint.h>
#include <stddef.h>

/* Defines */
#define AP_INPUT_INTERFACE_ADDR 0x80003FEC
#define AP_ADDR_IN (volatile uint8_t *)(AP_INPUT_INTERFACE_ADDR)
#define AP_DATA_IN (volatile uint8_t *)(AP_INPUT_INTERFACE_ADDR + 1)
#define AP_SEL_COL (volatile uint8_t *)(AP_INPUT_INTERFACE_ADDR + 2)
#define AP_SEL_INT_COL (volatile uint8_t *)(AP_INPUT_INTERFACE_ADDR + 3)

#define AP_OUTPUT_INTERFACE_ADDR 0x80003FF0
#define AP_DATA_OUT (volatile uint8_t *)(AP_OUTPUT_INTERFACE_ADDR)

#define AP_MODE_CONFIGURATION_ADDR 0x80003FF4
#define AP_MODE (volatile uint8_t *)(AP_MODE_CONFIGURATION_ADDR)
#define AP_OP_DIRECTION (volatile uint8_t *)(AP_MODE_CONFIGURATION_ADDR + 1)
#define AP_CMD (volatile uint8_t *)(AP_MODE_CONFIGURATION_ADDR + 2)

#define AP_CONTOL_SETTINGS_ADDR 0x80003FF8
#define AP_RST (volatile uint8_t *)(AP_CONTOL_SETTINGS_ADDR)
#define AP_WRITE_EN (volatile uint8_t *)(AP_CONTOL_SETTINGS_ADDR + 1)
#define AP_READ_EN (volatile uint8_t *)(AP_CONTOL_SETTINGS_ADDR + 2)

#define AP_IRQ_ADDR 0x80003FFC
#define AP_IRQ (volatile uint8_t *)(AP_IRQ_ADDR)

#define AP_CAM_SIZE 128

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

/* Prototypes */
// Utilities
void tiny_delay(uint32_t delay_in_nops);

// Reset: simple set and release
void reset_ap();

// Reset both internal collumns
// Base startup
void warmup_ap();

// R/W functions
void ap_set_mode(APMode mode);

void ap_set_cols(APCollunm sel_col, APInternalCollunm internal_col);

void ap_write(uint32_t addr, uint8_t value);

void ap_write_vector(APCollunm col, APInternalCollunm internal_col, uint8_t V[],
                     size_t size);

uint8_t ap_read(uint32_t addr);

void ap_read_vector(APCollunm col, APInternalCollunm internal_col, uint8_t V[],
                    size_t size);

void ap_read_result_vector(APInternalCollunm internal_col, uint8_t V[],
                           size_t size);

// AP computing
void set_op_direction(APOpDirection op_direction);

void trigger_ap_computing(APOperations op);

void ap_computing(APOperations op, APInternalCollunm internal_col, uint8_t A[],
                  uint8_t B[], size_t size);

// AP IRQ check
uint8_t ap_irq_check();

#endif
