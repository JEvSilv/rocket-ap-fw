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
#pragma GCC optimize ("O2")
void matmul_cpu();
#pragma GCC pop_options

//#pragma GCC push_options
//#pragma GCC optimize ("O2")
void sgemm_golden_cpu_test();
//#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O2")
void saxpy_golden(int n, uint8_t a, uint8_t *x, uint8_t *y);
#pragma GCC pop_options

void reduce_golden(uint8_t *a, uint8_t *b, uint8_t *result_sum,
		uint8_t *result_count, int n);
void matmul_golden(uint8_t **a, uint8_t **b, uint8_t **c, int n, int m, int o);
void index_golden(uint8_t *a, uint8_t *b, uint8_t *c, int n);

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

// saxpy
void saxpy_golden(int n, uint8_t a, uint8_t *x, uint8_t *y) {
	for (int i = 0; i < n; ++i) {
		y[i] = a * x[i] + y[i];
	}
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
            uint8_t sum = 0.0f;
            for (int k = 0; k < k_len; ++k) {
                sum += r_v_mgmt.A[i * k_len + k] * r_v_mgmt.B[k * n_len + j];
            }
            r_v_mgmt.C[i * n_len + j] = sum;
        }
    }
}

// index arithmetic
void index_golden(uint8_t *a, uint8_t *b, uint8_t *c, int n) {
	for (int i = 0; i < n; ++i) {
		a[i] = b[i] + i * c[i];
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

#endif /* CPU_TESTS_H_ */
