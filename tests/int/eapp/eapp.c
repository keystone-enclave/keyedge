#include "common_header.h"
#include "ocalls_eapp.h"

void EAPP_ENTRY eapp_entry() {
	simple_call(42);
	EAPP_RETURN(0);
}

