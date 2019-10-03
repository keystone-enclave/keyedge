#include "index.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Please specify one file name." << std::endl;
		return 0;
	}

	int parse_argc = (argc - 2) + 2;
	const char **parse_argv = new const char*[parse_argc];
	for (int i = 0; i < argc - 2; ++i)
		parse_argv[i] = argv[i + 2];
	parse_argv[argc - 2] = "-D__KEYEDGE";
	std::string include_arg = std::string ("-I") + std::string(std::getenv("KEYEDGE_DIR")) + "/target/include";
	parse_argv[argc - 1] = include_arg.c_str();

	// set up LibClang
	CXIndex index = clang_createIndex(0, 1);
	CXTranslationUnit unit = clang_parseTranslationUnit(
		index, argv[1], parse_argv, parse_argc, nullptr, 0, CXTranslationUnit_None);
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

