#include "index.h"

// the variable name of flatcc reference
std::string flatcc_reference_var(size_t indent, std::string argument_name) {
	return emit("__$(INDENT)_$(ARGUMENT_NAME)", {
		{"$(INDENT)", std::to_string(indent)},
		{"$(ARGUMENT_NAME)", argument_name}
	});
}

std::string emit_serialize_argument(const std::string& name, std::shared_ptr<element_information> argument, size_t indent = 0) {
	return emit(
		"\t$(FLATCC_REFERENCE) $(FLATCC_REFERENCE_VAR);\n"
		"\t$(SERIALIZE_ARGUMENT)\n", {
			// the reference used in flatcc
			{"$(FLATCC_REFERENCE)", (*argument -> type) -> flatcc_reference()},
			{"$(FLATCC_REFERENCE_VAR)", flatcc_reference_var(indent, argument -> name)},
			// the serialization of the argument
			{"$(SERIALIZE_ARGUMENT)", emit_serialize(
				name, flatcc_reference_var(indent, argument -> name),
				*argument -> type, indent + 1)}
		}, indent);
}

std::string emit_deserialize_argument(const std::string& name, const std::string& serialized_name, std::shared_ptr<function_information> type,
	std::shared_ptr<element_information> argument, size_t indent) {
	return emit_deserialize(name, emit("__ocall_wrapper_$(FUNCTION_NAME)_$(ARGUMENT_NAME)($(SERIALIZED_NAME))", {
			{"$(FUNCTION_NAME)", type -> name},
			{"$(ARGUMENT_NAME)", argument -> name},
			{"$(SERIALIZED_NAME)", serialized_name}
		}), *argument -> type, indent);
}

