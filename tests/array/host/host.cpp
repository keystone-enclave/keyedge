#include "ocalls_host.h"

int test_two_dimensional_array(int arr[2][3]) {
	int cnt = 0;
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 3; ++j)
			if (arr[i][j] != cnt++) {
				puts("[FAIL] Two dimensional array test failed.");
				return 1;
			}
	puts("[PASS] Two dimensional array test succeeded.");
	return 0;
}

int main(int argc, char** argv) {
	Keystone enclave;
	Params params;

	params.setFreeMemSize(2 * 1024 * 1024);
	params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 2 * 1024 * 1024);

	enclave.init(argv[1], argv[2], params);

	enclave.registerOcallDispatch(incoming_call_dispatch);
	
	printf("Registering...\n");
	fflush(stdout);
	
	/* We must specifically register functions we want to export to the
		enclave. */
	register_functions();

	edge_call_init_internals((uintptr_t)enclave.getSharedBuffer(),
		enclave.getSharedBufferSize());
	
	printf("Running the enclave...\n");
	fflush(stdout);

	enclave.run();

	return 0;
}

