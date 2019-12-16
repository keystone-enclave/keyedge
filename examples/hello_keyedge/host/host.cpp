#include <keystone.h>
#include <edge_call.h>
#include "ocalls.h"

void register_functions();

int print_string(const char* msg) {
	int ret = printf("%s\n", msg);
	fflush(stdout);
	return ret;
}

int print_return_value(int msg) {
	int ret = printf("The function previously called returned %d.\n", msg);
	fflush(stdout);
	return ret;
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

