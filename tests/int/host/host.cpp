#include "ocalls_host.h"

void simple_call(int val) {
	if (val == 42) puts("[PASS] Simple call test succeeded.");
	return;
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

