#include "parser.h"

namespace std {

	std::string to_string(CXString cstr) {
		std::string str(clang_getCString(cstr));
		clang_disposeString(cstr);
		return str;
	}

}

std::string type_information::str(const std::string& name) const {
	return "";
}

std::string type_information::flatcc_prefix() const {
	return "";
}

std::string type_information::flatcc_type() const {
	return "";
}

std::string type_information::flatcc_reference() const {
	return "";
}

std::string primitive_type_information::str(const std::string& name) const {
	return __str + (name.empty() ? std::string() : std::string(" ") + name);
}

std::string primitive_type_information::flatcc_prefix() const {
	return "flatbuffers_" + __flatcc_type;
}

std::string primitive_type_information::flatcc_type() const {
	return str();
}

std::string primitive_type_information::flatcc_reference() const {
	return str();
}

std::string array_information::str(const std::string& name) const {
	std::string name_lengths = name;
	for (size_t i = lengths.size(); i > 0; --i)
		name_lengths += std::string("[") + lengths[i - 1] + "]";
	return (*element_type) -> str(name_lengths);
}

std::string array_information::flatcc_prefix() const {
	return (*element_type) -> flatcc_prefix() + "_vec";
}

std::string array_information::flatcc_type() const {
	return flatcc_prefix() + "_t";
}

std::string array_information::flatcc_reference() const {
	return flatcc_prefix() + "_ref_t";
}

std::string struct_information::str(const std::string& name) const {
	return this -> name + (name.empty() ? std::string() : std::string(" ") + name);
}

std::string struct_information::flatcc_prefix() const {
	return str(); 
}

std::string struct_information::flatcc_type() const {
	return flatcc_prefix() + "_table_t";
}

std::string struct_information::flatcc_reference() const {
	return flatcc_prefix() + "_ref_t";
}

std::string pointer_information::str(const std::string& name) const {
	return (*type) -> str(std::string("*") + name);
}

std::string pointer_information::flatcc_prefix() const {
	return std::string("__pointer_") + (*type) -> str();
}

std::string pointer_information::flatcc_type() const {
	return flatcc_prefix() + "_table_t";
}

std::string pointer_information::flatcc_reference() const {
	return flatcc_prefix() + "_ref_t";
}

std::map<std::string, std::shared_ptr<type_information>> type_lookup;
std::vector<std::shared_ptr<type_information>> type_pool;
std::map<std::string, std::shared_ptr<function_information>> function_lookup;
std::vector<std::shared_ptr<struct_information>> struct_pool;

bool type_indicator::operator !() const {
	return is_null;
}

std::shared_ptr<type_information> type_indicator::operator *() const {
	if (str.empty()) return type_pool[p];
	return type_lookup[str];
}

void initialize_primitive_types() {
	type_lookup["int"] = std::shared_ptr<type_information>(
		new primitive_type_information("int", "int64"));
	type_lookup["signed char"] = std::shared_ptr<type_information>(
		new primitive_type_information("signed char", "int8"));
	// TODO: add more primitive types
}

type_indicator parse_type(CXType type) {
	// pointer detection
	if (type.kind == CXType_Pointer) {
		type_pool.push_back(std::shared_ptr<pointer_information>(
			new pointer_information(parse_type(clang_getPointeeType(type)))));
		return type_indicator(type_pool.size() - 1);
	}
	// array detection
	if (clang_getArraySize(type) >= 0) {
		type_indicator indicator = parse_type(clang_getArrayElementType(type));
		if (typeid(**indicator) == typeid(array_information)) {
			std::dynamic_pointer_cast<array_information>(*indicator) -> lengths.insert(
				std::dynamic_pointer_cast<array_information>(*indicator) -> lengths.begin(),
				std::to_string(clang_getArraySize(type)));
			return indicator;
		}
		type_pool.push_back(std::shared_ptr<array_information>(
			new array_information(std::vector<std::string>(1, std::to_string(clang_getArraySize(type))),
				indicator)));
		return type_indicator(type_pool.size() - 1);
	}
	type_indicator parsed = type_indicator(std::to_string(clang_getTypeSpelling(type)));
	return parsed;
}

std::shared_ptr<element_information> parse_element(CXCursor cursor) {
	std::shared_ptr<element_information> parsed(new element_information());
	// name
	parsed -> name = std::to_string(clang_getCursorSpelling(cursor));
	// type
	parsed -> type = parse_type(clang_getCursorType(cursor));
	return parsed;
}

size_t global_function_index = 1;
std::shared_ptr<function_information> parse_function(CXCursor cursor) {
	std::shared_ptr<function_information> parsed(new function_information());
	// name
	parsed -> name = std::to_string(clang_getCursorSpelling(cursor));
	// return type
	CXType function_type = clang_getCursorType(cursor);
	parsed -> return_type = parse_type(clang_getResultType(function_type));
	// arguments
	clang_visitChildren(cursor, [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult{
		if (clang_getCursorKind(cursor) == CXCursor_ParmDecl) {
			std::shared_ptr<element_information> arg = parse_element(cursor);
			((std::vector<std::shared_ptr<element_information>> *) client_data) -> push_back(arg);
		}
		return CXChildVisit_Continue;
	}, &parsed -> arguments);
	parsed -> index = global_function_index++;
	return parsed;
}

std::shared_ptr<struct_information> parse_struct(CXCursor cursor) {
	std::shared_ptr<struct_information> parsed(new struct_information());
	// name
	parsed -> name = std::to_string(clang_getCursorSpelling(cursor));
	// members
	clang_visitChildren(cursor, [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult{
		if (clang_getCursorKind(cursor) == CXCursor_VarDecl) {
			std::shared_ptr<element_information> arg = parse_element(cursor);
			((std::vector<std::shared_ptr<element_information>> *) client_data) -> push_back(arg);
		}
		return CXChildVisit_Continue;
	}, &parsed -> members);
	return parsed;
}

CXChildVisitResult parse_statement(CXCursor cursor, CXCursor parent, CXClientData client_data) {
	if (clang_getCursorKind(cursor) == CXCursor_StructDecl) {
		std::shared_ptr<struct_information> str = parse_struct(cursor);
		struct_pool.push_back(str);
		type_lookup[str -> name] = str;
	} else if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
		std::shared_ptr<function_information> function = parse_function(cursor);
		function_lookup[function -> name] = function;
	}
	return CXChildVisit_Continue;
}

