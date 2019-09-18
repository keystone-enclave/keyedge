#include "emitter.h"

// functions for generic emitting
std::string emit(const std::string& str) {
	return str;
}

std::string emit(const char *str) {
	return std::string(str);
}

template<typename T>
std::string emit(T t) {
	return std::to_string(t);
}

template<typename T, typename... Args>
std::string emit(T t, Args... args) {
	return emit(t) + emit(args...);
}

std::string emit_indent(size_t indent) {
	std::stringstream buffer;
	for (size_t i = 0; i < indent; ++i) buffer << "\t";
	return buffer.str();
}

// macros to help with emitting
#define BEGIN_EMIT std::stringstream buffer; \
	if (indent >= 500) exit(1);
#define RETURN_EMIT return buffer.str()
#define EMIT(...) buffer << emit_indent(indent) << emit(__VA_ARGS__) << "\n";
#define EMIT_(...) buffer << emit_indent(indent) << emit(__VA_ARGS__);
#define APPEND(...) buffer << emit(__VA_ARGS__)
#define APPEND_(...) buffer << emit(__VA_ARGS__) << "\n"

// serialize one variable
std::string emit_serialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent = 0) {
	BEGIN_EMIT;
	EMIT("{");
	if (typeid(*type) == typeid(array_information)) {
		// get the exact data type
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
		// generate a name for the variable to place array data
		std::string data_name = std::string("__keyedge_array_data_") + std::to_string(indent);
		// generate the suffix for the original array and the index for new variable
		std::string suffix, index, length = "1";
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			std::string var = "i" + std::to_string(indent + i);
			length += std::string(" * (") + cast -> lengths[i] + ")";
			index += var + " + (" + cast -> lengths[i] + ") * (";
			suffix = std::string("[") + var + "]" + suffix;
		}
		index += "0";
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			index += ")";
		}
		// define the variable for array data
		EMIT("\t", (*cast -> element_type) -> flatcc_reference(), " *", data_name, " = (",
			(*cast -> element_type) -> flatcc_reference(), " *) malloc((",
			length, ") * sizeof(",	(*cast -> element_type) -> flatcc_reference(), " *));");
		// copy the values
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			std::string var = "i" + std::to_string(indent + i);
			EMIT(emit_indent(i), "\tfor (size_t ", var, " = 0; ", var, " < (size_t) (", cast -> lengths[i], "); ++", var, ") {");
		}
		APPEND(emit_serialize(name + suffix, data_name + "[" + index + "]",
			*cast -> element_type, indent + 1 + cast -> lengths.size()));
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			EMIT(emit_indent(cast -> lengths.size() - 1 - i), "\t}");
		}
		// g++ forbids implicit conversion between "signed char *" and "char *", so cast it for flatcc
		if ((*cast -> element_type) -> str() == "char") {
			EMIT("\t", serialized_name, " = ", type -> flatcc_prefix(),
				"_create(&builder, (signed char *)", data_name, ", ", length, ");"); 
		} else {
			EMIT("\t", serialized_name, " = ", type -> flatcc_prefix(),
				"_create(&builder, ", data_name, ", ", length, ");"); 
		}
		EMIT("\tfree(", data_name, ");");
	} else if (typeid(*type) == typeid(struct_information)) {
		// get the exact data type
		std::shared_ptr<struct_information> cast =
			std::dynamic_pointer_cast<struct_information>(type);
		// start the struct
		EMIT("\t", cast -> name, "_start(&builder);");
		// serialize each member
		std::queue<std::string> sizes;
		for (std::shared_ptr<element_information>& member : cast -> members) {
			if (!(member -> attr_flag & ATTRIBUTE_VLA)) {
				EMIT("\t", (*member -> type) -> flatcc_reference(), " ",
					std::string("__") + std::to_string(indent) + "_" + member -> name, ";");
				APPEND(emit_serialize(name + "." + member -> name,
					std::string("__") + std::to_string(indent) + "_" + member -> name,
					*member -> type, indent + 1));
				if (member -> attr_flag & ATTRIBUTE_SIZE) {
					sizes.push(name + "." + member -> name);
				}
			}
		}
		for (std::shared_ptr<element_information>& member : cast -> members) {
			if (member -> attr_flag & ATTRIBUTE_VLA) {
				std::dynamic_pointer_cast<array_information>(*member -> type) -> lengths =
					std::vector <std::string>(1, sizes.front());
				sizes.pop();
				EMIT("\t", (*member -> type) -> flatcc_reference(), " ",
					std::string("__") + std::to_string(indent) + "_" + member -> name, ";");
				APPEND(emit_serialize(name + "." + member -> name,
					std::string("__") + std::to_string(indent) + "_" + member -> name,
					*member -> type, indent + 1));
			}
		}
		// construct the struct
		for (std::shared_ptr<element_information>& member : cast -> members) {
			EMIT("\t", cast -> name, "_", member -> name,
				"_add(&builder, ", std::string("__") + std::to_string(indent) + "_" + member -> name, ");");
		}
		EMIT("\t", serialized_name, " = ", cast -> name, "_end(&builder);");
	} else if (typeid(*type) == typeid(pointer_information)) {
		// get the exact data type
		std::shared_ptr<pointer_information> cast =
			std::dynamic_pointer_cast<pointer_information>(type);
		std::string data_name = std::string("__keyedge_pointer_data_" + std::to_string(indent));
		EMIT("\tif (", name, ") {");
		EMIT("\t\t", (*cast -> type) -> str(data_name), ";");
		APPEND(emit_serialize(std::string("(*") + name + ")", data_name,
			*cast -> type, indent + 2));
		EMIT("\t\t", serialized_name, " = ", cast -> flatcc_prefix(), "_create(&builder, 0, ", data_name, ");");
		EMIT("\t} else {");
		EMIT("\t\t", cast -> flatcc_prefix(), "_start(&builder);");
		EMIT("\t\t", cast -> flatcc_prefix(), "___is_null_add(&builder, 1);");
		EMIT("\t\t", serialized_name, " = ", cast -> flatcc_prefix(), "_end(&builder);");
		EMIT("\t}");
	} else {
		EMIT("\t", serialized_name, " = ", name, ";");
	}
	EMIT("}");
	RETURN_EMIT;
}

