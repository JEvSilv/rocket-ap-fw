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
#include <string.h>

#include "util.h"
#include "env_tests.h"
#include "ap_hal.h"

// Prototypes
int ap_computing_vertical_test(APOperations op, APInternalCollunm internal_col,
		uint8_t seed);
int ap_computing_horizontal_test(APOperations op,
		APInternalCollunm internal_col, uint8_t seed);
void pointer_ap_rw_test();
void testing_internal_cols();
void api_r_w_ap_test();
void asm_memory_fun_tests();
void functional_test();
void vector_ap_op_kernel_horizontal(APOperations op, uint8_t *A, uint8_t *B,
		uint8_t *C, uint32_t size);
void fill_a_col(uint8_t factor, int size);
void fill_a_b_cols(uint8_t *A, uint8_t *B, int size);
void offload(int size);
void ap_monitor_breakpoint();
void accum();
void vector_ap_op_horizontal(APOperations op, int size, int seed);
void test_offload(int size);
void accum_data_manipulation(int size);
void accumulate_horizontal();
void accumulate_vertical();
void test_vector_ap_op_horizontal();
void ap_accum();
void ap_reduce(uint8_t *a, uint8_t *b, uint8_t *result_sum,
		uint8_t *result_count, int n);
void ap_search_test();
void ap_set_test();

#pragma GCC push_options
#pragma GCC optimize ("O2")
void ap_mm(int m_len, int k_len, int n_len);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void ap_mm_v2(int m_len, int k_len, int n_len);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void ap_mm_v3(int m_len, int k_len, int n_len);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O3")
void mm_cols_setup(int m_len, int n_len, int col_a_index);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O3")
void ap_mm_v4(int m_len, int k_len, int n_len);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void ap_process_bayer_matrix_naive(uint32_t i_address, uint32_t o_address, int total_pixels, int size);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void ap_offload_camera_after_filter(uint32_t o_address, int size);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void load_camera_image(uint32_t i_address, int size);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void fill_ap_with_bayer_filter();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void ap_index(int n, uint32_t index, APInternalCollunm ic);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("Os")
void ap_index_offload(uint32_t a_adress, int n);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("Os")
void fill_ap_index();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("Os")
void load_ap_b_by_addr(int32_t addr, int n);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("Os")
void load_ap_a_by_addr(int32_t addr, int n);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("Os")
void ap_saxpy(uint32_t a_adress, uint32_t b_adress, uint32_t c_adress, int n);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void ap_hamming(int n);
#pragma GCC pop_options

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
	volatile uint8_t *cam_c = (volatile uint8_t *) CAM_C_0_BASE_ADDR;
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

	//ap_read_vector(CAM_C, internal_col, C, AP_COL_SIZE);

	int index_error = 0;

	for (int i = 0; i < AP_COL_SIZE; i++) {
		uint8_t a = A[i];
		uint8_t b = B[i];

		volatile uint8_t ap_op = cam_c[i];
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
	//set_mode_reg(0, CAM_C, 0, 0);
	volatile uint8_t *cam_c = (volatile uint8_t*) CAM_C_0_BASE_ADDR;
	volatile uint8_t *data = (volatile uint8_t*) 0x80000200;
	uint8_t *v = (uint8_t*) malloc(size);

	for (int i = 0; i < size; i++) {
		data[i] = 0;
	}

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
	for(int i = 0; i < AP_COL_SIZE; i++) {
		AP_CAM_A[i] = 5;
	}

	AP_CAM_A[10] = 10;
	start_compute_cycles();
	ap_search(10, LEFT, TARGET_C);
	end_compute_cycles();

	for(int i = 0; i < AP_COL_SIZE; i++) {
		r_v_mgmt.A[i] = AP_CAM_C[i];
	}

//	for(int i = 0; i < 10; i++) {
//		AP_CAM_A[i] = i;
//	}
//	ap_search(3, LEFT, TARGET_C);
//	for(int i = 0; i < 10; i++) {
//		r_v_mgmt.A[i] = AP_CAM_C[i];
//	}
//	ap_search(5, LEFT, TARGET_C);
//	for(int i = 0; i < 10; i++) {
//		r_v_mgmt.A[i] = AP_CAM_C[i];
//	}
//	ap_search(7, LEFT, TARGET_C);
//	for(int i = 0; i < 10; i++) {
//		r_v_mgmt.A[i] = AP_CAM_C[i];
//	}
	return;
}

