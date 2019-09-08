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
#define BEGIN_EMIT std::stringstream buffer
#define RETURN_EMIT return buffer.str()
#define EMIT(...) buffer << emit_indent(indent) << emit(__VA_ARGS__) << "\n";
#define EMIT_(...) buffer << emit_indent(indent) << emit(__VA_ARGS__);
#define APPEND(...) buffer << emit(__VA_ARGS__)
#define APPEND_(...) buffer << emit(__VA_ARGS__) << "\n"

// serialize one variable
std::string emit_serialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent) {
	BEGIN_EMIT;
	EMIT("{");
	if (typeid(*type) == typeid(array_information)) {
		// get the exact data type
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
		// generate a name for the variable to place array data
		std::string data_name = std::string("__keyedge_array_data");
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
		EMIT("\t", (*cast -> element_type) -> flatcc_reference(), " ", data_name, "[", length, "]", ";");
		// copy the values
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			std::string var = "i" + std::to_string(indent + i);
			EMIT(emit_indent(i), "\tfor (size_t ", var, " = 0; ", var, " < (", cast -> lengths[i], "); ++", var, ") {");
		}
		APPEND(emit_serialize(name + suffix, data_name + "[" + index + "]",
			*cast -> element_type, indent + 1 + cast -> lengths.size()));
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			EMIT(emit_indent(cast -> lengths.size() - 1 - i), "\t}");
		}
		EMIT("\t", serialized_name, " = ", type -> flatcc_prefix(),
			"_create(&builder, ", data_name, ", ", length, ");"); 
	} else if (typeid(*type) == typeid(struct_information)) {
		// get the exact data type
		std::shared_ptr<struct_information> cast =
			std::dynamic_pointer_cast<struct_information>(type);
		// start the struct
		EMIT("\t", cast -> name, "_start(&builder);");
		// serialize each member
		for (std::shared_ptr<element_information>& member : cast -> members) {
			EMIT("\t", (*member -> type) -> flatcc_reference(), " ",
				"__keyedge_" + member -> name, ";");
			APPEND(emit_serialize(name + "." + member -> name,
				"__keyedge_" + member -> name,
				*member -> type, indent + 1));
		}
		// construct the struct
		for (std::shared_ptr<element_information>& member : cast -> members) {
			EMIT("\t", cast -> name, "_", member -> name,
				"_add(&builder, ", "__keyedge_" + member -> name, ");");
		}
		EMIT("\t", serialized_name, " = ", cast -> name, "_end(&builder);");
	} else {
		EMIT("\t", serialized_name, " = ", name, ";");
	}
	EMIT("}");
	RETURN_EMIT;
}

// de-serialize one variable
std::string emit_deserialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent) {
	BEGIN_EMIT;
	EMIT("{");
	if (typeid(*type) == typeid(array_information)) {
		// get the exact data type
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
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
			EMIT(emit_indent(i), "\tfor (size_t ", var, " = 0; ", var, " < (", cast -> lengths[i], "); ++", var, ") {");
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
		for (std::shared_ptr<element_information>& member : cast -> members) {
			APPEND(emit_deserialize(name + "." + member -> name,
				cast -> name + "_" + member -> name + "(" + serialized_name + ")",
				*member -> type, indent + 1));
		}
	} else {
		EMIT("\t", name, " = ", serialized_name, ";");
	}
	EMIT("}");
	RETURN_EMIT;
}

// function argument list
std::string emit_function_argument_list(std::shared_ptr<function_information> f) {
	BEGIN_EMIT;
	APPEND("(");
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		APPEND ((*f -> arguments[i] -> type) -> str(f -> arguments[i] -> name));
		if (i + 1 < f -> arguments.size()) APPEND(", ");
	}
	APPEND(")");
	RETURN_EMIT;
}

std::string emit_struct_declaration(size_t indent) {
	BEGIN_EMIT;
	for (std::shared_ptr<struct_information>& str : struct_pool) {
		EMIT("typedef struct {");
		for (std::shared_ptr<element_information>& member : str -> members) {
			EMIT("\t", (*member -> type) -> str(member -> name), ";");
		}
		EMIT("} ", str -> name, ";");
	}
	RETURN_EMIT;
}

