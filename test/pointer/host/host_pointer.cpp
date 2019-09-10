#include "pointer.keg.host.h"

int print_pointer(signed char name, int* value) {
	if (value)
		printf("The value of %c is %d.\n", name, *value);
	else
		printf("%c is a null pointer.\n", name);
	return 0;
}

int main(int argc, char** argv) {
	Keystone enclave;
	Params params;

	params.setFreeMemSize(2*1024*1024);
	params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 2*1024*1024);

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
	
	printf("Enclave finished.\n");
	fflush(stdout);
	
	return 0;
}

