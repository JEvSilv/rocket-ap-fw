#include <stdint.h>
#include <stdlib.h>
//#include "env_tests.h"
#include "ap_tests.h"
#include "cpu_tests.h"
#include "ap_hal.h"

int main() {
	/* AP initialization */
 	warmup_ap();
	//accumulate_horizontal();
	//store_data_in_c();
	//test_vector_cpu_op();
	// test_vector_ap_op_horizontal();
	//test_offload(1024);
	//flush_test();
	//	volatile int result = ap_computing_horizontal_test(OR,
	//			LEFT, 14);
	//functional_test();
	//test_accum();
	//accumulate_horizontal();
	//basic_mem_test_2();
	//op_target_setup();
	//set_value_test();
	//ap_search_test();

//	fill_random_vectors(MULT, 2);

	// CPU running saxpy
//	start_compute_cycles();
//	saxpy_golden(64, 2, r_v_mgmt.A, r_v_mgmt.B);
//	end_compute_cycles();

//	start_compute_cycles();
//	index_golden(&r_v_mgmt.A[64], r_v_mgmt.A, r_v_mgmt.B, 64);
//	end_compute_cycles();

	// AP running saxpy
//	start_compute_cycles();
//	ap_index(NULL, r_v_mgmt.A, r_v_mgmt.B, 64);
//	end_compute_cycles();
ap_accum();
	asm volatile ("WFI");

	return 0;
}

