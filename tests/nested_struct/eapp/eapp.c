#include "common_header.h"
#include "ocalls_eapp.h"

int eapp_nested_struct() {
	nest1 n;
	n.n = (nest2*) malloc(sizeof(nest2));
	n.n -> n = (nest3*) malloc(sizeof(nest3));
	n.str = "nest1";
	n.n -> str = "nest2";
	n.n -> n -> str = "nest3";
	return test_nested_struct(n);
}

void EAPP_ENTRY eapp_entry() {
	eapp_nested_struct();
	EAPP_RETURN(0);
}