std::string emit_function_caller(std::shared_ptr<function_information> f, size_t indent) {
	std::queue<std::string> sizes;
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		if (f -> arguments[i] -> attr_flag & ATTRIBUTE_SIZE) {
			sizes.push(f -> arguments[i] -> name);
		}
	}
	return emit(
		"$(FUNCTION_SIGNATURE)$(FUNCTION_ARGUMENTS) {\n"
		"\tflatcc_builder_t builder;\n"
		"\tflatcc_builder_init(&builder);\n"
		"$(SERIALIZE_NON_VLA_ARGUMENTS)"
		"$(SERIALIZE_VLA_ARGUMENTS)"
		// construct the argument table
		"\t__ocall_wrapper_$(NAME)_start_as_root(&builder);\n"
		"$(ADD_ARGUMENTS)"
		"\t__ocall_wrapper_$(NAME)_end_as_root(&builder);\n"
		// get the table
		"\tvoid* __buf;\n"
		"\tsize_t __size;\n"
		"\t__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);\n"
		// do the routine
		"$(ROUTINE)"
		// clean-up
		"\tfree(__buf);\n"
		"\tflatcc_builder_clear(&builder);\n"
		"$(RETURN)"
		"}", {
			{"$(FUNCTION_SIGNATURE)", (*f -> return_type) -> str(f -> name)},
			{"$(FUNCTION_ARGUMENTS)", [&]() -> std::string {
				std::string ret = "(";
				for (size_t i = 0; i < f -> arguments.size(); ++i) {
					ret += (f -> arguments[i] -> type.is_const ? "const " : ""); 
					ret += (*f -> arguments[i] -> type) -> str(f -> arguments[i] -> name);
					if (i + 1 < f -> arguments.size()) ret += ", ";
				}
				ret += ")";
				return ret;
			}()},
			{"$(SERIALIZE_NON_VLA_ARGUMENTS)", [&]() -> std::string {
				std::string ret;
				for (size_t i = 0; i < f -> arguments.size(); ++i) {
					if (!(f -> arguments[i] -> attr_flag & ATTRIBUTE_VLA)) {
						ret += emit_serialize_argument(f -> arguments[i] -> name, f -> arguments[i], indent);
					}
				}
				return ret;
			}()},
			{"$(SERIALIZE_VLA_ARGUMENTS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& argument : f -> arguments) {
					if (argument -> attr_flag & ATTRIBUTE_VLA) {
						if (argument -> attr_flag & ATTRIBUTE_STR) {
							std::string var_name = emit("__$(INDENT)_$(ARGUMENT_NAME)_keyedge_size", {
								{"$(INDENT)", std::to_string(indent)},
								{"$(ARGUMENT_NAME)", argument -> name}
						});
						ret += emit("\tsize_t $(VAR_NAME) = strlen($(ARGUMENT_NAME)) + 1;\n", {
							{"$(VAR_NAME)", var_name},
							{"$(ARGUMENT_NAME)", argument -> name}
						}, indent);
						std::dynamic_pointer_cast<array_information>(*argument -> type) -> lengths =
							std::vector <std::string>(1, var_name);
					} else {
						std::dynamic_pointer_cast<array_information>(*argument -> type) -> lengths =
							std::vector <std::string>(1, sizes.front());
						sizes.pop();
					}
					ret += emit_serialize_argument(argument -> name, argument, indent);
				}
			}
			return ret;
		}()},
		{"$(NAME)", f -> name},
		{"$(ADD_ARGUMENTS)", [&]() -> std::string {
			std::string ret;
			for (size_t i = 0; i < f -> arguments.size(); ++i) {
				ret += emit(
					"\t__ocall_wrapper_$(NAME)_$(ARGUMENT_NAME)_add(&builder, $(FLATCC_REFERENCE_VAR));\n", {
						{"$(NAME)", f -> name},
						{"$(ARGUMENT_NAME)", f -> arguments[i] -> name},
						{"$(FLATCC_REFERENCE_VAR)", flatcc_reference_var(indent, f -> arguments[i] -> name)}
					}, indent);
			}
			return ret;
		}()},
		{"$(ROUTINE)", [&]() -> std::string {
			if (f -> attr_flag & ATTRIBUTE_INVERSE) {
				return emit(
					"\t*(int*) _shared_start = sizeof(int) + __size;\n"
					"\t*(((int*) _shared_start) + 1) = __function_$(NAME);\n"
					"\tmemcpy((char*) _shared_start + 2 * sizeof(int), (char*) __buf, __size);\n"
					"\tenclave.resume();\n", {
						{"$(NAME)", f -> name}
					}, indent);
			} else {
				return emit(
					"\tchar return_address[$(BUFFER_SIZE)];\n"
					"\tocall(__function_$(NAME), __buf, __size, return_address, $(BUFFER_SIZE));\n", {
						{"$(NAME)", f -> name},
						{"$(BUFFER_SIZE)", f -> buffer_size}
					}, indent);
			}
		}()},
		{"$(RETURN)", [&]() -> std::string {
			std::string ret;
			if ((*f -> return_type) -> str() != "void") {
				if (f -> attr_flag & ATTRIBUTE_INVERSE) {
					std::string serialized_return = emit(
						"__ocall_wrapper_$(NAME)___return_value(__ocall_wrapper_$(NAME)_as_root((char*) _shared_start + __result -> call_arg_offset))", {
							{"$(NAME)", f -> name}
						});
					ret = emit(
						"\tedge_call* __result = (edge_call*) _shared_start;\n"
						"\t$(DECLARE_RETURN);\n"
						"\t$(DESERIALIZE_RETURN);\n"
						"\treturn __return_value;\n", {
							{"$(DECLARE_RETURN)", (*f -> return_type) -> str("__return_value")},
							{"$(DESERIALIZE_RETURN)", emit_deserialize("__return_value",
								serialized_return, *f -> return_type, indent + 1)}
						}, indent);
				} else {
					std::string serialized_return = emit(
						"__ocall_wrapper_$(NAME)___return_value(__ocall_wrapper_$(NAME)_as_root(return_address))", {
							{"$(NAME)", f -> name}
						});
					ret = emit(
						"\t$(DECLARE_RETURN);\n"
						"\t$(DESERIALIZE_RETURN);\n"
						"\treturn __return_value;\n", {
							{"$(DECLARE_RETURN)", (*f -> return_type) -> str("__return_value")},
							{"$(DESERIALIZE_RETURN)", emit_deserialize("__return_value",
								serialized_return, *f -> return_type, indent + 1)}
						}, indent);
				}
			}
			return ret;
		}()}
	}, indent);
}

