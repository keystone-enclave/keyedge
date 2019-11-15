#include <cstdio>
#include <cstring>
#include "ocalls.h"

int test_nested_struct(nest1 n) {	
	if (strcmp(n.str, "nest1") || strcmp(n.n -> str, "nest2") || strcmp(n.n -> n -> str, "nest3")) {
		puts("[FAIL] Nested struct test failed.");
		return 1;
	}
	puts("[PASS] Nested struct test succeeded.");
	return 0;
}

