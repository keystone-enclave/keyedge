# Introduction
A simple edge call generator for Keystone.

# Build Instruction
## Prequisite
### Keystone
You need to have [Keystone](https://github.com/keystone-enclave/keystone) built if you have not done so already - read its `README`'s Quick Start part.

You also need to set `$KEYSTONE_SDK_DIR` to the `/sdk` directory of the repository, and add RISC-V compilers to `$PATH` - check `source.sh` under the root of its repository for how it is done.

### Flatcc
You need to have a copy of [Flatcc](https://github.com/dvidelabs/flatcc), used in serialization:
```
git clone https://github.com/dvidelabs/flatcc.git
cd flatcc
scripts/initbuild.sh make
scripts/build.sh
```

Set `$FLATCC_DIR` to the root of the repository, for example `$HOME/flatcc`.

### LibClang
You need to have a working [Clang](https://clang.llvm.org/get_started.html). This tool is built on Clang 10.0.0, but other versions might work.

Set `$LIBCLANG_INCLUDE_DIR` to the Clang include directory, where `clang-c/Index.h` may be included, typically `<some-path>/llvm-project/clang/include`.

Set `$LIBCLANG_LIB_DIR` to the Clang lib directory, typically `<some-path>/llvm-project/build/lib`.

## Build

Run `make` to build. Then set `$KEYEDGE_DIR` to this repository's directory.

## Example

To build the example, run `keyedge_vault.sh` in the folder. Then refer to Keystone on how to build the image and test it in QEMU.

To start a new project, simply copy the example, modify the project name in `vault.sh`, and have `keyedge/ocalls.h` containing every edge call you want to use.

