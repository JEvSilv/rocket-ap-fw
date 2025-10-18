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
#include "env_tests.h"
#include "ap_hal.h"

int ap_computing_vertical_test(APOperations op, APInternalCollunm internal_col,
		uint8_t seed) {
	uint8_t *A = (uint8_t*) malloc(AP_COL_SIZE);
	uint8_t *B = (uint8_t*) malloc(AP_COL_SIZE);
	uint8_t *C = (uint8_t*) malloc(AP_COL_SIZE);

	for (int i = 0; i < 10; i++) {
		A[i] = i;
	}

	ap_vertical_computing(op, CAM_B, internal_col, A, 10);

	while (ap_irq_check() == 0) {
	}

	*AP_CONTROL = 0x0;

	ap_read_vector(CAM_C, internal_col, C, AP_COL_SIZE);

	free(A);
	free(B);
	free(C);

	return 1;
}

int ap_computing_horizontal_test(APOperations op,
		APInternalCollunm internal_col, uint8_t seed) {
	volatile uint8_t *A = (volatile uint8_t*) malloc(AP_COL_SIZE);
	volatile uint8_t *B = (volatile uint8_t*) malloc(AP_COL_SIZE);
	volatile uint8_t *C = (volatile uint8_t*) malloc(AP_COL_SIZE);
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
	while (ap_irq_check() == 0) {
	}
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

void asm_memory_fun_tests() {
	ap_store_data(CAM_B_0_BASE_ADDR, 10);
	volatile uint8_t *dtim = (volatile uint8_t*) 0x80000000;
	*dtim = 0xf;
	ap_get_data_to(0x80000000, CAM_B_0_BASE_ADDR);
	volatile int x = 10;
}

void functional_test() {
	uint8_t *a = (volatile uint8_t*) malloc(5);
	uint8_t *b = (volatile uint8_t*) malloc(5);

	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	a[3] = 4;
	a[4] = 5;
	b[0] = 5;
	b[1] = 4;
	b[2] = 3;
	b[3] = 2;
	b[4] = 1;

	volatile uint8_t c[5] = { 0, 0, 0, 0, 0 };
	volatile uint8_t *cam_a = (volatile uint8_t*) CAM_A_0_BASE_ADDR;

	for (int i = 0; i < 5; i++) {
		cam_a[i] = a[i];
	}

	volatile uint8_t *cam_b = (volatile uint8_t*) CAM_B_0_BASE_ADDR;

	for (int i = 0; i < 5; i++) {
		cam_b[i] = b[i];
	}

	// Block interface
	*AP_CONTROL = 0x10000;

	// Setting internal collumn
	set_mode_reg(0, 0, 0, XOR);

	// Trigger ap computation
	*AP_CONTROL = 0x10000 | (1 << 8);

	// Wait to finish AP computation
	while (ap_irq_check() == 0) {
	}

	// Release block interface
	*AP_CONTROL = 0x0;

	// [TODO]: Avoid set mode reg by changing the cols automatically
	volatile uint8_t *cam_c = (volatile uint8_t*) CAM_C_0_BASE_ADDR;
	set_mode_reg(0, CAM_C, 0, 0);

	for (int i = 0; i < 5; i++) {
		c[i] = cam_c[i];
		tiny_delay(1);
		save_result(c[i]);
	}
}

void vector_ap_op_kernel_horizontal(APOperations op, uint8_t *A, uint8_t *B,
		uint8_t *C, uint32_t size) {
	int slices = (size / AP_COL_SIZE) + 1;
	// int slices = 1;

	for (int i = 0; i < slices; i++) {
		int start_index = AP_COL_SIZE * i;
		int chunk_size =
				(size < start_index + AP_COL_SIZE) ?
						size % AP_COL_SIZE : AP_COL_SIZE;

		ap_write_vector(CAM_A, LEFT, &A[start_index], chunk_size);
		ap_write_vector(CAM_B, LEFT, &B[start_index], chunk_size);
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		ap_read_vector(CAM_C, LEFT, C, chunk_size);
	}
}

void fill_a_col(uint8_t factor, int size) {
	volatile uint8_t *cam_a = (volatile uint8_t*) CAM_A_0_BASE_ADDR;

	for (int i = 0; i < size; i++) {
		cam_a[i] = i;
	}
}

void fill_a_b_cols(uint8_t *A, uint8_t *B, int size) {
	volatile uint8_t *cam_a = (volatile uint8_t*) CAM_A_0_BASE_ADDR;
	volatile uint8_t *cam_b = (volatile uint8_t*) CAM_B_0_BASE_ADDR;

	for (int i = 0; i < size; i++) {
		cam_a[i] = A[i];
	}

	for (int i = 0; i < size; i++) {
		cam_b[i] = B[i];
	}
}

void offload(int size) {
	volatile uint8_t *data = (uint8_t*) 0x80000100;
	volatile uint8_t *cam_c = (volatile uint8_t*) CAM_C_0_BASE_ADDR;

	set_mode_reg(LEFT, CAM_C, 0, 0);

	for (int i = 0; i < size; i++) {
		data[i] = cam_c[i];
	}
}

void ap_monitor_breakpoint() {
	//	Lines of code to check the AP internal state
	set_mode_reg(LEFT, CAM_A, 0, 0);
	set_mode_reg(RIGHT, CAM_A, 0, 0);
	set_mode_reg(LEFT, CAM_B, 0, 0);
	set_mode_reg(RIGHT, CAM_B, 0, 0);
	set_mode_reg(LEFT, CAM_C, 0, 0);
	set_mode_reg(RIGHT, CAM_C, 0, 0);
	offload(10);
}

void accum() {
	fill_a_col(1, 10);
	ap_trigger_vertical_computing_w_wait(ADD, CAM_A, LEFT);
	ap_monitor_breakpoint();
}

//Problema no gerador de vetores randomicos
void vector_ap_op_horizontal(APOperations op, int size, int seed) {
	fill_random_vectors(op, size);
	uint8_t *A = r_v_mgmt.A;
	uint8_t *B = r_v_mgmt.B;

	if (size <= AP_COL_SIZE) {
		fill_a_b_cols(A, B, size);
		start_compute_cycles();
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		end_compute_cycles();
	}

	if (size > AP_COL_SIZE && size <= AP_COL_SIZE * 2) {
		fill_a_b_cols(A, B, size);
		start_compute_cycles();
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		ap_trigger_computing_w_wait(op, HORIZONTAL, RIGHT);
		end_compute_cycles();
	}

	if (size == 2048) {
		fill_a_b_cols(A, B, 1024);
		start_compute_cycles();
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		ap_trigger_computing_w_wait(op, HORIZONTAL, RIGHT);
		offload(1024);
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		ap_trigger_computing_w_wait(op, HORIZONTAL, RIGHT);
		end_compute_cycles();
	}

	if (size == 4096) {
		fill_a_b_cols(A, B, 1024);
		start_compute_cycles();
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		ap_trigger_computing_w_wait(op, HORIZONTAL, RIGHT);
		offload(1024);
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		ap_trigger_computing_w_wait(op, HORIZONTAL, RIGHT);
		offload(1024);
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		ap_trigger_computing_w_wait(op, HORIZONTAL, RIGHT);
		offload(1024);
		ap_trigger_computing_w_wait(op, HORIZONTAL, LEFT);
		ap_trigger_computing_w_wait(op, HORIZONTAL, RIGHT);
		end_compute_cycles();
	}

	ap_monitor_breakpoint();

	warmup_ap();
}

void test_offload(int size) {
	set_mode_reg(0, CAM_C, 0, 0);
	volatile uint8_t *cam_c = (volatile uint8_t*) CAM_C_0_BASE_ADDR;
	uint8_t *data = (uint8_t*) 0x80000300;
	uint8_t *v = (uint8_t*) malloc(size);

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		cam_c[i] = i;
	}
	end_compute_cycles();

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		data[i] = cam_c[i];
	}
	end_compute_cycles();

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		v[i] = cam_c[i];
	}
	end_compute_cycles();

	volatile int i = 10;

	return;
}