// de-serialize one variable
std::string emit_deserialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent = 0) {
	BEGIN_EMIT;
	EMIT("{");
	if (typeid(*type) == typeid(array_information)) {
		// get the exact data type
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
		// malloc for the array if VLA
		if (cast -> is_vla) {
			EMIT("\t", name, " = (", (*cast -> element_type) -> str("*"), ") malloc((", cast -> lengths[0], ") * sizeof(", (*cast -> element_type) -> str(), "));");
		}
		// generate the suffix for the original array and the index for new variable
		std::string suffix, index;
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			std::string var = "i" + std::to_string(indent + i);
			index += var + " + (" + cast -> lengths[i] + ") * (";
			suffix = std::string("[") + var + "]" + suffix;
		}
		index += "0";
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			index += ")";
		}
		// copy the data
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			std::string var = "i" + std::to_string(indent + i);
			EMIT(emit_indent(i), "\tfor (size_t ", var, " = 0; ", var, " < (size_t) (", cast -> lengths[i], "); ++", var, ") {");
		}
		APPEND(emit_deserialize(name + suffix, cast -> flatcc_prefix() + "_at(" + serialized_name + ", " + index + ")",
			*cast -> element_type, indent + 1 + cast -> lengths.size()));
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			EMIT(emit_indent(cast -> lengths.size() - 1 - i), "\t}");
		}
	} else if (typeid(*type) == typeid(struct_information)) {
		// get the exact data type
		std::shared_ptr<struct_information> cast =
			std::dynamic_pointer_cast<struct_information>(type);
		// de-serialize each member
		std::queue<std::string> sizes;
		for (std::shared_ptr<element_information>& member : cast -> members) {
			if (!(member -> attr_flag & ATTRIBUTE_VLA)) {
				APPEND(emit_deserialize(name + "." + member -> name,
					cast -> name + "_" + member -> name + "(" + serialized_name + ")",
					*member -> type, indent + 1));
				if (member -> attr_flag & ATTRIBUTE_SIZE) {
					sizes.push(name + "." + member -> name);
				}
			}
		}
		for (std::shared_ptr<element_information>& member : cast -> members) {
			if (member -> attr_flag & ATTRIBUTE_VLA) {
				std::dynamic_pointer_cast<array_information>(*member -> type) -> lengths =
					std::vector <std::string>(1, sizes.front());
				sizes.pop();
				APPEND(emit_deserialize(name + "." + member -> name,
					cast -> name + "_" + member -> name + "(" + serialized_name + ")",
					*member -> type, indent + 1));
			}
		}
	} else if (typeid(*type) == typeid(pointer_information)) {
		// get the exact data type
		std::shared_ptr<pointer_information> cast =
			std::dynamic_pointer_cast<pointer_information>(type);
		EMIT("\tif (", cast -> flatcc_prefix(), "___is_null(", serialized_name, ")) {");
		EMIT("\t\t", name, " = 0;");
		EMIT("\t} else {");
		EMIT("\t\t", name, " = (", cast -> str(), ") malloc(sizeof(", (*cast -> type) -> str(), "));");
		APPEND(emit_deserialize(std::string("(*") + name + ")",
			cast -> flatcc_prefix() + "___data(" + serialized_name + ")", *cast -> type, indent + 2));
		EMIT("\t}");
	} else {
		EMIT("\t", name, " = ", serialized_name, ";");
	}
	EMIT("}");
	RETURN_EMIT;
}

