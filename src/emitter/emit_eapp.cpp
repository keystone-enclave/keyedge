#include "index.h"

std::string emit_header_eapp(size_t indent = 0) {
	return emit(
		"#include \"eapp_utils.h\"\n"
		"#include \"edge_call.h\"\n"
		"#include <syscall.h>\n"
		"#include \"malloc.h\"\n"
		"#include \"string.h\"\n"
		"#include \"edge_call.h\"\n"
		"#include \"h2ecall.h\"\n"
		"#include \"ocalls.h\"\n"
		"#include \"ocalls_common.h\"\n"
		"#include \"ocalls_builder.h\"\n"
		"#include \"ocalls_reader.h\"", {}, indent);
}

std::string emit_end_eapp(size_t indent = 0) {
	return emit(
		"void register_inverse_functions() {\n"
		"$(REGISTER_FUNCTIONS)"
		"\treturn;\n"
		"}", {
			{"$(REGISTER_FUNCTIONS)", [&]() -> std::string {
				std::string ret;
				for (auto& function_pair : function_lookup) {
					std::shared_ptr<function_information> f = function_pair.second;
					if (f -> attr_flag & ATTRIBUTE_INVERSE) {
						ret += emit("\th2ecall_list[__function_$(NAME)] = __wrapper_$(NAME);\n", {
							{"$(NAME)", f -> name}}, indent);
					}
				}
				return ret;
			}()},
		}, indent);
}

std::string emit_function_eapp(std::shared_ptr<function_information> f, size_t indent = 0) {
	if (f -> attr_flag & ATTRIBUTE_INVERSE) {
		return emit_function_callee(f, indent);
	} else {
		return emit_function_caller(f, indent);
	}
}

std::string emit_eapp(size_t indent) {
	return emit(
		"$(HEADER)\n"
		"$(FUNCTIONS)"
		"$(END)", {
			{"$(HEADER)", emit_header_eapp(indent)},
			{"$(FUNCTIONS)", [&]() -> std::string {
				std::string ret; 
				for (auto& function_pair : function_lookup) {
					ret += emit("$(EMIT)\n", {
						{"$(EMIT)", emit_function_eapp(function_pair.second, indent)}
					}, indent);
				}
				return ret;
			}()},
			{"$(END)", emit_end_eapp(indent)}
		}, indent);
}