void ap_set_test() {
	//start_compute_cycles();
	ap_set_value(CAM_A, LEFT, 10);
	//end_compute_cycles();
	start_compute_cycles();
	memset(r_v_mgmt.A, 'A', RANDOM_VECTORS_SIZE);
	memset(r_v_mgmt.B, 'B', RANDOM_VECTORS_SIZE);
	end_compute_cycles();
}

void ap_memcpy_test() {
	uint8_t str1[10] = "abcdefghij";
	start_compute_cycles();
	//memcpy(AP_CAM_A, str1, sizeof(uint8_t)*10);
	memcpy(AP_CAM_A, r_v_mgmt.A, RANDOM_VECTORS_SIZE);
	end_compute_cycles();
}


void ap_matrix_col_mm(uint8_t factor, size_t bytes, int iterator) {
	// SET FACTOR
	ap_set_value_cam_a_left(factor);

	// MULT
	ap_trigger_computing_w_wait(MULT, HORIZONTAL, LEFT);

	// COPY RESULT
	ap_memcpy(AP_CAM_A_1, AP_CAM_C, bytes);

	// ROW ACCUM
	ap_trigger_computing_w_wait(ADD_D, HORIZONTAL, RIGHT);

	ap_flush_c_0();
}

void ap_test_add_d() {
	AP_CAM_A_1[0] = 2;
	AP_CAM_A_1[1] = 6;
	AP_CAM_A_1[2] = 2;

	ap_trigger_computing_w_wait(ADD_D, HORIZONTAL, RIGHT);

	AP_CAM_A_1[0] = 3;
	AP_CAM_A_1[1] = 9;
	AP_CAM_A_1[2] = 3;

	ap_trigger_computing_w_wait(ADD_D, HORIZONTAL, RIGHT);
}


void ap_mm(int m_len, int k_len, int n_len) {
	//A = {
	// 2 3 1
	// 1 3 2
	// 3 1 3
	// };

//	r_v_mgmt.A[0] = 2;
//	r_v_mgmt.A[1] = 3;
//	r_v_mgmt.A[2] = 1;
//	r_v_mgmt.A[3] = 1;
//	r_v_mgmt.A[4] = 3;
//	r_v_mgmt.A[5] = 2;
//	r_v_mgmt.A[6] = 3;
//	r_v_mgmt.A[7] = 1;
//	r_v_mgmt.A[8] = 3;

	//B = {
	// 1 2
	// 3 2
	// 1 3
	// };

//	r_v_mgmt.B[0] = 1;
//	r_v_mgmt.B[1] = 2;
//	r_v_mgmt.B[2] = 3;
//	r_v_mgmt.B[3] = 2;
//	r_v_mgmt.B[4] = 1;
//	r_v_mgmt.B[5] = 3;

//	start_compute_cycles();
	for(int i = 0; i < m_len; i++) {
		for(int j = 0; j < k_len; j++) {
			ap_memcpy(AP_CAM_B, &r_v_mgmt.B[j*n_len], n_len);
			ap_matrix_col_mm(r_v_mgmt.A[(i*k_len)+j], n_len, j);
		}
		ap_memcpy(&r_v_mgmt.C[i*n_len], AP_CAM_C_1, n_len);
		ap_flush_c_1();
	}
//	end_compute_cycles();
}

void ap_matrix_col_mm_window(uint8_t factor, size_t bytes) {
	// SET FACTOR
	ap_set_value_cam_a_left(factor);

	// MULT
	ap_trigger_computing_w_wait(MULT, HORIZONTAL, LEFT);

	// ROW ACCUM // TARGET_A
	ap_trigger_computing_w_wait_target_a(ADD_D, HORIZONTAL, 0b011);

	ap_flush_c_0();
}

void ap_matrix_col_mm_window_2(uint8_t factor, size_t bytes) {
	// SET FACTOR
	ap_set_value_cam_a_left(factor);

	// MULT
	ap_trigger_computing_w_wait(MULT, HORIZONTAL, LEFT);

	// ROW ACCUM // TARGET_A
	ap_trigger_computing_w_wait_target_a(ADD_D, HORIZONTAL, 0b011);

	ap_flush_c_0();
}

