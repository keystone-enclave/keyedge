#include <cstdio>
#include "ocalls.h"

const int test[TEST_SIZE] = {
	INT_MIN,
	INT_MAX,
	0,
	-1,
	42
};

int test_index = 0;

void int_call(int val) {
	if (test_index >= TEST_SIZE) {
		printf("[FAIL] Too many calls made.\n");
		return;
	}
	if (test[test_index] != val) {
		printf("[FAIL] Test %d failed. Expecting %d but received %d.\n",
			test_index, test[test_index], val);
	} else {
		printf("[PASS] Test %d passed. Expecting %d and received %d.\n",
			test_index, test[test_index], val);
		++test_index;
	}
	if (test_index == TEST_SIZE) {
		puts("[PASS] Test succeeded.");
	}
	return;
}