std::string emit_header_eapp(size_t indent) {
	BEGIN_EMIT;
	EMIT("#include \"eapp_utils.h\"");
	EMIT("#include \"edge_call.h\"");
	EMIT("#include <syscall.h>");
	EMIT("#include \"ocalls_builder.h\"");
	EMIT("#include \"ocalls_reader.h\"");
	// struct declaration
	APPEND(emit_struct_declaration(indent));
	RETURN_EMIT;
}

std::string emit_header_host(size_t indent) {
	BEGIN_EMIT;
	EMIT("#include <keystone.h>");
	EMIT("#include <edge_call.h>");
	EMIT("#include \"ocalls_builder.h\"");
	EMIT("#include \"ocalls_reader.h\"");
	// struct declaration
	APPEND(emit_struct_declaration(indent));
	RETURN_EMIT;
}

std::string emit_end_eapp(size_t indent) {
	BEGIN_EMIT;
	RETURN_EMIT;
}

std::string emit_end_host(size_t indent) {
	BEGIN_EMIT;
	// register functions
	EMIT("void register_functions() {");
	for (auto& function_pair : function_lookup) {
		std::shared_ptr<function_information> f = function_pair.second;
		EMIT("\tconst unsigned long __function_", f -> name, " = ", f -> index, ";");
		EMIT("\tregister_call(__function_", f -> name, ", ", "__wrapper_", f -> name, ");");
	}
	EMIT("\treturn;");
	EMIT("}");
	RETURN_EMIT;
}

std::string emit_function_eapp(std::shared_ptr<function_information> f, size_t indent) {
	BEGIN_EMIT;
	// function index
	EMIT(std::string("const unsigned long __function_"), f -> name, " = ", f -> index, ";");
	// function header
	EMIT((*f -> return_type) -> str(f -> name), emit_function_argument_list(f), " {");
	// initiate a flatcc buffer
	EMIT("\tflatcc_builder_t builder;");
	EMIT("\tflatcc_builder_init(&builder);");
	// serialize the arguments
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		EMIT("\t", (*f -> arguments[i] -> type) -> flatcc_reference(), " ",
			"__flatcc_reference_", f -> arguments[i] -> name, ";");
		APPEND(emit_serialize(f -> arguments[i] -> name,
			std::string("__flatcc_reference_") + f -> arguments[i] -> name,
			*f -> arguments[i] -> type, indent + 1));
	}
	// construct the argument table
	EMIT("\t__ocall_wrapper_", f -> name, "_start_as_root(&builder);");
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		EMIT("\t__ocall_wrapper_", f -> name, "_", f -> arguments[i] -> name,
			"_add(&builder, __flatcc_reference_", f -> arguments[i] -> name, ");");
	}
	EMIT("\t__ocall_wrapper_", f -> name, "_end_as_root(&builder);");
	// get the table
	EMIT("\tvoid* buf;");
	EMIT("\tsize_t size;");
	EMIT("\tbuf = (void *) flatcc_builder_finalize_buffer(&builder, &size);");
	// get some space for the return value
	EMIT("\tchar return_address[1024];");
	// perform the ocall
	EMIT("\tocall(__function_", f -> name, ", buf, size, return_address, 1024);");
	// clean-up
	EMIT("\tfree(buf);");
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

std::string emit_function_host(std::shared_ptr<function_information> f, size_t indent) {
	BEGIN_EMIT;
	// give a definition of the original function
	EMIT((*f -> return_type) -> str(f -> name), emit_function_argument_list(f), ";");
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
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		EMIT("\t", (*f -> arguments[i] -> type) -> flatcc_type(), " ",
			"__flatcc_reference_", f -> arguments[i] -> name, " = ",
			"__ocall_wrapper_", f -> name, "_", f -> arguments[i] -> name, "(function_reference);");
		APPEND(emit_deserialize(f -> arguments[i] -> name,
			std::string("__flatcc_reference_") + f -> arguments[i] -> name,
			*f -> arguments[i] -> type, indent + 1));
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
	EMIT("\tvoid* buf;");
	EMIT("\tsize_t size;");
	EMIT("\tbuf = (void *) flatcc_builder_finalize_buffer(&builder, &size);");
	EMIT("\tuintptr_t data_section = edge_call_data_ptr();");
	EMIT("\tmemcpy((void *) data_section, buf, size);"); 
	EMIT("\tfree(buf);");
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

