/*
 * util.h
 *
 *  Created on: 11 de fev de 2025
 *      Author: root
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include "ap_hal.h"

#define SIM_BREAK (volatile uint8_t *)(0x80000000)
#define MAX_RANDOM_VEC 3
#define MAX_RESULTS 25
#define MAX_CHECK_RESULTS 10
#define RANDOM_VECTORS_SIZE 1
#define CPU_RESULT_VECTOR_SIZE 1024
#define MONITOR_VECTORS_SIZE 20
#define ACCUM_VECTORS_SIZE 3

extern uint64_t get_cycles();


// IDEA: Linked list
struct record_cycles_t {
	uint64_t start;
	uint64_t final;
	uint64_t result;
	uint64_t accum[ACCUM_VECTORS_SIZE];
	uint64_t results[MAX_RESULTS];
	int results_size;
} record = { .start = 0, .final = 0, .result = 0, .results = { 0 },
		.results_size = 0 };

struct random_vectors_mgmt_t {
	uint8_t A[RANDOM_VECTORS_SIZE];
	uint8_t B[RANDOM_VECTORS_SIZE];
	uint8_t C[RANDOM_VECTORS_SIZE];
} r_v_mgmt;

//struct cpu_result_vector_t {
//	uint8_t result[CPU_RESULT_VECTOR_SIZE];
//} cpu_result_vector;

struct monitor_vectors_t {
	uint8_t A_0_MON[MONITOR_VECTORS_SIZE];
	uint8_t B_0_MON[MONITOR_VECTORS_SIZE];
	uint8_t C_0_MON[MONITOR_VECTORS_SIZE];
	uint8_t A_1_MON[MONITOR_VECTORS_SIZE];
	uint8_t B_1_MON[MONITOR_VECTORS_SIZE];
	uint8_t C_1_MON[MONITOR_VECTORS_SIZE];
} monitor_vectors;

uint8_t start_accum = 0;
int lat_accum = 0;

struct check_results_t {
	uint8_t results[MAX_CHECK_RESULTS];
	int results_size;
} check_results = { .results = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, .results_size =
		0 };

void start_compute_cycles() {
	record.start = get_cycles();
}

void end_compute_cycles() {
	record.final = get_cycles();
	record.result = record.final - record.start;

	if (record.results_size == MAX_RESULTS)
		return;

	record.results[record.results_size] = record.result;
	record.results_size++;
}

void commit_accum_cycles(uint32_t index) {
	record.results[record.results_size] = record.accum[index];
	record.results_size++;
}

void accum_cycles(uint32_t index) {
	record.final = get_cycles();
	record.result = record.final - record.start;
	record.accum[index] += record.result;
}

void init_accum_cycles(uint32_t index) {
	record.accum[index] = 0;
}

void sim_break() {
	*SIM_BREAK = 0xff;
}

void counter_test() {
	uint64_t cycle_count = get_cycles();
	volatile int x = 10;

	for (int i = 0; i < 1000; i++) {
		x += x;
	}

	uint64_t cycle_count_2 = get_cycles();
	volatile uint64_t latency = cycle_count_2 - cycle_count;
}

void fill_random_vectors(APOperations op, int seed) {
	volatile uint32_t mask = 0xff;

	if (op > 3) {
		//mask = 0xf;
		mask = 0x3;
	}

	for (int i = 0; i < RANDOM_VECTORS_SIZE; i++) {
		r_v_mgmt.A[i] = (seed - i * 1234 + i + 4321) & mask;
		r_v_mgmt.B[i] = (seed + i * 4321 + i - 1234) & mask;
		r_v_mgmt.C[i] = 0;
	}
}

void save_result(uint8_t result) {
	if (check_results.results_size == MAX_CHECK_RESULTS)
		return;

	check_results.results[check_results.results_size] = result;
	check_results.results_size++;
}

void ap_monitor(int elements) {

	if(elements > MONITOR_VECTORS_SIZE) {return;}

	for(int i = 0; i < elements; i++){
		monitor_vectors.A_0_MON[i] = AP_CAM_A[i];
	}

	for(int i = 0; i < elements; i++){
		monitor_vectors.B_0_MON[i] = AP_CAM_B[i];
	}

	for(int i = 0; i < elements; i++){
		monitor_vectors.C_0_MON[i] = AP_CAM_C[i];
	}

	for(int i = 0; i < elements; i++){
		monitor_vectors.A_1_MON[i] = AP_CAM_A_1[i];
	}

	for(int i = 0; i < elements; i++){
		monitor_vectors.B_1_MON[i] = AP_CAM_B_1[i];
	}

	for(int i = 0; i < elements; i++){
		monitor_vectors.C_1_MON[i] = AP_CAM_C_1[i];
	}
}

volatile uint8_t checksum(uint8_t * arr, int size) {
	volatile uint8_t sum = 0;

	for (int i = 0; i < size; i++)	sum += arr[i];

	return sum;
}

#endif /* UTIL_H_ */