void ap_mm_v2(int m_len, int k_len, int n_len) {
	//A = {
	// 2 3 1
	// 1 3 2
	// 3 1 3
	// };

//	r_v_mgmt.A[0] = 2;
//	r_v_mgmt.A[1] = 3;
//	r_v_mgmt.A[2] = 1;
//	r_v_mgmt.A[3] = 1;
//	r_v_mgmt.A[4] = 3;
//	r_v_mgmt.A[5] = 2;
//	r_v_mgmt.A[6] = 3;
//	r_v_mgmt.A[7] = 1;
//	r_v_mgmt.A[8] = 3;

	//B = {
	// 1 2
	// 3 2
	// 1 3
	// };

//	r_v_mgmt.B[0] = 1;
//	r_v_mgmt.B[1] = 2;
//	r_v_mgmt.B[2] = 3;
//	r_v_mgmt.B[3] = 2;
//	r_v_mgmt.B[4] = 1;
//	r_v_mgmt.B[5] = 3;
//
//
//	int m_len = 3;
//	int k_len = 3;
//	int n_len = 2;

	for(int i = 0; i < m_len; i++) {
		for(int j = 0; j < k_len; j++) {
			ap_memcpy(AP_CAM_B, &r_v_mgmt.B[j*n_len], n_len);
			ap_matrix_col_mm_window(r_v_mgmt.A[(i*k_len)+j], n_len);
		}
		ap_memcpy(&r_v_mgmt.C[i*n_len], AP_CAM_A_1, n_len);
		ap_flush_a_1();
	}
}

void ap_mm_v3(int m_len, int k_len, int n_len) {
	//A = {
	// 2 3 1
	// 1 3 2
	// 3 1 3
	// };

//	r_v_mgmt.A[0] = 2;
//	r_v_mgmt.A[1] = 3;
//	r_v_mgmt.A[2] = 1;
//	r_v_mgmt.A[3] = 1;
//	r_v_mgmt.A[4] = 3;
//	r_v_mgmt.A[5] = 2;
//	r_v_mgmt.A[6] = 3;
//	r_v_mgmt.A[7] = 1;
//	r_v_mgmt.A[8] = 3;

	//B = {
	// 1 2
	// 3 2
	// 1 3
	// };
//
//	r_v_mgmt.B[0] = 1;
//	r_v_mgmt.B[1] = 2;
//	r_v_mgmt.B[2] = 3;
//	r_v_mgmt.B[3] = 2;
//	r_v_mgmt.B[4] = 1;
//	r_v_mgmt.B[5] = 3;
//
//
//	int m_len = 3;
//	int k_len = 3;
//	int n_len = 2;

	for(int i = 0; i < m_len; i++) {
		for(int j = 0; j < k_len; j++) {
			ap_memcpy(&AP_CAM_B[i*n_len], &r_v_mgmt.B[j*n_len], n_len);
			ap_matrix_col_mm_window_2(r_v_mgmt.A[(i*k_len)+j], n_len);
		}
	}
}

void mm_cols_setup(int m_len, int n_len, int col_a_index) {
	uint32_t row_b = col_a_index*n_len;
	for(int i = 0; i < m_len; i++) {
		uint32_t index_base = i * n_len;
		uint32_t col_base = i * m_len;
		for(int j = 0; j < n_len; j++) {
			AP_CAM_A[index_base+j] = r_v_mgmt.A[col_base+col_a_index];
			AP_CAM_B[index_base+j] = r_v_mgmt.B[row_b+j];
		}
	}
}

void mm_cols_setup_2(int c_size, int m_len, int n_len, int k_len, int mm_index) {
    uint32_t a_index = mm_index;
    uint32_t b_base  = mm_index * n_len;

    for (int i = 0; i < c_size; i++) {
        int col = i % n_len;

        AP_CAM_A[i] = r_v_mgmt.A[a_index];
        AP_CAM_B[i] = r_v_mgmt.B[b_base + col];

        if (col == n_len - 1) {
            a_index += k_len;
        }
    }
}

