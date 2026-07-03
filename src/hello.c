#include <stdint.h>
#include <stdlib.h>
//#include "env_tests.h"

//#define camera_8x8
//#define camera_16x16
//#define camera_32x32
//#define camera_48x48
#define camera_64x64
//#define random_vector_int8

#ifdef camera_8x8
	#include "camera_8x8_4bit.h"
#endif

#ifdef camera_16x16
	#include "camera_16x16_4bit.h"
#endif

#ifdef camera_32x32
	#include "camera_32x32_4bit.h"
#endif

#ifdef camera_48x48
	#include "camera_48x48_4bit.h"
#endif

#ifdef camera_64x64
	#include "camera_64x64_4bit.h"
#endif

#ifdef random_vector_int8
	#include "random_vector.h"
#endif


#include "ap_tests.h"
#include "cpu_tests.h"
#include "ap_hal.h"
#include "evaluation.h"

// Prototypes CPU TESTS
//void sgemm_golden(int m_len, int k_len, int n_len, uint8_t *a, uint8_t *b,
//		uint8_t *c);
//#pragma GCC push_options
//#pragma GCC optimize ("O2")
//void saxpy_golden(int n, uint8_t a, uint8_t *x, uint8_t *y);
//#pragma GCC pop_options
//void reduce_golden(uint8_t *a, uint8_t *b, uint8_t *result_sum,
//		uint8_t *result_count, int n);
//void matmul_golden(uint8_t **a, uint8_t **b, uint8_t **c, int n, int m, int o);
//void index_golden(uint8_t *a, uint8_t *b, uint8_t *c, int n);
//void vector_cpu_op_kernel_or(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size);
//void vector_cpu_op_kernel_xor(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size);
//void vector_cpu_op_kernel_and(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size);
//void vector_cpu_op_kernel_not(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size);
//void vector_cpu_op_kernel_add(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size);
//void vector_cpu_op_kernel_sub(uint8_t *A, uint8_t *B, uint8_t *C, uint32_t size);
//void vector_cpu_op_kernel_mult(uint8_t *A, uint8_t *B, uint8_t *C,
//		uint32_t size);
//void vector_cpu_op(int8_t seed, uint32_t size);
//void test_accum();

// Prototypes AP TESTS
//int ap_computing_vertical_test(APOperations op, APInternalCollunm internal_col,
//		uint8_t seed);
//int ap_computing_horizontal_test(APOperations op,
//		APInternalCollunm internal_col, uint8_t seed);
//void pointer_ap_rw_test();
//void testing_internal_cols();
//void api_r_w_ap_test();
//void asm_memory_fun_tests();
//void functional_test();
//void vector_ap_op_kernel_horizontal(APOperations op, uint8_t *A, uint8_t *B,
//		uint8_t *C, uint32_t size);
//void fill_a_col(uint8_t factor, int size);
//void fill_a_b_cols(uint8_t *A, uint8_t *B, int size);
//void offload(int size);
//void ap_monitor_breakpoint();
//void accum();
//void vector_ap_op_horizontal(APOperations op, int size, int seed);
//void test_offload(int size);
//void accum_data_manipulation(int size);
//void accumulate_horizontal();
//void accumulate_vertical();
//void test_vector_ap_op_horizontal();
//void ap_accum();
//void ap_reduce(uint8_t *a, uint8_t *b, uint8_t *result_sum,
//		uint8_t *result_count, int n);
//void ap_search_test();
//volatile result = ap_computing_horizontal_test(MULT, LEFT, 10);

void matmul_average_evaluation(int times, int dimension) {
	volatile uint32_t buff_load = 0;
	volatile uint32_t ap_load = 0;
	volatile uint32_t ap_computing = 0;
	volatile uint32_t cpu_computing = 0;

	for (int i = 0; i < times; i++) {
		warmup_ap();
		buffer_load_evaluation(dimension);
		volatile uint8_t comparison = matmul_evaluation (dimension);
		buff_load += record.results[0];
		ap_load += record.results[1];
		ap_computing += record.results[2];
		cpu_computing += record.results[3];
		record.results_size = 0;
	}

	buff_load = buff_load / times;
	ap_load = ap_load / times;
	ap_computing = ap_computing / times;
	cpu_computing = cpu_computing / times;

	return;
}

