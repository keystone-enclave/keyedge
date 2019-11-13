#include "common_header.h"
#include "ocalls_eapp.h"

void EAPP_ENTRY eapp_entry() {
	struct v val;
	val.negative = -1;
	val.large = 1u << 31;
	simple_call(val);
	EAPP_RETURN(0);
}

