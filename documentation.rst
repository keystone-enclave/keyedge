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

Coming soon.
