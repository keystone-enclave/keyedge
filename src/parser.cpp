#include "index.h"

namespace std {

	std::string to_string(CXString cstr) {
		std::string str(clang_getCString(cstr));
		clang_disposeString(cstr);
		return str;
	}

}

void add_primitive_type(const std::string& type, const std::string& flatcc_type) {
	type_lookup[type] = std::shared_ptr<type_information>(
		new primitive_type_information(type, flatcc_type));
	type_lookup[std::string("const ") + type] = std::shared_ptr<type_information>(
		new primitive_type_information(type, flatcc_type));
}

void initialize_primitive_types() {
	add_primitive_type("void", "int8");
	add_primitive_type("char", "int8");
	add_primitive_type("signed char", "int8");
	add_primitive_type("unsigned char", "uint8");
	add_primitive_type("short", "int16");
	add_primitive_type("signed short", "int16");
	add_primitive_type("unsigned short", "uint16");
	add_primitive_type("int", "int32");
	add_primitive_type("signed int", "int32");
	add_primitive_type("unsigned int", "uint32");
	add_primitive_type("long", "int32");
	add_primitive_type("signed long", "int32");
	add_primitive_type("unsigned long", "uint32");
	add_primitive_type("long long", "int64");
	add_primitive_type("signed long long", "int64");
	add_primitive_type("unsigned long long", "uint64");
}

type_indicator parse_type(CXType type) {
	type = clang_getCanonicalType(type);
	// pointer detection
	if (type.kind == CXType_Pointer) {
		type_indicator pointee = parse_type(clang_getPointeeType(type));
		type_pool.push_back(std::shared_ptr<pointer_information>(
			new pointer_information(pointee)));
		pointer_pool.push_back(std::dynamic_pointer_cast<pointer_information>(type_pool.back()));
		return type_indicator(type_pool.size() - 1, pointee.is_const);
	}
	// array detection
	if (clang_getArraySize(type) >= 0) {
		type_indicator indicator = parse_type(clang_getArrayElementType(type));
		if (typeid(**indicator) == typeid(array_information)) {
			std::dynamic_pointer_cast<array_information>(*indicator) -> lengths.push_back(
				std::to_string(clang_getArraySize(type)));
			return indicator;
		}
		type_pool.push_back(std::shared_ptr<array_information>(
			new array_information(std::vector<std::string>(1, std::to_string(clang_getArraySize(type))),
				indicator)));
		return type_indicator(type_pool.size() - 1, indicator.is_const);
	}
	type_indicator parsed = type_indicator(std::to_string(clang_getTypeSpelling(type)));
	parsed.is_const = clang_isConstQualifiedType(type);
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
		type_lookup[std::string("struct ") + str -> name] = str;
		type_lookup[std::string("const struct ") + str -> name] = str;
	} else if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
		if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor))) {
			std::shared_ptr<function_information> function = parse_function(cursor);
			function_lookup[function -> name] = function;
		}
	}
	return CXChildVisit_Continue;
}

