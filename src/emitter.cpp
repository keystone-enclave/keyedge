#include "emitter.h"

std::string emit_indent(size_t indent) {
	std::stringstream buffer;
	for (size_t i = 0; i < indent; ++i) buffer << "\t";
	return buffer.str();
}

// serialize one variable
std::string emit_serialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent) {
	if (typeid(*type) == typeid(array_information)) {
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
		std::stringstream buffer;
		std::string data_name = std::string("__data_") + std::to_string(buffer.str().size());
		buffer << emit_indent(indent) << cast -> serialized_type(data_name) << ";\n";
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
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			std::string var = "i" + std::to_string(indent + i);
			buffer << emit_indent(indent + i);
			buffer << "for (size_t " + var + " = 0; " + var + " < (" + cast -> lengths[i] + "); ++" + var + ") {\n";
		}
		buffer << emit_serialize(name + suffix, data_name + "[" + index + "]",
			*cast -> element_type, indent + cast -> lengths.size());
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			buffer << emit_indent(indent + cast -> lengths.size() - 1 - i);
			buffer << "}";
		}
		buffer << emit_indent(indent) << serialized_name << " = " << type -> flatcc_prefix() <<
			"_create(&builder, " << data_name << ", c_vec_len(" << data_name << "));\n"; 
		return buffer.str();
	} 
	if (typeid(*type) == typeid(struct_information)) {
/*
		std::shared_ptr<struct_information> cast =
			std::dynamic_pointer_cast<struct_information>(type);
		std::stringstream buffer;
		buffer << emit_indent(indent) << "// serializing " << cast -> name << "\n";
		for (std::shared_ptr<element_information>& member : cast -> members) {
			buffer << emit_serialize(name + "." + member -> name,
				std::string("&((") + cast -> serialized_type() + "*) (" + address + ")) -> " + member -> name,
				*member -> type, indent + 1);
		}
		return buffer.str();
*/
		return "";
	}
	return emit_indent(indent) + serialized_name + " = " + name + ";\n";
}

// de-serialize one variable
std::string emit_deserialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent) {
	if (typeid(*type) == typeid(array_information)) {
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
		std::stringstream buffer;
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
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			std::string var = "i" + std::to_string(indent + i);
			buffer << emit_indent(indent + i);
			buffer << "for (size_t " + var + " = 0; " + var + " < (" + cast -> lengths[i] + "); ++" + var + ") {\n";
		}
		buffer << emit_deserialize(name + suffix, cast -> flatcc_prefix() + "_at(" + serialized_name + ", " + index + ")",
			*cast -> element_type, indent + cast -> lengths.size());
		for (size_t i = 0; i < cast -> lengths.size(); ++i) {
			buffer << emit_indent(indent + cast -> lengths.size() - 1 - i);
			buffer << "}";
		}
		buffer << "\n";
		return buffer.str();
	} 
	if (typeid(*type) == typeid(struct_information)) {
/*
		std::shared_ptr<struct_information> cast =
			std::dynamic_pointer_cast<struct_information>(type);
		std::stringstream buffer;
		buffer << emit_indent(indent) << "// de-serializing " << cast -> name << "\n";
		for (std::shared_ptr<element_information>& member : cast -> members) {
			buffer << emit_deserialize(name + "." + member -> name,
				std::string("&((") + cast -> serialized_type() + "*) (" + address + ")) -> " + member -> name,
				*member -> type, indent + 1);
		}
		return buffer.str();
*/
		return "";
	}
	return emit_indent(indent) + name + " = " + serialized_name + ";\n";
}

// function argument list
std::string emit_function_argument_list(std::shared_ptr<function_information> f) {
	std::stringstream buffer;
	buffer << "(";
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		buffer << (*f -> arguments[i] -> type) -> str(f -> arguments[i] -> name);
		if (i + 1 < f -> arguments.size()) buffer << ", ";
	}
	buffer << ")";
	return buffer.str();
}

std::string emit_struct_declaration(size_t indent) {
	std::stringstream buffer;
	for (std::shared_ptr<struct_information>& str : struct_pool) {
		buffer << emit_indent(indent);
		buffer << "typedef struct {\n";
		for (std::shared_ptr<element_information>& member : str -> members) {
			buffer << emit_indent(indent + 1);
			buffer << (*member -> type) -> str(member -> name) << ";\n";
		}
		buffer << emit_indent(indent) << "} " << str -> name << ";\n";
		buffer << "typedef struct {\n";
		for (std::shared_ptr<element_information>& member : str -> members) {
			buffer << emit_indent(indent + 1);
			buffer << (*member -> type) -> serialized_type(member -> name) << ";\n";
		}
		buffer << emit_indent(indent) << "} __serialized_" << str -> name << ";\n";
	}
	return buffer.str();
}