void ap_mm_v4(int m_len, int k_len, int n_len) {
	//A = {
	// 2 3 1
	// 1 3 2
	// 3 1 3
	// };

//	r_v_mgmt.A[0] = 2;
//	r_v_mgmt.A[1] = 3;
//	r_v_mgmt.A[2] = 1;
//	r_v_mgmt.A[3] = 1;
//	r_v_mgmt.A[4] = 3;
//	r_v_mgmt.A[5] = 2;
//	r_v_mgmt.A[6] = 3;
//	r_v_mgmt.A[7] = 1;
//	r_v_mgmt.A[8] = 3;

	//B = {
	// 1 2
	// 3 2
	// 1 3
	// };

//	r_v_mgmt.B[0] = 1;
//	r_v_mgmt.B[1] = 2;
//	r_v_mgmt.B[2] = 3;
//	r_v_mgmt.B[3] = 2;
//	r_v_mgmt.B[4] = 1;
//	r_v_mgmt.B[5] = 3;
	uint32_t c_size = m_len * n_len;
	for(int i = 0; i < m_len; i++) {
		//start_compute_cycles();
		start_compute_cycles();
		mm_cols_setup(m_len, n_len, i);
		accum_cycles(0);
		//end_compute_cycles();
		start_compute_cycles();
		ap_trigger_computing_w_wait(MULT, HORIZONTAL, LEFT);
		ap_trigger_computing_w_wait_target_a(ADD_D, HORIZONTAL, 0b011);
		ap_flush_c_0();
		accum_cycles(1);
	}
	commit_accum_cycles(0);
	commit_accum_cycles(1);
	memcpy(r_v_mgmt.C, AP_CAM_A_1, c_size);
}

/////////////////// BAYER FILTER /////////////////////
void fill_ap_with_bayer_filter() {
	start_compute_cycles();
	uint8_t bayer_matrix[2][2] = {{0, 2},{3, 1}};
	for(int i = 0; i < AP_COL_SIZE; i++) {
		AP_CAM_B[i] = bayer_matrix[0][i & 1];
		AP_CAM_B_1[i] = bayer_matrix[1][i & 1];
	}
	ap_monitor(10);
}

void load_camera_image(uint32_t i_address, int size) {
	volatile uint8_t * input_address = (uint8_t*) i_address;
	int ap_index_0 = 0;
	int ap_index_1 = 0;
	// fill CAM_A with image values
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			int index = (i*size) + j;
			if ((i & 1) == 1) {
				AP_CAM_A_1[ap_index_1] = input_address[index];
				ap_index_1++;
			} else {
				AP_CAM_A[ap_index_0] = input_address[index];
				ap_index_0++;
			}
		}
	}
	ap_monitor(10);
}

void ap_offload_camera_after_filter(uint32_t o_address, int size) {
	volatile uint8_t * output_address = (uint8_t*) o_address;
	uint32_t ap_index_0 = 0;
	uint32_t ap_index_1 = 0;

	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			int index = (i*size) + j;
			if ((i & 1) == 1) {
				output_address[index] = AP_CAM_C_1[ap_index_1];
				ap_index_1++;
			} else {
				output_address[index] = AP_CAM_C[ap_index_0];
				ap_index_0++;
			}
		}
	}

}

void ap_process_bayer_matrix_naive(uint32_t i_address, uint32_t o_address, int total_pixels, int size) {
	start_compute_cycles();
	load_camera_image(i_address, size);
	end_compute_cycles();

	// AP computation
	start_compute_cycles();
	//ap_trigger_computing_w_wait(ADD, HORIZONTAL, LEFT);
	//ap_trigger_computing_w_wait(ADD, HORIZONTAL, RIGHT);

	ap_trigger_computing_w_wait(MULT, HORIZONTAL, LEFT);
	ap_trigger_computing_w_wait(MULT, HORIZONTAL, RIGHT);

	end_compute_cycles();


	start_compute_cycles();
	ap_offload_camera_after_filter(o_address, size);
	end_compute_cycles();
}
//////////////////////////////////////////////////////////////////

/////////////////// LOADS //////////////////////////////
#include "camera_64x64_4bit.h";

