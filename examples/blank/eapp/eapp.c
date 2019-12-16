#include "eapp_utils.h"
#include "string.h"
#include "h2ecall.h"
#include "edge_call.h"
#include <syscall.h>
#include "ocalls.h"

void register_inverse_functions();

void EAPP_ENTRY eapp_entry() {
	EAPP_RETURN(0);
}