std::string emit_header_eapp(size_t indent) {
	std::stringstream buffer;
	buffer << emit_indent(indent);
	buffer << "#include \"eapp_utils.h\"\n";
	buffer << emit_indent(indent);
	buffer << "#include \"edge_call.h\"\n";
	buffer << emit_indent(indent);
	buffer << "#include <syscall.h>\n";
	buffer << emit_indent(indent);
	buffer << "#include \"ocalls_builder.h\"\n";
	buffer << emit_indent(indent);
	buffer << "#include \"ocalls_reader.h\"\n";
	buffer << emit_indent(indent);
	buffer << "#define c_vec_len(V) (sizeof(V)/sizeof((V)[0]))\n";
	// struct declaration
	buffer << emit_struct_declaration(indent);
/*
	buffer << emit_indent(indent);
	buffer << "int main();\n";
	buffer << emit_indent(indent);
	buffer << "int __eapp_main() {\n";
	buffer << emit_indent(indent + 1);
	buffer << "return main();\n";
	buffer << emit_indent(indent);
	buffer << "}\n";
*/
	return buffer.str();
}

std::string emit_header_host(size_t indent) {
	std::stringstream buffer;
	buffer << emit_indent(indent);
	buffer << "#include <keystone.h>\n";
	buffer << emit_indent(indent);
	buffer << "#include <edge_call.h>\n";
	buffer << emit_indent(indent);
	buffer << "#include \"ocalls_builder.h\"\n";
	buffer << emit_indent(indent);
	buffer << "#include \"ocalls_reader.h\"\n";
	buffer << emit_indent(indent);
	buffer << "#define c_vec_len(V) (sizeof(V)/sizeof((V)[0]))\n";
	// struct declaration
	buffer << emit_struct_declaration(indent);
	return buffer.str();
}

std::string emit_end_eapp(size_t indent) {
	return "";
}

std::string emit_end_host(size_t indent) {
	std::stringstream buffer;
	// register functions
	buffer << emit_indent(indent);
	buffer << "void register_functions() {\n";
	for (auto& function_pair : function_lookup) {
		std::shared_ptr<function_information> f = function_pair.second;
		buffer << emit_indent(indent + 1);
		buffer << std::string("const unsigned long __function_") << f -> name << " = " << f -> index << ";\n";
		buffer << emit_indent(indent + 1);
		buffer << "register_call(__function_" << f -> name << ", " <<
			"__wrapper_" << f -> name << ");\n";
	}
	buffer << emit_indent(indent + 1);
	buffer << "return;\n";
	buffer << emit_indent(indent);
	buffer << "}\n";
	return buffer.str();
}

std::string emit_function_eapp(std::shared_ptr<function_information> f, size_t indent) {
	std::stringstream buffer;
	// function index
	buffer << emit_indent(indent);
	buffer << std::string("const unsigned long __function_") << f -> name << " = " << f -> index << ";\n";
	// function header
	buffer << emit_indent(indent);
	buffer << (*f -> return_type) -> serialized_type(f -> name);
	buffer << emit_function_argument_list(f);
	buffer << " {\n";
	// initiate a flatcc buffer
	buffer << emit_indent(indent + 1); 
	buffer << "flatcc_builder_t builder;\n";
	buffer << emit_indent(indent + 1); 
	buffer << "flatcc_builder_init(&builder);\n";
	// serialize the arguments
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		buffer << emit_indent(indent + 1);
		buffer << (*f -> arguments[i] -> type) -> flatcc_reference() << " " <<
			"__flatcc_reference_" << f -> arguments[i] -> name << ";\n";
		buffer << emit_serialize(f -> arguments[i] -> name,
			std::string("__flatcc_reference_") + f -> arguments[i] -> name,
			*f -> arguments[i] -> type, indent + 1);
	}
	// serialize the argument table
	buffer << emit_indent(indent + 1); 
	buffer << "__ocall_wrapper_" << f -> name << "_start_as_root(&builder);\n";
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		buffer << emit_indent(indent + 1);
		buffer << "__ocall_wrapper_" << f -> name << "_" << f -> arguments[i] -> name <<
			"_add(&builder, __flatcc_reference_" << f -> arguments[i] -> name << ");\n";
	}
	buffer << emit_indent(indent + 1); 
	buffer << "__ocall_wrapper_" << f -> name << "_end_as_root(&builder);\n";
	// get the table
	buffer << emit_indent(indent + 1); 
	buffer << "void* buf;\n";
	buffer << emit_indent(indent + 1); 
	buffer << "size_t size;\n";
	buffer << emit_indent(indent + 1); 
	buffer << "buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);\n";
	// get some space for the return value
	buffer << emit_indent(indent + 1); 
	buffer << "char return_address[1024];\n";
	// perform the ocall
	buffer << emit_indent(indent + 1);
	buffer << "ocall(__function_" << f -> name << ", buf, size, return_address, 1024);\n";
	// clean-up
	buffer << emit_indent(indent + 1) << "free(buf);\n";
	buffer << emit_indent(indent + 1) << "flatcc_builder_clear(&builder);\n";
	// return the value
	buffer << emit_indent(indent + 1);
	buffer << (*f -> return_type) -> str("__return_value") << ";\n";
	std::string ret = std::string("__ocall_wrapper_") + f -> name + "___return_value(__ocall_wrapper_" + f -> name + "_as_root(return_address))";
	buffer << emit_deserialize("__return_value", ret, *f -> return_type, indent + 1);
	buffer << emit_indent(indent + 1);
	buffer << "return __return_value;\n";
	// function end
	buffer << emit_indent(indent);
	buffer << "}\n";
	return buffer.str();
}

