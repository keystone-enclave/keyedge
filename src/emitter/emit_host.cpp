#include "index.h"

std::string emit_header_host(size_t indent = 0) {
	return emit(
		"#include <keystone.h>\n"
		"#include <edge_call.h>\n"
		"#include \"ocalls.h\"\n"
		"#include \"ocalls_common.h\"\n"
		"#include \"ocalls_builder.h\"\n"
		"#include \"ocalls_reader.h\"", {}, indent);
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
					ret += emit("\tregister_call(__function_$(NAME), __wrapper_$(NAME));\n", {
						{"$(NAME)", f -> name}}, indent);
				}
				return ret;
			}()},
		}, indent);
}

std::string emit_deserialize_argument(const std::string& function_name, std::shared_ptr<element_information>& argument,
	size_t indent = 0) {
	return emit(
		"\t$(FLATCC_TYPE) __flatcc_reference_$(NAME) = __ocall_wrapper_$(FUNCTION_NAME)_$(NAME)(function_reference);\n"
		"\t$(DESERIALIZE)\n", {
			{"$(FLATCC_TYPE)", (*argument -> type) -> flatcc_type()},
			{"$(NAME)", argument -> name},
			{"$(FUNCTION_NAME)", function_name},
			{"$(DESERIALIZE)", emit_deserialize(argument -> name,
				std::string("__flatcc_reference_") + argument -> name,
				*argument -> type, indent + 1)}
		}, indent);
}

