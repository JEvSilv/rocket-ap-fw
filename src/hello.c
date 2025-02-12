#include <stdint.h>
#include <stdlib.h>
#include "env_tests.h"
#include "ap_tests.h"
#include "cpu_tests.h"

int main() {
	/* AP initialization */
	warmup_ap();

	volatile int x = ap_computing_horizontal_test(XOR, LEFT, 13);

	return 0;
}

