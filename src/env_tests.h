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

void basic_tests() {
	int a = 10;
	int b = 30;
	volatile int c = a + b;
	volatile uint8_t *ap_reset_addr = (volatile uint8_t*) 0x80003FEC;

	*ap_reset_addr = 1;
	*ap_reset_addr = 0;
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
