#include "eapp_header.h"

void EAPP_ENTRY eapp_entry() {
	int val = 42;
	int* pval = &val;
	int **p = double_pointer(&pval);
	if (**p == 0) test_success();
	EAPP_RETURN(0);
}

