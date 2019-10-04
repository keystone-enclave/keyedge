#include "common_header.h"
#include "ocalls_eapp.h"
#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>

int eapp_two_dimensional_array() {
	int arr[4][5];
	int cnt = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 5; ++j)
			arr[i][j] = cnt++;
	return test_two_dimensional_array(arr);
}

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
	if (eapp_two_dimensional_array()) EAPP_RETURN(0);
	if (eapp_nested_struct()) EAPP_RETURN(0);
	EAPP_RETURN(0);
}

