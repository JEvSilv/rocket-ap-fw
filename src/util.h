/*
 * util.h
 *
 *  Created on: 11 de fev de 2025
 *      Author: root
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <stdint.h>

#define SIM_BREAK (volatile uint8_t *)(0x80000000)
extern uint64_t get_cycles();

// IDEA: Linked list
struct record_cycles {
	uint64_t start;
	uint64_t final;
	uint64_t result;
} record;

void start_compute_cycles() {
	record.start = get_cycles();
}

void end_compute_cycles() {
	record.final = get_cycles();
	record.result = record.final - record.start;
}

void sim_break() {
	*SIM_BREAK = 0xff;
}

void counter_test() {
	uint64_t cycle_count = get_cycles();
	volatile int x = 10;

	for(int i = 0; i < 1000; i++) {
		x += x;
	}

	uint64_t cycle_count_2 = get_cycles();
	volatile uint64_t latency = cycle_count_2 - cycle_count;
}

#endif /* UTIL_H_ */
