#ifndef __PARSER_H
#define __PARSER_H

#include "tokenizer.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

// type information
struct type_information {
	// build a type declaration with the name specified
	virtual std::string str(const std::string& name = "") const;
	// serialized verion of the type, useful only in structs and arrays
	virtual std::string serialized_type(const std::string& name = "") const;
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
	virtual std::string str(const std::string& name = "") const;
	virtual std::string serialized_type(const std::string& name = "") const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
	array_information(const std::vector<std::string>& lengths,
		type_indicator element_type) :
		lengths(lengths), element_type(element_type) {}
};

// an element, consisting of a type and a name
struct element_information {
	type_indicator type;
	std::string name;
};

// structs
struct struct_information : type_information {
	std::string name;
	std::vector<std::shared_ptr<element_information>> members;
	virtual std::string str(const std::string& name = "") const;
	virtual std::string serialized_type(const std::string& name = "") const;
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

// preparsed token information
struct token {
	raw_token raw;
	size_t parenthesis_type;	// 0 for non-parenthesis, 1 for left, 2 for right
	size_t correspondant;		// the corresponding parenthesis, if any
	size_t statement_end;		// the end of the current statement
};

// lookup tables
extern std::map<std::string, std::shared_ptr<type_information>> type_lookup;
extern std::map<std::string, std::shared_ptr<function_information>> function_lookup;
extern std::vector<std::shared_ptr<struct_information>> struct_pool;

// primitive type initialization
void initialize_primitive_types();

// preparse
std::vector<token> preparse(const std::vector<raw_token>& raw_tokens);

// parse different entities
type_indicator parse_type(
	const std::vector<token>& tokens, size_t begin, size_t end,
	std::vector<std::string> array_length = std::vector<std::string>());
std::shared_ptr<element_information> parse_element(
	const std::vector<token>& tokens, size_t begin, size_t end,
	std::vector<std::string> array_length = std::vector<std::string>());
std::shared_ptr<function_information> parse_function(const std::vector<token>& tokens, size_t begin, size_t end);
std::shared_ptr<struct_information> parse_struct(const std::vector<token>& tokens, size_t begin, size_t end);
int parse_statement(const std::vector<token>& tokens, size_t begin, size_t end);

#endif

