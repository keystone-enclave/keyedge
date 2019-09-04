#include "struct.keg.host.h"

int print_messages(int count, message m[5]) {
	for (int i = 0; i < count; ++i)
		printf("%d:%d:%d %s: %s\n", m[i].t.h, m[i].t.m, m[i].t.s, m[i].sender, m[i].content);
	return count;
}

int main(int argc, char** argv) {
	Keystone enclave;
	Params params;

	params.setFreeMemSize(1024*1024);
	params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 1024*1024);

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

