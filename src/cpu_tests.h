/*
 * cpu_tests.h
 *
 *  Created on: 11 de fev de 2025
 *      Author: root
 */

#ifndef CPU_TESTS_H_
#define CPU_TESTS_H_
#include <stdint.h>
#include <stdlib.h>
#include "util.h"

// Prototypes CPU TESTS
void sgemm_golden(int m_len, int k_len, int n_len, uint8_t *a, uint8_t *b,
		uint8_t *c);

#pragma GCC push_options
#pragma GCC optimize ("O3")
void matmul_cpu();
#pragma GCC pop_options

//#pragma GCC push_options
//#pragma GCC optimize ("O2")
void sgemm_golden_cpu_test();
//#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void saxpy_golden(uint32_t r_address, uint32_t x_adress, uint32_t y_adress, int n);
#pragma GCC pop_options

void reduce_golden(uint8_t *a, uint8_t *b, uint8_t *result_sum,
		uint8_t *result_count, int n);
void matmul_golden(uint8_t **a, uint8_t **b, uint8_t **c, int n, int m, int o);

#pragma GCC push_options
#pragma GCC optimize ("O2")
void index_golden(int n);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void vector_cpu_op_kernel_or();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void vector_cpu_op_kernel_xor();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void vector_cpu_op_kernel_and();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void vector_cpu_op_kernel_not();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void vector_cpu_op_kernel_add();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void vector_cpu_op_kernel_sub();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void vector_cpu_op_kernel_mult();
#pragma GCC pop_options

void vector_cpu_op(int8_t seed, uint32_t size);
void test_accum();

#pragma GCC push_options
#pragma GCC optimize ("O2")
void cpu_search_test();
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void process_bayer_matrix(uint32_t i_address, uint32_t o_address, uint32_t size, uint32_t total_pixels);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void process_bayer_matrix_3x3(const uint8_t *input, volatile uint8_t *output, uint32_t size, uint32_t total_pixels);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void relu_golden(int n);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void branch_golden(int n);
#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void hamming_code_golden(int n);
#pragma GCC pop_options
//-------------------------------------------------------------------------

// sgemm
void sgemm_golden(int m_len, int k_len, int n_len, uint8_t *a, uint8_t *b,
		uint8_t *c) {
	for (int i = 0; i < m_len; ++i)
		for (int j = 0; j < n_len; ++j)
			for (int k = 0; k < k_len; ++k)
				c[i * n_len + j] += a[i * k_len + k] * b[j + k * n_len];
}

void sgemm_golden_cpu_test() {
	//int m_len = 20;
	//int k_len = 20;
	//int n_len = 2;

	int m_len = 3;
	int k_len = 3;
	int n_len = 2;

	//fill_random_vectors(MULT, 10);

	//A = {
	// 2 3 1
	// 1 3 2
	// 3 1 3
	// };

	r_v_mgmt.A[0] = 2;
	r_v_mgmt.A[1] = 3;
	r_v_mgmt.A[2] = 1;
	r_v_mgmt.A[3] = 1;
	r_v_mgmt.A[4] = 3;
	r_v_mgmt.A[5] = 2;
	r_v_mgmt.A[6] = 3;
	r_v_mgmt.A[7] = 1;
	r_v_mgmt.A[8] = 3;

	//B = {
	// 1 2
	// 3 2
	// 1 3
	// };

	r_v_mgmt.B[0] = 1;
	r_v_mgmt.B[1] = 2;
	r_v_mgmt.B[2] = 3;
	r_v_mgmt.B[3] = 2;
	r_v_mgmt.B[4] = 1;
	r_v_mgmt.B[5] = 3;

	//C = {
	// 1 2
	// 1 2
	// 1 2
	// };

	r_v_mgmt.C[0] = 1;
	r_v_mgmt.C[1] = 2;
	r_v_mgmt.C[2] = 1;
	r_v_mgmt.C[3] = 2;
	r_v_mgmt.C[4] = 1;
	r_v_mgmt.C[5] = 2;

	start_compute_cycles();
	for (int i = 0; i < m_len; ++i)
		for (int j = 0; j < n_len; ++j)
			for (int k = 0; k < k_len; ++k)
				r_v_mgmt.C[i * n_len + j] += r_v_mgmt.A[i * k_len + k] * r_v_mgmt.B[j + k * n_len];
	end_compute_cycles();
}

//#include "camera_64x64_4bit.h"
// saxpy
void saxpy_golden(uint32_t r_address, uint32_t x_adress, uint32_t y_adress, int n) {
#ifdef camera_64x64
	uint8_t a = 2;
	volatile uint8_t * x = (uint8_t*) x_adress;
	volatile uint8_t * y = (uint8_t*) y_adress;
	volatile uint8_t * r = (uint8_t*) r_address;

	for (int i = 0; i < n; ++i) {
		//r[i] = a * x[i] + y[i];
		r[i] = 2*CAMERA_64x64_4BIT[i]+CAMERA_64x64_4BIT[i];
	}
#endif
}

