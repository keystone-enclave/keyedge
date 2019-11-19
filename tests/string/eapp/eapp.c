#include "common_header.h"
#include "ocalls_eapp.h"

char test[TEST_SIZE][512] = {
	"Hello World!",
	"",
	"gXUXCwlF2Y6V6IRpT0Msy0j4qtTrTuhSrweDG741bW4acWBs7Ge2yecYIVb6rXJAsNgLqNVF2ayPru1Q3ZIAc7wPinV7F5e7Y1NWqhy1h9lVDYjLZnWWnnD5kMf1t6LiwSo9nA2Yv4SgydUrNIIQiJ9z5yCiUtRJkEplDD913fG0Fhzzlbs8K5kvs8VOpERYdAJGmNW9Km9g1rKNLqjiPcwrZrHmhFhMtTDHqMY7GGmVH9ktAhxFLMdpT8FctMlF"
};

void EAPP_ENTRY eapp_entry() {
	for (int i = 0; i < TEST_SIZE; ++i) {
		string_call(test[i]);
	}
	EAPP_RETURN(0);
}

