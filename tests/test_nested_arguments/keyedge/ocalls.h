#include <ocalls_header.h>

typedef struct nest3 {
	keyedge_str char* str;
} nest3;

typedef struct nest2 {
	nest3* n;
	keyedge_str char* str;
} nest2;

typedef struct nest1 {
	nest2* n;
	keyedge_str char* str;
} nest1;

int test_two_dimensional_array(int arr[2][3]);
int test_nested_struct(nest1 n);

