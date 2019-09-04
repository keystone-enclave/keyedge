#include "tokenizer.h"
#include "parser.h"
#include "emitter.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Please specify one file name." << std::endl;
		return 0;
	}
	std::ifstream fin(argv[1]);
	std::stringstream buffer;
	buffer << fin.rdbuf();
	std::vector<raw_token> raw_tokens = tokenize(buffer.str());
	std::vector<token> tokens = preparse(raw_tokens);
	initialize_primitive_types();
	for (size_t i = 0; i < tokens.size(); i = tokens[i].statement_end + 1) {
		parse_statement(tokens, i, tokens[i].statement_end);
	}
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

