/*
 * cpu_tests.h
 *
 *  Created on: 11 de fev de 2025
 *      Author: root
 */

#ifndef CPU_TESTS_H_
#define CPU_TESTS_H_
#include <stdint.h>

uint64_t vector_cpu_op(int8_t seed, uint32_t size) {
 	uint32_t vector_size = size;

 	uint8_t *A = (uint8_t*) malloc(vector_size);
 	uint8_t *B = (uint8_t*) malloc(vector_size);
 	uint8_t *C = (uint8_t*) malloc(vector_size);

 	volatile uint64_t cycle_count = 0;
 	unsigned long long start = 0, end = 0;

 	uint32_t mask = 0xff;

 	for (int i = 0; i < vector_size; i++) {
 		A[i] = (seed - i * 1234 + i + 4321) & mask;
 		B[i] = (seed + i * 4321 - i + 1234) & mask;
 	}

 	metal_timer_get_cyclecount(0, &start);

 	for (int i = 0; i < vector_size; i++) {
 		C[i] = A[i] + B[i];
 	}

 	metal_timer_get_cyclecount(0, &end);
 	cycle_count = end - start;

 	free(A);
 	free(B);
 	free(C);

 	return cycle_count;
}

void report_cycles_vector_op_cpu() {
	uint64_t report[5];
	uint32_t sizes[5];

	sizes[0] = 32;
	sizes[1] = 64;
	sizes[2] = 128;
	sizes[3] = 256;
	sizes[4] = 512;

	for (int i = 0; i < 5; i++) {
		report[i] = vector_cpu_op(14+i, sizes[i]);
	}

	return;
}


#endif /* CPU_TESTS_H_ */
