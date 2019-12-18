#include "index.h"

std::string emit_header_host(size_t indent = 0) {
	return emit(
		"#include <keystone.h>\n"
		"#include <edge_call.h>\n"
		"#include \"ocalls.h\"\n"
		"#include \"ocalls_common.h\"\n"
		"#include \"ocalls_builder.h\"\n"
		"#include \"ocalls_reader.h\"\n"
		"extern Keystone enclave;\n", {}, indent);
}

std::string emit_end_host(size_t indent = 0) {
	return emit(
		"void register_functions() {\n"
		"$(REGISTER_FUNCTIONS)"
		"\treturn;\n"
		"}", {
			{"$(REGISTER_FUNCTIONS)", [&]() -> std::string {
				std::string ret;
				for (auto& function_pair : function_lookup) {
					std::shared_ptr<function_information> f = function_pair.second;
					if (!(f -> attr_flag & ATTRIBUTE_INVERSE)) {
						ret += emit("\tregister_call(__function_$(NAME), __wrapper_$(NAME));\n", {
							{"$(NAME)", f -> name}}, indent);
					}
				}
				return ret;
			}()},
		}, indent);
}

std::string emit_function_host(std::shared_ptr<function_information> f, size_t indent = 0) {
	if (f -> attr_flag & ATTRIBUTE_INVERSE) {
		return emit_function_caller(f, indent);
	} else {
		return emit_function_callee(f, indent);
	}
}

std::string emit_host(size_t indent) {
	return emit(
		"$(HEADER)\n"
		"$(FUNCTIONS)"
		"$(END)", {
			{"$(HEADER)", emit_header_host(indent)},
			{"$(FUNCTIONS)", [&]() -> std::string {
				std::string ret; 
				for (auto& function_pair : function_lookup) {
					ret += emit("$(EMIT)\n", {
						{"$(EMIT)", emit_function_host(function_pair.second, indent)}
					}, indent);
				}
				return ret;
			}()},
			{"$(END)", emit_end_host(indent)}
		}, indent);
}