// function argument list
std::string emit_function_argument_list(std::shared_ptr<function_information> f, size_t indent = 0) {
	BEGIN_EMIT;
	APPEND("(");
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		APPEND ((*f -> arguments[i] -> type) -> str(f -> arguments[i] -> name));
		if (i + 1 < f -> arguments.size()) APPEND(", ");
	}
	APPEND(")");
	RETURN_EMIT;
}

std::string emit_struct_fbs(std::shared_ptr<struct_information> str, size_t indent = 0) {
	BEGIN_EMIT;
	EMIT("table ", str -> fbs_type(), " {");
	for (std::shared_ptr<element_information>& member : str -> members) {
		EMIT("\t", member -> name, ":", (*member -> type) -> fbs_type(), ";");
	}
	EMIT("}");
	RETURN_EMIT;
}

std::string emit_pointer_fbs(std::shared_ptr<pointer_information> p, size_t indent = 0) {
	BEGIN_EMIT;
	EMIT("table ", p -> fbs_type(), " {");
	EMIT("\t__is_null:bool;");
	EMIT("\t__data:", (*p -> type) -> fbs_type(), ";");
	EMIT("}");
	RETURN_EMIT;
}

std::string emit_header_eapp(size_t indent = 0) {
	BEGIN_EMIT;
	EMIT("#include \"eapp_utils.h\"");
	EMIT("#include \"edge_call.h\"");
	EMIT("#include <syscall.h>");
	EMIT("#include \"ocalls.h\"");
	EMIT("#include \"ocalls_common.h\"");
	EMIT("#include \"ocalls_builder.h\"");
	EMIT("#include \"ocalls_reader.h\"");
	RETURN_EMIT;
}

std::string emit_header_host(size_t indent = 0) {
	BEGIN_EMIT;
	EMIT("#include <keystone.h>");
	EMIT("#include <edge_call.h>");
	EMIT("#include \"ocalls.h\"");
	EMIT("#include \"ocalls_common.h\"");
	EMIT("#include \"ocalls_builder.h\"");
	EMIT("#include \"ocalls_reader.h\"");
	RETURN_EMIT;
}

std::string emit_end_eapp(size_t indent = 0) {
	BEGIN_EMIT;
	RETURN_EMIT;
}

std::string emit_end_host(size_t indent = 0) {
	BEGIN_EMIT;
	// register functions
	EMIT("void register_functions() {");
	for (auto& function_pair : function_lookup) {
		std::shared_ptr<function_information> f = function_pair.second;
		EMIT("\tregister_call(__function_", f -> name, ", ", "__wrapper_", f -> name, ");");
	}
	EMIT("\treturn;");
	EMIT("}");
	RETURN_EMIT;
}

std::string emit_function_common(std::shared_ptr<function_information> f, size_t indent = 0) {
	BEGIN_EMIT;
	EMIT("const unsigned long __function_", f -> name, " = ", f -> index, ";");
	RETURN_EMIT;
}

