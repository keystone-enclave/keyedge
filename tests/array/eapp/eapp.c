#include "common_header.h"
#include "ocalls_eapp.h"

int eapp_two_dimensional_array() {
	int arr[2][3];
	int cnt = 0;
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 3; ++j)
			arr[i][j] = cnt++;
	return test_two_dimensional_array(arr);
}

void EAPP_ENTRY eapp_entry() {
	eapp_two_dimensional_array();
	EAPP_RETURN(0);
}

