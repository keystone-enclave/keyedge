#include <cstdio>
#include <cstring>
#include "ocalls.h"

char test[TEST_SIZE][512] = {
	"Hello World!",
	"",
	"gXUXCwlF2Y6V6IRpT0Msy0j4qtTrTuhSrweDG741bW4acWBs7Ge2yecYIVb6rXJAsNgLqNVF2ayPru1Q3ZIAc7wPinV7F5e7Y1NWqhy1h9lVDYjLZnWWnnD5kMf1t6LiwSo9nA2Yv4SgydUrNIIQiJ9z5yCiUtRJkEplDD913fG0Fhzzlbs8K5kvs8VOpERYdAJGmNW9Km9g1rKNLqjiPcwrZrHmhFhMtTDHqMY7GGmVH9ktAhxFLMdpT8FctMlF"
};

int test_index = 0;

void string_call(char* val) {
	if (test_index >= TEST_SIZE) {
		printf("[FAIL] Too many calls made.\n");
		return;
	}
	if (strcmp(test[test_index], val)) {
		printf("[FAIL] Test %d failed. Expecting \"%s\" but received \"%s\".\n",
			test_index, test[test_index], val);
	} else {
		printf("[PASS] Test %d passed. Expecting \"%s\" and received \"%s\".\n",
			test_index, test[test_index], val);
		++test_index;
	}
	if (test_index == TEST_SIZE) {
		puts("[PASS] Test succeeded.");
	}
	return;
}

