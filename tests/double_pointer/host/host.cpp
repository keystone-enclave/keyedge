#include <cstdio>

int val;
int* pval = &val;

int** double_pointer(int** test) {
	if (**test == 42) {
		val = 0;
		return &pval;
	}
	val = 1;
	return &pval;
}

void test_success() {
	puts("[PASS] Double pointer test succeeded.");
}