// accumulate and reduce
void reduce_golden(uint8_t *a, uint8_t *b, uint8_t *result_sum,
		uint8_t *result_count, int n) {
	int count = 0;
	uint8_t s = 0;
	for (int i = 0; i < n; ++i) {
		if (a[i] != 3) {
			s += a[i] * b[i];
			count++;
		}
	}

	*result_sum = s;
	*result_count = count;
}

// matrix multiplication
// A[n][o], B[m][o] --> C[n][m];
void matmul_golden(uint8_t **a, uint8_t **b, uint8_t **c, int n, int m, int o) {
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			c[i][j] = 0;
			for (int k = 0; k < o; ++k) {
				c[i][j] += a[i][k] * b[j][k];
			}
		}
}

void matmul_cpu(int m_len, int k_len, int n_len) {
	//int m_len = 20;
	//int k_len = 20;
	//int n_len = 2;

	//fill_random_vectors(MULT, 10);

	//A = {
	// 2 3 1
	// 1 3 2
	// 3 1 3
	// };
//
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

    for (int i = 0; i < m_len; ++i) {
        for (int j = 0; j < n_len; ++j) {
            uint8_t sum = 0;
            for (int k = 0; k < k_len; ++k) {
                sum += r_v_mgmt.A[i * k_len + k] * r_v_mgmt.B[k * n_len + j];
            }
            r_v_mgmt.C[i * n_len + j] = sum;
        }
    }
}

void branch_golden(int n) {
  volatile uint8_t * a = (uint8_t*) 0x80002000;
  volatile uint8_t * b = (uint8_t*) 0x80003000;
  volatile uint8_t * c = (uint8_t*) 0x80003000;

  for (int i = 0; i < n; ++i) {
    c[i] = (b[i] != 0.0) ? a[i] + b[i] : 1;
  }
}

void hamming_code_golden(int n) {
  volatile uint8_t * a = (uint8_t*) 0x80002000;
  volatile uint8_t * b = (uint8_t*) 0x80003000;
  volatile uint8_t * c = (uint8_t*) 0x80003000;
  uint8_t code = 0xff;

  for (int i = 0; i < n; ++i) {
    c[i] = (a[i] ^ b[i]) ^ code;
  }
}

// index arithmetic
void index_golden(int n) {
	volatile uint8_t * a = (uint8_t*) 0x80002000;
	volatile uint8_t * b = (uint8_t*) 0x80003000;
	volatile uint8_t * c = (uint8_t*) 0x80003000;

	for (int i = 0; i < n; ++i) {
		a[i] = b[i] + i * c[i];
	}
}

void relu_golden(int n) {
	volatile uint8_t * data = (uint8_t*) 0x80003000;
    for (int i = 0; i < n; i++) {
        if (data[i] < 0) {
            data[i] = 0;
        }
    }
}

void vector_cpu_op_kernel_or() {
	fill_random_vectors(OR, RANDOM_VECTORS_SIZE);

	start_compute_cycles();
	for (int i = 0; i < RANDOM_VECTORS_SIZE; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] | r_v_mgmt.B[i];
	}

	for (int i = 0; i < RANDOM_VECTORS_SIZE; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] | r_v_mgmt.B[i];
	}

	for (int i = 0; i < RANDOM_VECTORS_SIZE; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] | r_v_mgmt.B[i];
	}

	for (int i = 0; i < RANDOM_VECTORS_SIZE; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] | r_v_mgmt.B[i];
	}
	end_compute_cycles();
}

void vector_cpu_op_kernel_xor() {
	fill_random_vectors(XOR, RANDOM_VECTORS_SIZE);
	uint32_t size = RANDOM_VECTORS_SIZE;

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] ^ r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] ^ r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] ^ r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] ^ r_v_mgmt.B[i];
	}

	end_compute_cycles();
}

void vector_cpu_op_kernel_and() {
	fill_random_vectors(AND, RANDOM_VECTORS_SIZE);
//	uint32_t size = RANDOM_VECTORS_SIZE/2;
	uint32_t size = RANDOM_VECTORS_SIZE;

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] & r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] & r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] & r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] & r_v_mgmt.B[i];
	}

	end_compute_cycles();
}

void vector_cpu_op_kernel_not() {
	fill_random_vectors(NOT, RANDOM_VECTORS_SIZE);
	//uint32_t size = RANDOM_VECTORS_SIZE/2;
	uint32_t size = RANDOM_VECTORS_SIZE;

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = ~r_v_mgmt.A[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = ~r_v_mgmt.A[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = ~r_v_mgmt.A[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = ~r_v_mgmt.A[i];
	}

	end_compute_cycles();
}

void vector_cpu_op_kernel_add() {
	fill_random_vectors(ADD, RANDOM_VECTORS_SIZE);
//	uint32_t size = RANDOM_VECTORS_SIZE/2;
	uint32_t size = RANDOM_VECTORS_SIZE;

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] + r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] + r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] + r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] + r_v_mgmt.B[i];
	}

	end_compute_cycles();
}

