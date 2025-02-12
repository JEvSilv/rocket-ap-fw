/*
 * ap_tests.h
 *
 *  Created on: 11 de fev de 2025
 *      Author: root
 */

#ifndef AP_TESTS_H_
#define AP_TESTS_H_

#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include "ap_hal.h"

int ap_computing_vertical_test(APOperations op,
		APInternalCollunm internal_col, uint8_t seed) {
	uint8_t *A = (uint8_t*) malloc(AP_COL_SIZE);
	uint8_t *B = (uint8_t*) malloc(AP_COL_SIZE);
	uint8_t *C = (uint8_t*) malloc(AP_COL_SIZE);

	for (int i = 0; i < 10; i++) {
		A[i] = i;
	}

	ap_vertical_computing(op, CAM_B, internal_col, A, 10);

	while (ap_irq_check() == 0) {}

	*AP_CONTROL = 0x0;

	ap_read_vector(CAM_C, internal_col, C, AP_COL_SIZE);

	free(A);
	free(B);
	free(C);

	return 1;
}

int ap_computing_horizontal_test(APOperations op,
		APInternalCollunm internal_col, uint8_t seed) {
	uint8_t *A = (uint8_t*) malloc(AP_COL_SIZE);
	uint8_t *B = (uint8_t*) malloc(AP_COL_SIZE);
	uint8_t *C = (uint8_t*) malloc(AP_COL_SIZE);
	volatile uint64_t cycle_count;
	unsigned long long start = 0, end = 0;

	volatile uint32_t mask = 0xff;

	if (op > 3) {
		mask = 0xf;
	}

	for (int i = 0; i < AP_COL_SIZE; i++) {
		A[i] = (seed - i * 1234 + i + 4321) & mask;
		B[i] = (seed + i * 4321 + i - 1234) & mask;
	}

	ap_computing(op, internal_col, HORIZONTAL, A, B, AP_COL_SIZE);
	start_compute_cycles();
	while (ap_irq_check() == 0) {}
	end_compute_cycles();
//	metal_timer_get_cyclecount(0, &end);
//	cycle_count = end - start;

	*AP_CONTROL = 0x0;

	ap_read_vector(CAM_C, internal_col, C, AP_COL_SIZE);

	int index_error = 0;

	for (int i = 0; i < AP_COL_SIZE; i++) {
		uint8_t a = A[i];
		uint8_t b = B[i];

		volatile uint8_t ap_op = C[i];
		volatile uint8_t sw_op = ap_unit_test_sw(op, a, b);

		if (ap_op != sw_op) {
			return 0;
		}
	}

	free(A);
	free(B);
	free(C);

	return 1;
}



void pointer_ap_rw_test() {
	uint32_t addr = 0x80004000;
	uint32_t quant_internal_cols = 6;
	uint32_t dtim_addr = 0x80000000;

	volatile uint8_t *ptr = (volatile uint8_t*) addr;
	volatile uint8_t *ptr_dtim = (volatile uint8_t*) dtim_addr;

	// Write
	for (int i = 0; i < AP_COL_SIZE * quant_internal_cols; i++) {
		ptr = (volatile uint8_t*) (addr + i);
		*ptr = i % 128;
		tiny_delay(1);
	}

	// Direct Read
	for (int i = 0; i < AP_COL_SIZE * quant_internal_cols; i++) {
		ptr = (volatile uint8_t*) (addr + i);
		ptr_dtim = (volatile uint8_t*) (dtim_addr + i * 4);
		*ptr_dtim = *ptr;
		tiny_delay(1);
	}

	// Passing to a register and then to DTIM
	for (int i = 0; i < AP_COL_SIZE * quant_internal_cols; i++) {
		ptr = (volatile uint8_t*) (addr + i);
		ptr_dtim = (volatile uint8_t*) (dtim_addr + i * 4);
		uint8_t tmp = *ptr;
		*ptr_dtim = tmp + 1;
		tiny_delay(1);
	}
}

void testing_internal_cols() {
	uint32_t addr = 0x80004000;
	volatile uint8_t *ptr_v = (volatile uint8_t*) addr;
	uint8_t result[10];

	for (int i = 0; i < 10; i++) {
		ptr_v[i] = i + 1;
		tiny_delay(1);
	}

	for (int i = 0; i < 10; i++) {
		result[i] = ptr_v[i];
		tiny_delay(1);
	}

	return;
}

void api_r_w_ap_test() {
	// Put the breakpoint on the loops and
	// will see the values storage on the buffer read

	uint8_t buffer[AP_COL_SIZE];
	uint8_t buffer_read[AP_COL_SIZE];

	for (int i = 0; i < AP_COL_SIZE; i++) {
		buffer[i] = i;
	}

	ap_write_vector(CAM_A, LEFT, buffer, AP_COL_SIZE);
	ap_read_vector(CAM_A, LEFT, buffer_read, AP_COL_SIZE);

	for (int i = 0; i < AP_COL_SIZE; i++) {
		buffer[i] = i + 1;
	}

	ap_write_vector(CAM_A, RIGHT, buffer, AP_COL_SIZE);
	ap_read_vector(CAM_A, RIGHT, buffer_read, AP_COL_SIZE);

	for (int i = 0; i < AP_COL_SIZE; i++) {
		buffer[i] = i + 2;
	}

	ap_write_vector(CAM_B, LEFT, buffer, AP_COL_SIZE);
	ap_read_vector(CAM_B, LEFT, buffer_read, AP_COL_SIZE);

	for (int i = 0; i < AP_COL_SIZE; i++) {
		buffer[i] = i + 3;
	}

	ap_write_vector(CAM_B, RIGHT, buffer, AP_COL_SIZE);
	ap_read_vector(CAM_B, RIGHT, buffer_read, AP_COL_SIZE);

	for (int i = 0; i < AP_COL_SIZE; i++) {
		buffer[i] = i + 4;
	}

	ap_write_vector(CAM_C, LEFT, buffer, AP_COL_SIZE);
	ap_read_vector(CAM_C, LEFT, buffer_read, AP_COL_SIZE);

	for (int i = 0; i < AP_COL_SIZE; i++) {
		buffer[i] = i + 5;
	}

	ap_write_vector(CAM_C, RIGHT, buffer, AP_COL_SIZE);
	ap_read_vector(CAM_C, RIGHT, buffer_read, AP_COL_SIZE);

	return;
}

#endif /* AP_TESTS_H_ */