std::string emit_function_host(std::shared_ptr<function_information> f, size_t indent) {
	std::stringstream buffer;
	// give a definition of the original function
	buffer << emit_indent(indent);
	buffer << (*f -> return_type) -> str(f -> name);
	buffer << emit_function_argument_list(f);
	buffer << ";\n";
	// wrapper
	buffer << emit_indent(indent);
	buffer << "void __wrapper_" << f -> name << "(void* buffer) {\n";
	buffer << emit_indent(indent + 1);
	buffer << "struct edge_call* edge_call = (struct edge_call*) buffer;\n";
	buffer << emit_indent(indent + 1);
	buffer << "uintptr_t call_args;\n";
	buffer << emit_indent(indent + 1);
	buffer << "size_t arg_len;\n";
	buffer << emit_indent(indent + 1);
	buffer << "if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {\n";
	buffer << emit_indent(indent + 2);
    buffer << "edge_call -> return_data.call_status = CALL_STATUS_BAD_OFFSET;\n";
	buffer << emit_indent(indent + 2);
    buffer << "return;\n";
	buffer << emit_indent(indent + 1);
	buffer << "}\n";
	// allocate all arguments
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		buffer << emit_indent(indent + 1);
		buffer << (*f -> arguments[i] -> type) -> str(f -> arguments[i] -> name) << ";\n";
	}
	// de-serialize the arguments
	buffer << emit_indent(indent + 1);
	buffer << "__ocall_wrapper_" << f -> name << "_table_t function_reference = __ocall_wrapper_" << f -> name << "_as_root((void *) call_args);\n";
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		buffer << emit_indent(indent + 1);
		buffer << (*f -> arguments[i] -> type) -> flatcc_type() << " " <<
			"__flatcc_reference_" << f -> arguments[i] -> name << " = " <<
			"__ocall_wrapper_" << f -> name << "_" << f -> arguments[i] -> name << "(function_reference);\n";
		buffer << emit_deserialize(f -> arguments[i] -> name,
			std::string("__flatcc_reference_") + f -> arguments[i] -> name,
			*f -> arguments[i] -> type, indent + 1);
	}
	// call the function
	buffer << emit_indent(indent + 1);
	buffer << (*f -> return_type) -> str("ret_val") << ";\n";
	buffer << emit_indent(indent + 1);
	buffer << "ret_val = " << f -> name << "(";
	for (size_t i = 0; i < f -> arguments.size(); ++i) {
		buffer << f -> arguments[i] -> name;
		if (i + 1 < f -> arguments.size()) buffer << ", ";
	}
	buffer << ");\n";
	// initiate a flatcc buffer
	buffer << emit_indent(indent + 1); 
	buffer << "flatcc_builder_t builder;\n";
	buffer << emit_indent(indent + 1); 
	buffer << "flatcc_builder_init(&builder);\n";
	// setup return data
	buffer << emit_indent(indent + 1);
	buffer << (*f -> return_type) -> flatcc_reference() << " " <<
		"__flatcc_reference_ret_val;\n";
	buffer << emit_serialize("ret_val",
		std::string("__flatcc_reference_ret_val"),
		*f -> return_type, indent + 1);
	buffer << emit_indent(indent + 1); 
	buffer << "__ocall_wrapper_" << f -> name << "_start_as_root(&builder);\n";
	buffer << emit_indent(indent + 1);
	buffer << "__ocall_wrapper_" << f -> name << "___return_value_add(&builder, __flatcc_reference_ret_val);\n";
	buffer << emit_indent(indent + 1); 
	buffer << "__ocall_wrapper_" << f -> name << "_end_as_root(&builder);\n";
	// get the table
	buffer << emit_indent(indent + 1); 
	buffer << "void* buf;\n";
	buffer << emit_indent(indent + 1); 
	buffer << "size_t size;\n";
	buffer << emit_indent(indent + 1); 
	buffer << "buf = (void *) flatcc_builder_finalize_buffer(&builder, &size);\n";
	buffer << emit_indent(indent + 1);
	buffer << "uintptr_t data_section = edge_call_data_ptr();\n";
	buffer << emit_indent(indent + 1);
	buffer << "memcpy((void *) data_section, buf, size);\n"; 
	buffer << emit_indent(indent + 1);
	buffer << "if (edge_call_setup_ret(edge_call, (void*) data_section, 1024)) {\n";
	buffer << emit_indent(indent + 2);
    buffer << "edge_call -> return_data.call_status = CALL_STATUS_BAD_PTR;\n";
	buffer << emit_indent(indent + 1);
	buffer << "} else {\n";
	buffer << emit_indent(indent + 2);
    buffer << "edge_call -> return_data.call_status = CALL_STATUS_OK;\n";
	buffer << emit_indent(indent + 1);
	buffer << "}\n";
	buffer << emit_indent(indent + 1);
	buffer << "return;\n";
	buffer << emit_indent(indent);
	buffer << "}\n";
	return buffer.str();
}

