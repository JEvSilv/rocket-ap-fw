#ifndef EVALUATION
#define EVALUATION
#include "util.h"
#include "ap_tests.h"
#include "cpu_tests.h"

uint8_t matmul_evaluation(int dimension);
void sgemm_evaluation();
void saxpy_evaluation();
void reduce_evaluation();
void index_evaluation();
void filter_index_evaluation();
void evm_evaluation();
void hamming_evaluation();
void bitcount_evaluation();
void checksum_evaluation();

uint8_t matmul_evaluation (int dimension) {
	fill_random_vectors(MULT, 11);

	start_compute_cycles();
	//ap_mm(dimension, dimension, dimension);
	//ap_mm_v2(dimension, dimension, dimension);
	//ap_mm_v3(dimension, dimension, dimension);
	ap_mm_v4(dimension, dimension, dimension);
	end_compute_cycles();

	memcpy(r_v_mgmt.C, AP_CAM_A_1, RANDOM_VECTORS_SIZE);
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

#endif