std::string emit_function_eapp(std::shared_ptr<function_information> f, size_t indent = 0) {
	BEGIN_EMIT;
	// function header
	EMIT((*f -> return_type) -> str(f -> name), emit_function_argument_list(f), " {");
	// initiate a flatcc buffer
	EMIT("\tflatcc_builder_t builder;");
	EMIT("\tflatcc_builder_init(&builder);");
	// serialize the arguments
	std::queue<std::string> sizes;
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		if (!(f -> arguments[i] -> attr_flag & ATTRIBUTE_VLA)) {
			EMIT("\t", (*f -> arguments[i] -> type) -> flatcc_reference(), " ",
				"__flatcc_reference_", f -> arguments[i] -> name, ";");
			APPEND(emit_serialize(f -> arguments[i] -> name,
				std::string("__flatcc_reference_") + f -> arguments[i] -> name,
				*f -> arguments[i] -> type, indent + 1));
			if (f -> arguments[i] -> attr_flag & ATTRIBUTE_SIZE) {
				sizes.push(f -> arguments[i] -> name);
			}
		}
	}
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		if (f -> arguments[i] -> attr_flag & ATTRIBUTE_VLA) {
			std::dynamic_pointer_cast<array_information>(*f -> arguments[i] -> type) -> lengths =
				std::vector <std::string>(1, sizes.front());
			sizes.pop();
			EMIT("\t", (*f -> arguments[i] -> type) -> flatcc_reference(), " ",
				"__flatcc_reference_", f -> arguments[i] -> name, ";");
			APPEND(emit_serialize(f -> arguments[i] -> name,
				std::string("__flatcc_reference_") + f -> arguments[i] -> name,
				*f -> arguments[i] -> type, indent + 1));
		}
	}
	// construct the argument table
	EMIT("\t__ocall_wrapper_", f -> name, "_start_as_root(&builder);");
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		EMIT("\t__ocall_wrapper_", f -> name, "_", f -> arguments[i] -> name,
			"_add(&builder, __flatcc_reference_", f -> arguments[i] -> name, ");");
	}
	EMIT("\t__ocall_wrapper_", f -> name, "_end_as_root(&builder);");
	// get the table
	EMIT("\tvoid* __buf;");
	EMIT("\tsize_t __size;");
	EMIT("\t__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);");
	// get some space for the return value
	EMIT("\tchar return_address[1024];");
	// perform the ocall
	EMIT("\tocall(__function_", f -> name, ", __buf, __size, return_address, 1024);");
	// clean-up
	EMIT("\tfree(__buf);");
	EMIT("\tflatcc_builder_clear(&builder);");
	// return the value
	EMIT("\t", (*f -> return_type) -> str("__return_value"), ";");
	std::string ret = std::string("__ocall_wrapper_") + f -> name +
		"___return_value(__ocall_wrapper_" + f -> name + "_as_root(return_address))";
	APPEND(emit_deserialize("__return_value", ret, *f -> return_type, indent + 1));
	EMIT("\treturn __return_value;");
	EMIT("}");
	RETURN_EMIT;
}

