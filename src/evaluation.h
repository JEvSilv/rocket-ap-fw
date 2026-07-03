#ifndef EVALUATION
#define EVALUATION
#include "util.h"
#include "ap_tests.h"
#include "cpu_tests.h"

uint8_t matmul_evaluation(int dimension);
void sgemm_evaluation();
int saxpy_evaluation(int n);
void reduce_evaluation();
int index_evaluation(int n);
void filter_index_evaluation();
void evm_evaluation();
void hamming_evaluation();
void bitcount_evaluation();
void checksum_evaluation();
int relu_evaluation(int n);


void buffer_load_evaluation(int dimension) {
	uint32_t size = dimension * dimension;
	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		r_v_mgmt.A[i] = i;
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.B[i] = i;
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = i;
	}
	end_compute_cycles();
}

uint8_t matmul_evaluation(int dimension) {
	fill_random_vectors(MULT, 11);
	init_accum_cycles(0);
	init_accum_cycles(1);
	//start_compute_cycles();
	//ap_mm(dimension, dimension, dimension);
	//ap_mm_v2(dimension, dimension, dimension);
	//ap_mm_v3(dimension, dimension, dimension);
	ap_mm_v4(dimension, dimension, dimension);
	//end_compute_cycles();
	ap_monitor(10);
	//memcpy(r_v_mgmt.C, AP_CAM_A_1, RANDOM_VECTORS_SIZE);
	volatile uint8_t ap_result = checksum(r_v_mgmt.C, RANDOM_VECTORS_SIZE);

	memset(r_v_mgmt.C, 0, RANDOM_VECTORS_SIZE);

	start_compute_cycles();
	matmul_cpu(dimension, dimension, dimension);
	end_compute_cycles();

	volatile uint8_t cpu_result = checksum(r_v_mgmt.C, RANDOM_VECTORS_SIZE);

	if (ap_result == cpu_result) {
		return 1;
	}

	return 0;
}

void clear_buffer(int size) {
	volatile uint8_t *output_address = (uint8_t*) 0x8000000;
	for (int i = 0; i < size; i++) {
		output_address[i] = 0;
	}
}

int filter_evaluation(int size) {
	uint32_t input_address = 0x80003000;
	uint32_t output_address = 0x80002000;

	fill_ap_with_bayer_filter();
	//clear_buffer(size*size);

	if (size <= 32) {
		ap_process_bayer_matrix_naive(input_address, output_address, 0, size);
	} else {
		volatile int total_pixels = size * size;
		volatile uint32_t CAM_A_SIZE = (AP_COL_SIZE * 2);
		volatile uint32_t CAM_MAX_CAMERA_SIZE = 32;

		while (total_pixels > 0) {
			ap_process_bayer_matrix_naive(input_address, output_address, 0,
					CAM_MAX_CAMERA_SIZE);
			total_pixels = total_pixels - CAM_A_SIZE;
			input_address += CAM_A_SIZE;
			output_address += CAM_A_SIZE;
		}
	}

	volatile uint8_t ap_result = checksum(output_address, size * size);

	//clear_buffer(size*size);

	start_compute_cycles();
	process_bayer_matrix(input_address, 0x8000000, size, size * size);
	end_compute_cycles();
	volatile uint8_t cpu_result = checksum(output_address, size * size);

	if (cpu_result == ap_result) {
		return 1;
	}

	return 0;

}

int index_evaluation(int n) {
	volatile uint32_t a = 0x80002000;
	volatile uint32_t b = 0x80003000;
	volatile uint32_t c = 0x80003800;

	fill_ap_index();

	if (n <= 512) {
		ap_index(n, 0, LEFT);
	} else {
		volatile int index = 0;
		volatile int total = n;
		int slice = AP_COL_SIZE;
		while (index < n) {
			ap_index(slice, index, LEFT);
			total = total - AP_COL_SIZE;
			if (total < AP_COL_SIZE) {
				slice = total;
			}
			index = index + AP_COL_SIZE;
		}
	}

	volatile uint8_t ap_result = checksum(a, n);

	start_compute_cycles();
	index_golden(n);
	end_compute_cycles();

	volatile uint8_t cpu_result = checksum(a, n);

	if (cpu_result == ap_result) {
		return 1;
	}

	return 0;

}

int relu_evaluation(int n) {
	volatile uint32_t result = 0x80002000;

	volatile uint8_t ap_result = checksum(result, n);


	relu_golden(n);


	volatile uint8_t cpu_result = checksum(result, n);

	if (cpu_result == ap_result) {
		return 1;
	}

	return 0;
}

int saxpy_evaluation(int n) {
	volatile uint32_t result = 0x80002000;
	volatile uint32_t x = 0x80003000;
	volatile uint32_t y = 0x80003800;

//	if (n == 1024) {
//		ap_saxpy(result, x, y, AP_COL_SIZE);
//		warmup_ap();
//		ap_saxpy(result + AP_COL_SIZE, x + AP_COL_SIZE, y + AP_COL_SIZE, AP_COL_SIZE);
//	}
//
//	if (n == 2048) {
//		ap_saxpy(result, x, y, AP_COL_SIZE);
//		ap_saxpy(result + AP_COL_SIZE, x + AP_COL_SIZE, y + AP_COL_SIZE, AP_COL_SIZE);
//		ap_saxpy(result + (AP_COL_SIZE*2), x + (AP_COL_SIZE*2), y + (AP_COL_SIZE*2), AP_COL_SIZE);
//		ap_saxpy(result + (AP_COL_SIZE*3), x + (AP_COL_SIZE*3), y + (AP_COL_SIZE*3), AP_COL_SIZE);
//	}
//
//	if(n < 512) {
//		ap_saxpy(result, x, y, n);
//	}
	start_compute_cycles();
	volatile int k = 10;
	end_compute_cycles();
	//ap_saxpy(result, x, y, n);

	// 512
	//ap_saxpy(result, x, y, 480);
	//ap_saxpy(result, x, y, 32);

	// 1024
//	ap_saxpy(result, x, y, 480);
//	ap_saxpy(result, x, y, 480);
//	ap_saxpy(result, x, y, 64);

	//2048
	ap_saxpy(result, x, y, 480);
	ap_saxpy(result, x, y, 480);
	ap_saxpy(result, x, y, 480);
	ap_saxpy(result, x, y, 480);
	ap_saxpy(result, x, y, 128);


	volatile uint8_t ap_result = checksum(result, n);

//	start_compute_cycles();
//	// 512
//	//saxpy_golden(result, x, y, 480);
//	//saxpy_golden(result, x, y, 32);
//
//	// 1024
//	//saxpy_golden(result, x, y, 480);
//	//saxpy_golden(result, x, y, 480);
//	//saxpy_golden(result, x, y, 64);
//
//	saxpy_golden(result, x, y, 480);
//	saxpy_golden(result, x, y, 480);
//	saxpy_golden(result, x, y, 480);
//	saxpy_golden(result, x, y, 480);
//	saxpy_golden(result, x, y, 128);
//	end_compute_cycles();



	volatile uint8_t cpu_result = checksum(result, n);

	if (cpu_result == ap_result) {
		return 1;
	}

	return 0;
}


#endif
