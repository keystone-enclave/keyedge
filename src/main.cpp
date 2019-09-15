#include "tokenizer.h"
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
	std::ofstream feapp(std::string(argv[1]) + ".eapp.h");
	std::ofstream fhost(std::string(argv[1]) + ".host.h");
	feapp << emit_header_eapp();
	fhost << emit_header_host();
	for (auto& function_pair : function_lookup) {
		feapp << emit_function_eapp(function_pair.second);
		fhost << emit_function_host(function_pair.second);
	}
	feapp << emit_end_eapp();
	fhost << emit_end_host();
	return 0;
}

