#include "common_header.h"
#include "ocalls_eapp.h"

void EAPP_ENTRY eapp_entry() {
	for (int i = 0; i < TEST_SIZE; ++i) {
		int_call(test[i]);
	}
	EAPP_RETURN(0);
}

