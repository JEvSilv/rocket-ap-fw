/*
 * env_tests.h
 *
 *  Created on: 11 de fev de 2025
 *      Author: root
 */

#ifndef ENV_TESTS_H_
#define ENV_TESTS_H_
#include <stdint.h>
#include "ap_hal.h"

void basic_mem_test() {
	volatile uint8_t *flag = (volatile uint8_t*) 0x80000000;
	*flag = 1;
	volatile uint8_t x = *flag;
	flag = (volatile uint8_t*) 0x80004004;
	*flag = 10;
}

void basic_mem_test_2() {
	volatile uint8_t * ap = (volatile uint8_t *) (0x80004000);
	volatile uint8_t t;

	for(int i = 0; i < AP_COL_SIZE*6; i++) {
		ap[i] = i+1;
	}
}

void flush_test() {
	basic_mem_test_2();
	flush_col_ap(CAM_A, LEFT);
	flush_col_ap(CAM_A, RIGHT);
	flush_col_ap(CAM_B, LEFT);
	flush_col_ap(CAM_B, RIGHT);
	flush_col_ap(CAM_C, LEFT);
	flush_col_ap(CAM_C, RIGHT);
}


void basic_tests() {
	int a = 10;
	int b = 30;
	volatile int c = a + b;
	volatile uint8_t *ap_reset_addr = (volatile uint8_t*) 0x80003FEC;

	*ap_reset_addr = 1;
	*ap_reset_addr = 0;
}

void set_value_test() {
	ap_set_value(CAM_A, LEFT, 1);
	ap_set_value(CAM_A, RIGHT, 2);
	ap_set_value(CAM_B, LEFT, 3);
	ap_set_value(CAM_B, RIGHT, 4);
	ap_set_value(CAM_C, LEFT, 5);
	ap_set_value(CAM_C, RIGHT, 6);

	return;
//  volatile uint8_t *ap = (volatile uint8_t *) CAM_A_0_BASE_ADDR;
//  *ap = 0x1;
//  *AP_CONTROL = 0x1000000;
//  /* *AP_MODE = 0x0100007; */
//  set_mode_reg(0, 0, 0, 7);
//  *AP_CONTROL = 0x1000100;
//  waiting_for_ap_computing(3);
//
//  ap = (volatile uint8_t *) CAM_A_1_BASE_ADDR;
//  *ap = 0x2;
//  *AP_CONTROL = 0x1000000;
//  /* *AP_MODE = 0x0100007; */
//  set_mode_reg(1, 0, 0, 7);
//  *AP_CONTROL = 0x1000100;
//  waiting_for_ap_computing(3);
//
//  ap = (volatile uint8_t *) CAM_B_0_BASE_ADDR;
//  *ap = 0x3;
//  *AP_CONTROL = 0x1000000;
//  /* *AP_MODE = 0x0100007; */
//  set_mode_reg(0, 1, 0, 7);
//  *AP_CONTROL = 0x1000100;
//  waiting_for_ap_computing(3);
//
//
//  ap = (volatile uint8_t *) CAM_B_1_BASE_ADDR;
//  *ap = 0x4;
//  *AP_CONTROL = 0x1000000;
//  /* *AP_MODE = 0x0100007; */
//  set_mode_reg(1, 1, 0, 7);
//  *AP_CONTROL = 0x1000100;
//  waiting_for_ap_computing(3);
//
//
//  ap = (volatile uint8_t *) CAM_C_0_BASE_ADDR;
//  *ap = 0x5;
//  *AP_CONTROL = 0x1000000;
//  /* *AP_MODE = 0x0100007; */
//  set_mode_reg(0, 2, 0, 7);
//  *AP_CONTROL = 0x1000100;
//  waiting_for_ap_computing(3);
//
//
//  ap = (volatile uint8_t *) CAM_C_1_BASE_ADDR;
//  *ap = 0x6;
//  *AP_CONTROL = 0x1000000;
//  /* *AP_MODE = 0x0100007; */
//  set_mode_reg(1, 2, 0, 7);
//  *AP_CONTROL = 0x1000100;
//  waiting_for_ap_computing(3);

}

void op_target_setup() {
  volatile uint8_t *cam_b = (volatile uint8_t *) CAM_B_1_BASE_ADDR;
  volatile uint8_t *cam_c = (volatile uint8_t *) CAM_C_1_BASE_ADDR;

  for(int i = 0; i < AP_COL_SIZE; i++) {
     cam_b[i] = i % 10;
     cam_c[i] = i % 10;
  }

  set_control_reg(1, 1, 0, 0);
  set_mode_reg(1, 0, 0, ADD);
  set_control_reg(1, 1, 1, 0);

  waiting_for_ap_computing(3);

  return;
}

uint8_t ap_unit_test_sw(APOperations op, uint8_t x, uint8_t y) {
	switch (op) {
	case OR:
		return x | y;
		break;
	case XOR:
		return x ^ y;
		break;
	case AND:
		return x & y;
		break;
	case NOT:
		return ~x;
		break;
	case ADD:
		return x + y;
		break;
	case SUB:
		return x - y;
		break;
	case MULT:
		return x * y;
		break;
	default:
		break;
	}
	return 0;
}


#endif /* ENV_TESTS_H_ */
