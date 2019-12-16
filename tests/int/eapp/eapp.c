#include "eapp_header.h"

const int test[TEST_SIZE] = {
	INT_MIN,
	INT_MAX,
	0,
	-1,
	42
};

void EAPP_ENTRY eapp_entry() {
	for (int i = 0; i < TEST_SIZE; ++i) {
		int_call(test[i]);
	}
	EAPP_RETURN(0);
}