void load_ap_a_by_addr(int32_t addr, int n) {
	//volatile uint8_t * v = (uint8_t*) addr;
	for(int i = 0; i < n; i++) {
		//AP_CAM_A[i] = v[i];
		AP_CAM_A[i] = CAMERA_64x64_4BIT[i];
	}
}

void load_ap_a_1_by_addr(int32_t addr, int n) {
	volatile uint8_t * v = (uint8_t*) addr;
	for(int i = 0; i < n; i++) {
		//AP_CAM_A_1[i] = v[i];
		AP_CAM_A_1[i] = CAMERA_64x64_4BIT[i];
	}
}

void load_ap_b_by_addr(int32_t addr, int n) {
	volatile uint8_t * v = (uint8_t*) addr;
	for(int i = 0; i < n; i++) {
		//AP_CAM_B[i] = v[i];
		AP_CAM_B[i] = CAMERA_64x64_4BIT[i];
	}
}

void load_ap_b_1_by_addr(int32_t addr, int n) {
	volatile uint8_t * v = (uint8_t*) addr;
	for(int i = 0; i < n; i++) {
		//AP_CAM_B_1[i] = v[i];
		AP_CAM_B_1[i] = CAMERA_64x64_4BIT[i];
	}
}

///////////////////////////////////////////////////////

/////////////////// SAXPY KERNEL /////////////////////
// a = 2*b+c
#pragma GCC push_options
#pragma GCC optimize ("O2")
void ap_saxpy_load(uint32_t b_adress, uint32_t c_adress, int n);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("Os")
void ap_saxpy_offload(uint32_t a_adress, int n);
#pragma GCC pop_options

extern void ap_offload_asm();

//#include "camera_64x64_4bit.h"


void ap_saxpy_load(uint32_t b_adress, uint32_t c_adress, int n) {
#ifdef camera_64x64
		volatile uint8_t * b = (uint8_t*) b_adress;
		volatile uint8_t * c = (uint8_t*) c_adress;
		for(int i = 0; i < n; i++) {
		AP_CAM_A[i] = CAMERA_64x64_4BIT[i];
		AP_CAM_B_1[i] = CAMERA_64x64_4BIT[i];
	}
#endif

}

void ap_saxpy_offload(uint32_t a_adress, int n) {
	volatile uint8_t * a = (uint8_t*) a_adress;
	for(int i = 0; i < n; i++) {
//		/a[i] = AP_CAM_A_1[i];
		a[i] = AP_CAM_A[i];
	}
}

void ap_saxpy(uint32_t a_adress, uint32_t b_adress, uint32_t c_adress, int n) {
	volatile uint32_t * a = (uint32_t*) a_adress;
	volatile uint8_t * b = (uint8_t*) b_adress;
	volatile uint8_t * c = (uint8_t*) c_adress;

	start_compute_cycles();
	ap_saxpy_load(b_adress, c_adress, n);
	end_compute_cycles();

	ap_set_value_cam_b_left(2);

	start_compute_cycles();
	ap_trigger_computing_w_wait(MULT, HORIZONTAL, LEFT);
	reset_ap_engine();
	ap_trigger_computing_w_wait_target_a(ADD, HORIZONTAL, 0b011);
	end_compute_cycles();


	start_compute_cycles();
	burst_read_on();
	for(int i = 0; i < (n >> 2); i++) {
		a[i] = AP_CAM_A_1_W[i];
	}
	burst_read_off();
	end_compute_cycles();

	volatile int x = 10;

}

/////////////////// INDEX KERNEL /////////////////////


void fill_ap_index() {
	for(int i = 0; i < AP_COL_SIZE; i++) {
		AP_CAM_B_1[i] = i;
	}
}

void ap_index_offload(uint32_t a_adress, int n) {
	uint8_t * a = (uint8_t*) a_adress;
//	for(int i = 0; i < n; i++) {
//		a[i] = AP_CAM_A[i];
//	}
	//memcpy(a, AP_CAM_A, n);


	burst_read_on();
	for(int i = 0; i < (n >> 2); i++) {
		a[i] = AP_CAM_A_W[i];
	}
	burst_read_off();

}