std::string emit_function_host(std::shared_ptr<function_information> f, size_t indent = 0) {
	std::queue<std::string> sizes;
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		if (f -> arguments[i] -> attr_flag & ATTRIBUTE_SIZE) {
			sizes.push(f -> arguments[i] -> name);
		}
	}
	return emit(
		"void __wrapper_$(NAME)(void* buffer) {\n"
		"\tstruct edge_call* edge_call = (struct edge_call*) buffer;\n"
		"\tuintptr_t call_args;\n"
		"\tsize_t arg_len;\n"
		"\tif (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {\n"
		"\t\tedge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;\n"
		"\t\treturn;\n"
		"\t}\n"
		"$(ALLOCATE_ARGUMENTS)"
		"\t__ocall_wrapper_$(NAME)_table_t function_reference = __ocall_wrapper_$(NAME)_as_root((void *) call_args);\n"
		"$(DESERIALIZE_NON_VLA_ARGUMENTS)"
		"$(DESERIALIZE_VLA_ARGUMENTS)"
		"$(CALL_FUNCTION)"
		"\tflatcc_builder_t builder;\n"
		"\tflatcc_builder_init(&builder);\n"
		"\t__ocall_wrapper_$(NAME)_start_as_root(&builder);\n"
		"$(SETUP_RETURN)"
		"\t__ocall_wrapper_$(NAME)_end_as_root(&builder);\n"
		"\tvoid* __buf;\n"
		"\tsize_t __size;\n"
		"\t__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);\n"
		"\tuintptr_t data_section = edge_call_data_ptr();\n"
		"\tmemcpy((void *) data_section, __buf, __size);\n" 
		"\tfree(__buf);\n"
		"\tflatcc_builder_clear(&builder);\n"
		"\tif (edge_call_setup_ret(edge_call, (void*) data_section, 1024)) {\n"
		"\t\tedge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;\n"
		"\t} else {\n"
		"\t\tedge_call -> return_data.call_status = CALL_STATUS_OK;\n"
		"\t}\n"
		"$(RELEASE_CONST)"
		"\treturn;\n"
		"}", {
			{"$(NAME)", f -> name},
			{"$(ALLOCATE_ARGUMENTS)", [&]() -> std::string {
				std::string ret;
				for (size_t i = 0; i < f -> arguments.size(); ++i) {
					ret += emit("\t$(EMIT);\n", {{"$(EMIT)", (*f -> arguments[i] -> type) -> str(f -> arguments[i] -> name)}});
				}
				return ret;
			}()},
			{"$(DESERIALIZE_NON_VLA_ARGUMENTS)", [&]() -> std::string {
				std::string ret;
				for (auto& argument : f -> arguments) {
					if (!(argument -> attr_flag & ATTRIBUTE_VLA)) {
						ret += emit_deserialize_argument(f -> name, argument, indent);
					}
				}
				return ret;
			}()},
			{"$(DESERIALIZE_VLA_ARGUMENTS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& argument : f -> arguments) {
					if (argument -> attr_flag & ATTRIBUTE_VLA) {
						if (argument -> attr_flag & ATTRIBUTE_STR) {
							std::string var_name = emit("__$(INDENT)_$(ARGUMENT_NAME)_keyedge_size", {
								{"$(INDENT)", std::to_string(indent)},
								{"$(ARGUMENT_NAME)", argument -> name}
							});
							ret += emit("size_t $(VAR_NAME) = $(ARGUMENT_FLATCC_PREFIX)_len(__ocall_wrapper_$(NAME)_$(ARGUMENT_NAME)($(SERIALIZED_NAME)));\n", {
								{"$(VAR_NAME)", var_name},
								{"$(ARGUMENT_FLATCC_PREFIX)", (*argument -> type) -> flatcc_prefix()},
								{"$(NAME)", f -> name},
								{"$(ARGUMENT_NAME)", argument -> name},
								{"$(SERIALIZED_NAME)", "function_reference"}
							}, indent);
							std::dynamic_pointer_cast<array_information>(*argument -> type) -> lengths =
								std::vector <std::string>(1, var_name);
						} else {
							std::dynamic_pointer_cast<array_information>(*argument -> type) -> lengths =
								std::vector <std::string>(1, sizes.front());
							sizes.pop();
						}
						ret += emit_deserialize_argument(f -> name, argument, indent);
					}
				}
				return ret;
			}()},
			{"$(CALL_FUNCTION)", [&]() -> std::string {
				std::string ret;
				if ((*f -> return_type) -> str() != "void") {
					ret += emit(
						"\t$(DECL);\n"
						"\tret_val = ", {
							{"$(DECL)", (*f -> return_type) -> str("ret_val")}
						}, indent);
				} else {
					ret += emit("\t", {}, indent);
				}
				ret += emit("$(NAME)($(SIGNATURE));\n", {
					{"$(NAME)", f -> name},
					{"$(SIGNATURE)", [&]() -> std::string {
						std::string ret;
						for (size_t i = 0; i < f -> arguments.size(); ++i) {
							ret += f -> arguments[i] -> name;
							if (i + 1 < f -> arguments.size()) ret += ", ";
						}
						return ret;
					}()}
				}, indent);
				return ret;
			}()},
			{"$(SETUP_RETURN)", [&]() -> std::string {
				std::string ret;
				if ((*f -> return_type) -> str() != "void") {
					ret += emit(
						"\t$(FLATCC_REFERENCE) __flatcc_reference_ret_val;\n"
						"\t$(SERIALIZE)\n"
						"\t__ocall_wrapper_$(NAME)___return_value_add(&builder, __flatcc_reference_ret_val);\n", {
							{"$(FLATCC_REFERENCE)", (*f -> return_type) -> flatcc_reference()},
							{"$(SERIALIZE)", emit_serialize("ret_val", "__flatcc_reference_ret_val",
								*f -> return_type, indent + 1)},
							{"$(NAME)", f -> name}
						}, indent);
				}
				return ret;
			}()},
			{"$(RELEASE_CONST)", [&]() -> std::string {
				std::string ret;
				for (size_t i = 0; i < f -> arguments.size(); ++i) {
					if (f -> arguments[i] -> type.is_const) {
						ret += emit(
							"\tfree($(ARGUMENT_NAME));\n", {
								{"$(ARGUMENT_NAME)", f -> arguments[i] -> name}
							}, indent);
					}
				}
				return ret;
			}()}
		}, indent);
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