void filter_average_evaluation(int times, int dimension) {

	volatile uint32_t ap_load = 0;
	volatile uint32_t ap_offload = 0;
	volatile uint32_t ap_computing = 0;
	volatile uint32_t cpu_computing = 0;

	for (int i = 0; i < times; i++) {
		warmup_ap();
		volatile int result = filter_evaluation(dimension);
		// 48x48
		if (dimension == 48) {
			ap_offload += record.results[2] + record.results[5] + record.results[8];
			ap_load += record.results[3] + record.results[6];
			ap_computing += record.results[1] + record.results[4] + record.results[7];
			cpu_computing += record.results[9];
		}

		if (dimension == 64) {
			ap_offload += record.results[2] + record.results[5] + record.results[8] + record.results[11];
			ap_load += record.results[3] + record.results[6] + record.results[9];
			ap_computing += record.results[1] + record.results[4] + record.results[7] + record.results[10];
			cpu_computing += record.results[12];
		}

		record.results_size = 0;
	}

	ap_offload = ap_offload / times;
	ap_load = ap_load / times;
	ap_computing = ap_computing / times;
	cpu_computing = cpu_computing / times;

	return;
}


int main() {
	/* AP initialization */
 	warmup_ap();

 	//matmul_average_evaluation(10, 16);
 	//volatile int result = filter_evaluation(48);
 	//filter_average_evaluation(10, 64);
 	//fill_random_vectors(MULT, 11);
 	//ap_mm_v4(3, 3, 2);
 	//index_evaluation(2048);

 	// 512
 	//ap_index(480, 0, LEFT);
 	//ap_index(32, 0, LEFT);

 	// 1024
 	// 	ap_index(480, 0, LEFT);
 	// 	ap_index(480, 0, LEFT);
 	// 	ap_index(64, 0, LEFT);


 	//end_compute_cycles();
 	// 2048
	//ap_index(480, 0, LEFT);
	//ap_index(480, 0, LEFT);
	//ap_index(480, 0, LEFT);
	//ap_index(480, 0, LEFT);
	//ap_index(128, 0, LEFT);

 	ap_hamming(960);
 	ap_hamming(64);


 	//saxpy_evaluation(1024);
 	//ap_relu(0x80002000, 480*3);
 	//ap_relu(0x80002000, 304);
	return 0;
}

// 	volatile uint32_t cpu_lat;
// 	for (int i = 0; i < 10; i++) {
// 		relu_evaluation(4096);
// 		cpu_lat += record.results[0];
// 		record.results_size = 0;
// 	}
//
// 	cpu_lat = cpu_lat / 10;

//start_compute_cycles();
// 	index_golden(64);
// 	end_compute_cycles();
// 	start_compute_cycles();
// 	index_golden(128);
// 	end_compute_cycles();
// 	start_compute_cycles();
// 	index_golden(256);
// 	end_compute_cycles();
// 	start_compute_cycles();
// 	index_golden(512);
// 	end_compute_cycles();
// 	start_compute_cycles();
// 	index_golden(1024);
// 	end_compute_cycles();
// 	start_compute_cycles();
// 	index_golden(2048);


//start_compute_cycles();
//branch_golden(64);
//end_compute_cycles();
//start_compute_cycles();
//branch_golden(128);
//end_compute_cycles();
//start_compute_cycles();
//branch_golden(256);
//end_compute_cycles();
//start_compute_cycles();
//branch_golden(512);
//end_compute_cycles();
//start_compute_cycles();
//branch_golden(1024);
//end_compute_cycles();
//start_compute_cycles();
//branch_golden(2048);
//end_compute_cycles();

//	start_compute_cycles();
//	hamming_code_golden(64);
//	end_compute_cycles();
//	start_compute_cycles();
//	hamming_code_golden(128);
//	end_compute_cycles();
//	start_compute_cycles();
//	hamming_code_golden(256);
//	end_compute_cycles();
//	start_compute_cycles();
//	hamming_code_golden(512);
//	end_compute_cycles();
//	start_compute_cycles();
//	hamming_code_golden(1024);
//	end_compute_cycles();
//	start_compute_cycles();
//	hamming_code_golden(2048);
//	end_compute_cycles();