// TODO: COLOCO SÓ EM UM BUFFER?
void ap_index(int n, uint32_t index, APInternalCollunm ic) {
	fill_ap_index();

	//ap_monitor(10);
	start_compute_cycles();
	load_ap_a_by_addr(0x80003000, n);
	end_compute_cycles();
	//ap_monitor(10);

	start_compute_cycles();
	ap_trigger_computing_w_wait(MULT, HORIZONTAL, 0b010);
	end_compute_cycles();

	//ap_monitor(10);
	start_compute_cycles();
	load_ap_a_1_by_addr(0x80003000, n);
	end_compute_cycles();

	//ap_monitor(10);
	start_compute_cycles();
	ap_trigger_computing_w_wait_target_a(ADD_D, HORIZONTAL, 0b001);
	end_compute_cycles();
	//ap_monitor(20);

	//end_compute_cycles();
	start_compute_cycles();
	ap_index_offload(0x80002000 + index, n);
	end_compute_cycles();

}
//////////////////////////////////////////////////////////////////

////////////////////////////// AP RELU //////////////////////////
// TODO: CONSTRUIR UMA SOLUÇÃO EM HW

//#include "random_vector.h"

void ap_relu(uint32_t addr, int n) {
	volatile uint32_t * a = (uint32_t*) addr;
	start_compute_cycles();
	for(int i = 0; i < n; i++) {
		//AP_CAM_A[i] = VETOR_ALEATORIO_4096[i];
	}
	end_compute_cycles();
	start_compute_cycles();
	ap_trigger_computing_w_wait(RELU, HORIZONTAL, LEFT);
	end_compute_cycles();
	start_compute_cycles();
	ap_trigger_computing_w_wait(RELU, HORIZONTAL, RIGHT);
	end_compute_cycles();


	start_compute_cycles();
	burst_read_on();
	for(int i = 0; i < 304; i++) {
		a[i] = AP_CAM_A_1_W[i];
	}
	burst_read_off();
	end_compute_cycles();
}

// Vou até 960
void ap_branch(int n) {
	volatile uint32_t * a = (uint32_t*) 0x80002000;

	start_compute_cycles();
	load_ap_a_by_addr(0x80003000, n);
	end_compute_cycles();

	start_compute_cycles();
	ap_search(0, LEFT, TARGET_C);
	ap_search(0, RIGHT, TARGET_C);
	end_compute_cycles();

	start_compute_cycles();
	load_ap_b_by_addr(0x80003000, n);
	end_compute_cycles();

	start_compute_cycles();
	ap_trigger_computing_w_wait(ADD, HORIZONTAL, RIGHT);
	ap_trigger_computing_w_wait(ADD, HORIZONTAL, RIGHT);
	end_compute_cycles();

	start_compute_cycles();
	burst_read_on();
	for(int i = 0; i < 120; i++) {
		a[i] = AP_CAM_C_W[i];
	}

	for(int i = 0; i < 120; i++) {
		a[i] = AP_CAM_C_1_W[i];
	}
	burst_read_off();
	end_compute_cycles();
}

void ap_hamming(int n) {
	volatile uint32_t * a = (uint32_t*) 0x80002000;

	start_compute_cycles();
	load_ap_a_by_addr(0x80003000, n);
	end_compute_cycles();

	start_compute_cycles();
	load_ap_b_by_addr(0x80003000, n);
	end_compute_cycles();

	start_compute_cycles();
	ap_trigger_computing_w_wait(XOR, HORIZONTAL, RIGHT);
	ap_trigger_computing_w_wait(XOR, HORIZONTAL, LEFT);
	ap_set_value_cam_b_left(0xff);
	ap_set_value_cam_b_right(0xff);
	ap_trigger_computing_w_wait_target_a(XOR, HORIZONTAL, LEFT);
	ap_trigger_computing_w_wait_target_a(XOR, HORIZONTAL, RIGHT);
	end_compute_cycles();

	start_compute_cycles();
	burst_read_on();
	for(int i = 0; i < 120; i++) {
		a[i] = AP_CAM_A_W[i];
	}

	for(int i = 0; i < 120; i++) {
		a[i] = AP_CAM_A_1_W[i];
	}
	burst_read_off();
	end_compute_cycles();
}


#endif /* AP_TESTS_H_ */
