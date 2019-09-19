#ifndef __PARSER_H
#define __PARSER_H

#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "attributes.h"

#include <clang-c/Index.h>

// type information
struct type_information {
	// build a type declaration with the name specified
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
	type_information() {}
	virtual ~type_information() {}
};

// type indicator
struct type_indicator {
	//If it is a primitive type or a struct, use the name.
	std::string str;
	//Otherwise, use the index of type_pool.
	size_t p;
	bool is_null;
	type_indicator() : is_null(true) {}
	type_indicator(const std::string& str) : str(str), is_null(false) {}
	type_indicator(size_t p) : p(p), is_null(false) {}
	bool operator !() const;
	std::shared_ptr<type_information> operator *() const;
};

// primitive types
struct primitive_type_information : type_information {
	std::string __str;
	std::string __flatcc_type;
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
	primitive_type_information(const std::string& str, const std::string& flatcc_type)
		: __str(str), __flatcc_type(flatcc_type) {}
};

// arrays
struct array_information : type_information {
	std::vector<std::string> lengths;
	type_indicator element_type;
	bool is_vla;
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
	array_information(const std::vector<std::string>& lengths,
		type_indicator element_type, bool is_vla = false) :
		lengths(lengths), element_type(element_type), is_vla(is_vla) {}
};

// an element, consisting of a type and a name
struct element_information {
	type_indicator type;
	std::string name;
	int attr_flag;
};

// structs
struct struct_information : type_information {
	std::string name;
	std::vector<std::shared_ptr<element_information>> members;
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
};

// pointers
struct pointer_information : type_information {
	type_indicator type;
	pointer_information(type_indicator type) : type(type) {}
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
};

// function information
struct function_information {
	std::string name;
	size_t index;
	type_indicator return_type;
	std::vector<std::shared_ptr<element_information>> arguments;
};

// lookup tables
extern std::map<std::string, std::shared_ptr<type_information>> type_lookup;
extern std::map<std::string, std::shared_ptr<function_information>> function_lookup;
extern std::vector<std::shared_ptr<struct_information>> struct_pool;
extern std::vector<std::shared_ptr<pointer_information>> pointer_pool;

// primitive type initialization
void initialize_primitive_types();

// statement parse
CXChildVisitResult parse_statement(CXCursor cursor, CXCursor parent, CXClientData client_data);

#endif

