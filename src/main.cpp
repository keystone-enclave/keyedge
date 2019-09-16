#include "parser.h"
#include "emitter.h"

#include <iostream>
#include <fstream>

#include <clang-c/Index.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Please specify one file name." << std::endl;
		return 0;
	}

	// set up LibClang
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = clang_parseTranslationUnit(
		index, argv[1], nullptr, 0, nullptr, 0, CXTranslationUnit_None);
	if (unit == nullptr) {
		std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
		exit(-1);
	}

	initialize_primitive_types();
	CXCursor cursor = clang_getTranslationUnitCursor(unit);
	clang_visitChildren(cursor, parse_statement, nullptr);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	// emit output
	std::ofstream fcommon("ocalls_common.h");
	std::ofstream feapp("ocalls_eapp.h");
	std::ofstream fhost("ocalls_host.h");
	std::ofstream ffbs("ocalls.fbs");
	fcommon << emit_common();
	feapp << emit_eapp();
	fhost << emit_host();
	ffbs << emit_fbs();

	return 0;
}

