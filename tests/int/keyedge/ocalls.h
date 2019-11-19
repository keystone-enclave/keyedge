#include <ocalls_header.h>
#include <limits.h>

#define TEST_SIZE 5

const int test[TEST_SIZE] = {
	INT_MIN,
	INT_MAX,
	0,
	-1,
	42
};

void int_call(int val);

