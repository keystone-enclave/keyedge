Keyedge Edge Call Generation Tool
=================================

There is an edge call generator for Keystone, similar to Edger8r in
SGX. It takes a user-defined header file, looks through each function
inside, and generates wrapper code so that the user may directly make
edge calls in eapp and implement them in host without the hassle of
manually coding all the data marshalling inbetween.

Installation
------------

Keystone
^^^^^^^^

You need to have
`Keystone <https://github.com/keystone-enclave/keystone>`_ built if
you have not done so already - read its ``README``'s Quick Start
part.

You also need to set the environment variable ``$KEYSTONE_SDK_DIR``
to the ``/sdk`` directory of the repository, and add RISC-V compilers
to ``$PATH`` - check ``source.sh`` under the root of its repository
for how it is typically done.

Flatcc
^^^^^^

You need to have a copy of
`Flatcc <https://github.com/dvidelabs/flatcc>`_, which is used in
serialization. The following snippet may be used to obtain it.

::

	git clone https://github.com/dvidelabs/flatcc.git
	cd flatcc
	scripts/initbuild.sh make
	scripts/build.sh

After the installation is complete, set the environment variable
``$FLATCC_DIR`` to the root of the repository, for example
``$HOME/flatcc``.

LibClang
^^^^^^^^

You need to have a working
'Clang <https://clang.llvm.org/get_started.html>'_. This tool is
built on Clang 10.0.0, but other versions might work.

Set the environment variable ``$LIBCLANG_INCLUDE_DIR`` to the Clang
``include`` directory, where ``clang-c/Index.h`` may be included,
typically ``<some-path>/llvm-project/clang/include``.

Set the environment variable ``$LIBCLANG_LIB_DIR`` to the Clang lib
directory, typically ``<some-path>/llvm-project/build/lib``.

Build
^^^^^

Execute ``make`` to build. Then set the environment variable
``$KEYEDGE_DIR`` to this repository's directory.

Tutorial
--------

While it is possible to build any application with Keyedge from
scratch, it is more recommended that a template is used to reduce
effort in dealing with mundane work. In this tutorial we will build
a hello-world like example from a blank template to demostrate how it
is done.

To begin with, let us copy the blank folder to start a project.

::

	cd ${KEYEDGE_DIR}
	cp -r examples/blank/ examples/tutorial
	cd examples/tutorial

Then, edit ``vault.sh`` so that the name of the project is correctly
reflected in the ``NAME`` variable.

::

	################################################################
	#                   Replace the variables                      #
	################################################################
	NAME=tutorial
	VAULT_DIR=`dirname $0`
	BUILD_COMMAND="make -C eapp && make -C host"
	OUTPUT_DIR=$KEYSTONE_SDK_DIR/../buildroot_overlay/root/$NAME
	EYRIE_DIR=$KEYSTONE_SDK_DIR/rts/eyrie
	EYRIE_PLUGINS="freemem"
	PACKAGE_FILES="eapp/eapp \
		           host/host \
		           $EYRIE_DIR/eyrie-rt"
	PACKAGE_SCRIPT="./host eapp eyrie-rt"
	
After that, we will append the edge call we want to use in the
``keyedge/ocalls.h`` file.

.. code-block:: cpp

	#include <ocalls_header.h>

	// Add edge call function declarations here.

	void print_string(keyedge_str char* str);

Here, we added a ``print_string`` function for the enclave to be able
to output message through the host application via the use of an edge
call. We can see that the syntax of the header file is identical to a
regular C header file, except that a ``keyedge_str`` modifier is used
to show that the ``str`` argument is actually a C-styled string,
rather than a pointer to a single character. Such modifier would not
be necessary, if, say, we were to add a ``print_int`` function that
prints an integer to the screen:

.. code-block:: cpp

	void print_int(int val);

After we have declared a function to be a edge call, we need to
provide its implementation in the host application. Therefore,
we append the following function definition to ``host/host.cpp``.

.. code-block:: cpp

	void print_string(char* str) {
		printf("%s", str);
		free(str);
	}
	
It is worth noting that we do not need a ``keyedge_str`` modifier
here, since it is only used in the data marshalling part, not in
actual call implementation. It is also worth noting that since the
string is allocated in place, we need to free it after we handled
it, or risk a undesired memory leak.

With all that being said, we can now configure our enclave to make a
real edge call. Simply program it to do so in ``eapp/eapp.c``.

.. code-block:: cpp

	void EAPP_ENTRY eapp_entry() {
		print_string("Hello world!\n");
		EAPP_RETURN(0);
	}

Now that we have finished every part, simply run
``./keyedge_vault.sh`` to build all the parts. If everything works,
we can then execute ``make image`` under Keystone directory to build
the QEMU image, and test the tutorial out via QEMU.