void accum_data_manipulation(int size) {
	int half = size / 2;
	unsigned int src_half = CAM_C_0_BASE_ADDR + half;
	unsigned int src = CAM_C_0_BASE_ADDR;

	for(int i = 0; i < half; i++) {
		ap_get_data_to(CAM_A_0_BASE_ADDR + i, src + i);
		ap_get_data_to(CAM_B_0_BASE_ADDR + i, src_half + i);
	}
}

void accumulate_horizontal() {
	for (int i = 0; i < 512; i++) {
		ap_store_data(CAM_A_0_BASE_ADDR + i, 1);
		ap_store_data(CAM_B_0_BASE_ADDR + i, 1);
	}

	ap_monitor_breakpoint();

	start_compute_cycles();
	for (volatile int i = 512; i > 0; i = i / 2) {
		ap_trigger_computing_w_wait(ADD, HORIZONTAL, LEFT);
		ap_monitor_breakpoint();
		// reset ou zerar a coluna C
		accum_data_manipulation(i);
	}
	end_compute_cycles();
}

void accumulate_vertical() {

}

// Tests
// Test vector AP op
void test_vector_ap_op_horizontal() {
	const int sizes[8] = { 32, 64, 128, 256, 512, 1024, 2048, 4096 };
	APOperations op = NOT;
	vector_ap_op_horizontal(op, sizes[0], 14);
	vector_ap_op_horizontal(op, sizes[1], 14);
	vector_ap_op_horizontal(op, sizes[2], 14);
	vector_ap_op_horizontal(op, sizes[3], 14);
	vector_ap_op_horizontal(op, sizes[4], 14);
	vector_ap_op_horizontal(op, sizes[5], 14);
	vector_ap_op_horizontal(op, sizes[6], 14);
	vector_ap_op_horizontal(op, sizes[7], 14);
}