void vector_cpu_op_kernel_sub() {
	fill_random_vectors(SUB, RANDOM_VECTORS_SIZE);
//	uint32_t size = RANDOM_VECTORS_SIZE/2;
	uint32_t size = RANDOM_VECTORS_SIZE;

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] - r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] - r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] - r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] - r_v_mgmt.B[i];
	}

	end_compute_cycles();
}

void vector_cpu_op_kernel_mult() {
	fill_random_vectors(MULT, RANDOM_VECTORS_SIZE);
//	uint32_t size = RANDOM_VECTORS_SIZE/2;
	uint32_t size = RANDOM_VECTORS_SIZE;

	start_compute_cycles();
	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] * r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] * r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] * r_v_mgmt.B[i];
	}

	for (int i = 0; i < size; i++) {
		r_v_mgmt.C[i] = r_v_mgmt.A[i] * r_v_mgmt.B[i];
	}

	end_compute_cycles();
}

void vector_cpu_op(int8_t seed, uint32_t size) {
	fill_random_vectors(ADD, size);
	uint8_t *A = r_v_mgmt.A;
	uint8_t *B = r_v_mgmt.B;

	uint8_t *C = (volatile uint8_t*) 0x80000000;

	start_compute_cycles();
// 	vector_cpu_op_kernel_or(A, B, C, size);
// 	vector_cpu_op_kernel_xor(A, B, C, size);
// 	vector_cpu_op_kernel_and(A, B, C, size);
// 	vector_cpu_op_kernel_not(A, B, C, size);
// 	vector_cpu_op_kernel_add(A, B, C, size);
// 	vector_cpu_op_kernel_sub(A, B, C, size);
	vector_cpu_op_kernel_mult(A, B, C, size);
	end_compute_cycles();

	volatile int x = 10;
}

uint8_t accum_kernel(uint8_t *A,
		uint32_t size) {
	volatile uint8_t sum;
	for (int i = 0; i < size; i++) {
		sum += A[i];
	}

	return sum;
}

// Tests
// Test vector CPU OP
void test_vector_cpu_op() {
const int sizes[8] = { 32, 64, 128, 256, 512, 1024, 2048, 4096 };

for (int i = 0; i < 8; i++)
	vector_cpu_op(15, sizes[i]);
}

void test_accum() {
	uint8_t *A = r_v_mgmt.A;
	for(int i = 0; i < 1024; i++) {
		A[i] = 1;
	}

	start_compute_cycles();
	volatile uint8_t y = accum_kernel(A, 1024);
	end_compute_cycles();
	volatile int x = y;
	return;
}


void cpu_search_test() {
	for(int i = 0; i < 1024; i++) {
		r_v_mgmt.A[i] = 5;
	}

	r_v_mgmt.A[10] = 10;

	start_compute_cycles();
//	for(int j = 0; j < 4; j++) {
		for(int i = 0; i < 1024; i++) {
			if(r_v_mgmt.A[i] == 10) {
				r_v_mgmt.B[i] = 1;
				continue;
			}
			r_v_mgmt.B[i] = 0;
		}
//	}
	end_compute_cycles();

	return;
}


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


const uint8_t matrix_dispersed[3][3] = {
    {2, 9, 4},
    {7, 14, 12},
    {6, 11, 3}
};

// 2. Define the static Scaled 2x2 Bayer Matrix
//const uint8_t bayer_matrix[2][2] = {
//    {3, 11},
//    {15, 7}
//};

const uint8_t bayer_matrix[2][2] = {
    {0, 2},
    {3, 1}
};


/**
 * Executes the spatial matrix addition loop with hardware saturation.
 * Maps a flat 1D vector back to 2D space coordinates dynamically.
 */
void process_bayer_matrix_3x3(const uint8_t *input, volatile uint8_t *output, uint32_t size, uint32_t total_pixels) {
    for (int i = 0; i < total_pixels; i++) {
        // Reconstruct 2D row (y) and column (x) coordinates from 1D flat index
        int y = i / size;
        int x = i % size;

        uint8_t pixel_value = input[i];
        uint8_t modifier = matrix_dispersed[y % 3][x % 3];
        //uint8_t mixed_value = pixel_value + modifier;
        uint8_t mixed_value = pixel_value * modifier;
        output[i] = mixed_value;
    }
}


void process_bayer_matrix(uint32_t i_address, uint32_t o_address, uint32_t size, uint32_t total_pixels) {
	volatile uint8_t * output_address = (uint8_t*) 0x80002000;
	volatile uint8_t * input_address = (uint8_t*) 0x80003000;

	for (int i = 0; i < total_pixels; i++) {
        int y = i / size;
        int x = i % size;

        uint8_t pixel_value = input_address[i];
        uint8_t modifier = bayer_matrix[y & 1][x & 1];
        uint16_t mixed_value = pixel_value + modifier;
        output_address[i] = mixed_value;
    }
}


#endif /* CPU_TESTS_H_ */

