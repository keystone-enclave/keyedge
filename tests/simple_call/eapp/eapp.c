#include "common_header.h"
#include "ocalls_eapp.h"

void EAPP_ENTRY eapp_entry() {
	simple_call();
	EAPP_RETURN(0);
}