std::string emit_function_host(std::shared_ptr<function_information> f, size_t indent = 0) {
	BEGIN_EMIT;
	// wrapper
	EMIT("void __wrapper_", f -> name, "(void* buffer) {");
	EMIT("\tstruct edge_call* edge_call = (struct edge_call*) buffer;");
	EMIT("\tuintptr_t call_args;");
	EMIT("\tsize_t arg_len;");
	EMIT("\tif (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {");
    EMIT("\t\tedge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;");
    EMIT("\t\treturn;");
	EMIT("\t}");
	// allocate all arguments
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		EMIT("\t", (*f -> arguments[i] -> type) -> str(f -> arguments[i] -> name), ";");
	}
	// de-serialize the arguments
	EMIT("\t__ocall_wrapper_", f -> name, "_table_t function_reference = __ocall_wrapper_", f -> name, "_as_root((void *) call_args);");
	std::queue<std::string> sizes;
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		if (!(f -> arguments[i] -> attr_flag & ATTRIBUTE_VLA)) {
			EMIT("\t", (*f -> arguments[i] -> type) -> flatcc_type(), " ",
				"__flatcc_reference_", f -> arguments[i] -> name, " = ",
				"__ocall_wrapper_", f -> name, "_", f -> arguments[i] -> name, "(function_reference);");
			APPEND(emit_deserialize(f -> arguments[i] -> name,
				std::string("__flatcc_reference_") + f -> arguments[i] -> name,
				*f -> arguments[i] -> type, indent + 1));
			if (f -> arguments[i] -> attr_flag & ATTRIBUTE_SIZE) {
				sizes.push(f -> arguments[i] -> name);
			}
		}
	}
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		if (f -> arguments[i] -> attr_flag & ATTRIBUTE_VLA) {
			std::dynamic_pointer_cast<array_information>(*f -> arguments[i] -> type) -> lengths =
				std::vector <std::string>(1, sizes.front());
			sizes.pop();
			EMIT("\t", (*f -> arguments[i] -> type) -> flatcc_type(), " ",
				"__flatcc_reference_", f -> arguments[i] -> name, " = ",
				"__ocall_wrapper_", f -> name, "_", f -> arguments[i] -> name, "(function_reference);");
			APPEND(emit_deserialize(f -> arguments[i] -> name,
				std::string("__flatcc_reference_") + f -> arguments[i] -> name,
				*f -> arguments[i] -> type, indent + 1));
		}
	}
	// call the function
	EMIT("\t", (*f -> return_type) -> str("ret_val"), ";");
	EMIT_("\tret_val = ", f -> name, "(");
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		APPEND(f -> arguments[i] -> name);
		if (i + 1 < f -> arguments.size()) APPEND(", ");
	}
	APPEND_(");");
	// initiate a flatcc buffer
	EMIT("\tflatcc_builder_t builder;");
	EMIT("\tflatcc_builder_init(&builder);");
	// setup return data
	EMIT("\t", (*f -> return_type) -> flatcc_reference(), " __flatcc_reference_ret_val;");
	APPEND(emit_serialize("ret_val", std::string("__flatcc_reference_ret_val"),
		*f -> return_type, indent + 1));
	EMIT("\t__ocall_wrapper_", f -> name, "_start_as_root(&builder);");
	EMIT("\t__ocall_wrapper_", f -> name, "___return_value_add(&builder, __flatcc_reference_ret_val);");
	EMIT("\t__ocall_wrapper_", f -> name, "_end_as_root(&builder);");
	// get the table
	EMIT("\tvoid* __buf;");
	EMIT("\tsize_t __size;");
	EMIT("\t__buf = (void *) flatcc_builder_finalize_buffer(&builder, &__size);");
	EMIT("\tuintptr_t data_section = edge_call_data_ptr();");
	EMIT("\tmemcpy((void *) data_section, __buf, __size);"); 
	EMIT("\tfree(__buf);");
	EMIT("\tflatcc_builder_clear(&builder);");
	EMIT("\tif (edge_call_setup_ret(edge_call, (void*) data_section, 1024)) {");
    EMIT("\t\tedge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;");
	EMIT("\t} else {");
    EMIT("\t\tedge_call -> return_data.call_status = CALL_STATUS_OK;");
	EMIT("\t}");
	EMIT("\treturn;");
	EMIT("}");
	RETURN_EMIT;
}

std::string emit_function_fbs(std::shared_ptr<function_information> f, size_t indent = 0) {
	BEGIN_EMIT;
	EMIT("table __ocall_wrapper_", f -> name, " {");
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		EMIT("\t", f -> arguments[i] -> name, ":", (*f -> arguments[i] -> type) -> fbs_type(), ";");
	}
	EMIT("\t__return_value:", (*f -> return_type) -> fbs_type(), ";");
	EMIT("}");
	RETURN_EMIT;
}

std::string emit_common(size_t indent) {
	BEGIN_EMIT;
	for (auto& function_pair : function_lookup) {
		APPEND(emit_function_common(function_pair.second));
	}
	RETURN_EMIT;
}

std::string emit_eapp(size_t indent) {
	BEGIN_EMIT;
	APPEND(emit_header_eapp());
	for (auto& function_pair : function_lookup) {
		APPEND(emit_function_eapp(function_pair.second));
	}
	APPEND(emit_end_eapp());
	RETURN_EMIT;
}

std::string emit_host(size_t indent) {
	BEGIN_EMIT;
	APPEND(emit_header_host());
	for (auto& function_pair : function_lookup) {
		APPEND(emit_function_host(function_pair.second));
	}
	APPEND(emit_end_host());
	RETURN_EMIT;
}

std::string emit_fbs(size_t indent) {
	BEGIN_EMIT;
	for (auto& str : struct_pool) {
		APPEND(emit_struct_fbs(str));
	}
	for (auto& pointer_pair : pointer_lookup) {
		APPEND(emit_pointer_fbs(pointer_pair.second));
	}
	for (auto& function_pair : function_lookup) {
		APPEND(emit_function_fbs(function_pair.second));
	}
	RETURN_EMIT;
}

