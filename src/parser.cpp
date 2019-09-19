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

std::string type_information::fbs_type() const {
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

std::string primitive_type_information::fbs_type() const {
	return __flatcc_type;
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
	if (is_vla) return (*element_type) -> str(std::string("*") + name);
	std::string name_lengths = name;
	for (size_t i = lengths.size(); i > 0; --i)
		name_lengths += std::string("[") + lengths[i - 1] + "]";
	return (*element_type) -> str(name_lengths);
}

std::string array_information::fbs_type() const {
	return std::string("[") + (*element_type) -> fbs_type() + std::string("]");
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

std::string struct_information::fbs_type() const {
	return str();
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

std::string pointer_information::fbs_type() const {
	return std::string("__pointer_") + (*type) -> flatcc_prefix();
}

std::string pointer_information::flatcc_prefix() const {
	return std::string("__pointer_") + (*type) -> flatcc_prefix();
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
std::map<std::string, std::shared_ptr<pointer_information>> pointer_lookup;

bool type_indicator::operator !() const {
	return is_null;
}

std::shared_ptr<type_information> type_indicator::operator *() const {
	if (str.empty()) return type_pool[p];
	return type_lookup[str];
}

void initialize_primitive_types() {
	type_lookup["void"] = std::shared_ptr<type_information>(
		new primitive_type_information("void", "int8"));
	type_lookup["char"] = std::shared_ptr<type_information>(
		new primitive_type_information("char", "int8"));
	type_lookup["signed char"] = std::shared_ptr<type_information>(
		new primitive_type_information("signed char", "int8"));
	type_lookup["unsigned char"] = std::shared_ptr<type_information>(
		new primitive_type_information("unsigned char", "uint8"));
	type_lookup["short"] = std::shared_ptr<type_information>(
		new primitive_type_information("short", "int16"));
	type_lookup["signed short"] = std::shared_ptr<type_information>(
		new primitive_type_information("signed short", "int16"));
	type_lookup["unsigned short"] = std::shared_ptr<type_information>(
		new primitive_type_information("unsigned short", "uint16"));
	type_lookup["int"] = std::shared_ptr<type_information>(
		new primitive_type_information("int", "int32"));
	type_lookup["signed int"] = std::shared_ptr<type_information>(
		new primitive_type_information("signed int", "int32"));
	type_lookup["unsigned int"] = std::shared_ptr<type_information>(
		new primitive_type_information("unsigned int", "uint32"));
	type_lookup["long"] = std::shared_ptr<type_information>(
		new primitive_type_information("long", "int32"));
	type_lookup["signed long"] = std::shared_ptr<type_information>(
		new primitive_type_information("signed long", "int32"));
	type_lookup["unsigned long"] = std::shared_ptr<type_information>(
		new primitive_type_information("unsigned long", "uint32"));
	// TODO: add more primitive types
}

type_indicator parse_type(CXType type) {
	// pointer detection
	if (type.kind == CXType_Pointer) {
		type_pool.push_back(std::shared_ptr<pointer_information>(
			new pointer_information(parse_type(clang_getPointeeType(type)))));
		pointer_lookup[type_pool.back() -> fbs_type()] = std::dynamic_pointer_cast<pointer_information>(type_pool.back());
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
	// attributes
	parsed -> attr_flag = 0;
	clang_visitChildren(cursor, [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult{
		if (clang_getCursorKind(cursor) == CXCursor_AnnotateAttr) {
			if (std::to_string(clang_getCursorSpelling(cursor)) == ATTRIBUTE_VLA_STR) {
				*(int *)client_data |= ATTRIBUTE_VLA;
			} else if (std::to_string(clang_getCursorSpelling(cursor)) == ATTRIBUTE_SIZE_STR) {
				*(int *)client_data |= ATTRIBUTE_SIZE;
			} else if (std::to_string(clang_getCursorSpelling(cursor)) == ATTRIBUTE_STR_STR) {
				*(int *)client_data |= ATTRIBUTE_STR;
			}
		}
		return CXChildVisit_Continue;
	}, &parsed -> attr_flag);
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
	// handling vectors
	std::vector <size_t> vectors;
	for (size_t i = 0; i < parsed -> arguments.size(); ++i) {
		if (parsed -> arguments[i] -> attr_flag & ATTRIBUTE_VLA) {
			vectors.push_back(i);
		}
	}
	for (size_t i = 0; i < vectors.size(); ++i) {
		type_pool[parsed -> arguments[vectors[i]] -> type.p] = std::shared_ptr<array_information>(
			new array_information(std::vector<std::string>(),
				std::dynamic_pointer_cast<pointer_information>(*parsed -> arguments[vectors[i]] -> type) -> type, true));
	}
	// handling indexes
	parsed -> index = global_function_index++;
	return parsed;
}

std::shared_ptr<struct_information> parse_struct(CXCursor cursor) {
	std::shared_ptr<struct_information> parsed(new struct_information());
	// name
	parsed -> name = std::to_string(clang_getCursorSpelling(cursor));
	// members
	clang_visitChildren(cursor, [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult{
		if (clang_getCursorKind(cursor) == CXCursor_FieldDecl) {
			std::shared_ptr<element_information> arg = parse_element(cursor);
			((std::vector<std::shared_ptr<element_information>> *) client_data) -> push_back(arg);
		}
		return CXChildVisit_Continue;
	}, &parsed -> members);
	// handling vectors
	std::vector <size_t> vectors;
	for (size_t i = 0; i < parsed -> members.size(); ++i) {
		if (parsed -> members[i] -> attr_flag & ATTRIBUTE_VLA) {
			vectors.push_back(i);
		}
	}
	for (size_t i = 0; i < vectors.size(); ++i) {
		type_pool[parsed -> members[vectors[i]] -> type.p] = std::shared_ptr<array_information>(
			new array_information(std::vector<std::string>(),
				std::dynamic_pointer_cast<pointer_information>(*parsed -> members[vectors[i]] -> type) -> type, true));
	}
	return parsed;
}

CXChildVisitResult parse_statement(CXCursor cursor, CXCursor parent, CXClientData client_data) {
	if (clang_getCursorKind(cursor) == CXCursor_StructDecl) {
		std::shared_ptr<struct_information> str = parse_struct(cursor);
		struct_pool.push_back(str);
		type_lookup[str -> name] = str;
	} else if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
		if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor))) {
			std::shared_ptr<function_information> function = parse_function(cursor);
			function_lookup[function -> name] = function;
		}
	}
	return CXChildVisit_Continue;
}