//void ap_saxpy(int n, uint8_t a, uint8_t *x, uint8_t *y) {
//	ap_set_value(CAM_B, LEFT, a);
//	ap_trigger_computing(MULT, LEFT, HORIZONTAL, TARGET_C);
//	ap_flush_col(CAM_A, LEFT);
//	for (int i = 0; i < n; i++) {
//		AP_CAM_B[i] = y[i];
//	}
//	ap_trigger_computing(ADD, LEFT, HORIZONTAL, TARGET_A);
//}
//
//// Maybe put some starting point
//void ap_index(uint8_t *a, uint8_t *b, uint8_t *c, int n) {
//	for(int i = 0; i < n; i++) {
//		AP_CAM_A[i] = c[i];
//		AP_CAM_B[i] = i%15;
//	}
//	ap_trigger_computing(MULT, LEFT, HORIZONTAL, TARGET_C);
//	// ap_flush_col(CAM_B, LEFT);
//	for(int i = 0; i < n; i++) {
//		AP_CAM_B[i] = b[i];
//	}
//	ap_trigger_computing(ADD, LEFT, HORIZONTAL, TARGET_A);
//}

void ap_accum() {
	for(int i = 0; i < 10; i++) {
//		AP_CAM_A[i] = 1;
//		tiny_delay(1);
		AP_CAM_B[i] = 1;
		tiny_delay(1);
		AP_CAM_C[i] = 1;
	}

	ap_trigger_computing(ADD, CAM_A, LEFT, HORIZONTAL, TARGET_C);
	ap_trigger_computing(ADD, CAM_A, LEFT, HORIZONTAL, TARGET_C);
	ap_trigger_computing(ADD, CAM_A, LEFT, HORIZONTAL, TARGET_C);

}

// Maybe put some starting point
void ap_reduce(uint8_t *a, uint8_t *b, uint8_t *result_sum,
		uint8_t *result_count, int n) {
	ap_flush_col(CAM_C, LEFT);
	for(int i = 0; i < n; i++) {
		AP_CAM_A[i] = a[i];
	}
	ap_search(3, LEFT, TARGET_C);
	ap_flush_col(CAM_A, LEFT);
	ap_search(0, LEFT, TARGET_C);
	ap_flush_col(CAM_C, LEFT);
	for(int i = 0; i < n; i++) {
		AP_CAM_B[i] = a[i];
	}
}

void ap_search_test() {
	for(int i = 0; i < 10; i++) {
		AP_CAM_A[i] = i;
	}
	ap_search(3, LEFT, TARGET_C);
	for(int i = 0; i < 10; i++) {
		volatile uint8_t *x = AP_CAM_C[i];
	}
	ap_search(5, LEFT, TARGET_C);
	for(int i = 0; i < 10; i++) {
		volatile uint8_t *x = AP_CAM_C[i];
	}
	ap_search(7, LEFT, TARGET_C);
	for(int i = 0; i < 10; i++) {
		volatile uint8_t *x = AP_CAM_C[i];
	}
}

#endif /* AP_TESTS_H_ */
