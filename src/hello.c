#include <stdint.h>
#include <stdlib.h>
//#include "env_tests.h"


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



int main() {
	/* AP initialization */
 	warmup_ap();

 	volatile uint8_t comparison = matmul_evaluation (10);
 	//ap_mm_v4(3, 3, 2);

	return 0;
}

