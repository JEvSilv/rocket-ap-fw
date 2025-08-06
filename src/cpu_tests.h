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

// sgemm
void sgemm_golden(int m_len, int k_len, int n_len, uint8_t *a, uint8_t *b,
		uint8_t *c) {
	for (int i = 0; i < m_len; ++i)
		for (int j = 0; j < n_len; ++j)
			for (int k = 0; k < k_len; ++k)
				c[i * n_len + j] += a[i * k_len + k] * b[j + k * n_len];
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
		if (a[i] != 42.0) {
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

// index arithmetic
void index_golden(uint8_t *a, uint8_t *b, uint8_t *c, int n) {
	for (int i = 0; i < n; ++i) {
		a[i] = b[i] + i * c[i];
	}
}

void vector_cpu_op_kernel_or(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size) {
	for (int i = 0; i < size; i++) {
		C[i] = A[i % 1024] | B[i % 1024];
	}
}

void vector_cpu_op_kernel_xor(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size) {
	for (int i = 0; i < size; i++) {
		C[i] = A[i % 1024] ^ B[i % 1024];
	}
}

void vector_cpu_op_kernel_and(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size) {
	for (int i = 0; i < size; i++) {
		C[i] = A[i % 1024] & B[i % 1024];
	}
}

void vector_cpu_op_kernel_not(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size) {
	for (int i = 0; i < size; i++) {
		C[i] = ~A[i % 1024];
	}
}

void vector_cpu_op_kernel_add(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size) {
	for (int i = 0; i < size; i++) {
		C[i] = A[i % 1024] + B[i % 1024];
	}
}

void vector_cpu_op_kernel_sub(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size) {
	for (int i = 0; i < size; i++) {
		C[i] = A[i % 1024] - B[i % 1024];
	}
}

void vector_cpu_op_kernel_mult(uint8_t *A, uint8_t *B, uint8_t *C,
		uint32_t size) {
	for (int i = 0; i < size; i++) {
		C[i] = A[i % 1024] * B[i % 1024];
	}
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

#endif /* CPU_TESTS_H_ */
