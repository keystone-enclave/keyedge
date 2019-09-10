#include "parser.h"

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
	return flatcc_prefix() + "_t";
}

std::string struct_information::flatcc_reference() const {
	return flatcc_prefix() + "_ref_t";
}

std::string pointer_information::str(const std::string& name) const {
	return (*type) -> str(std::string("*") + name);
}

std::string pointer_information::flatcc_prefix() const {
	return (*type) -> flatcc_prefix();
}

std::string pointer_information::flatcc_type() const {
	return (*type) -> flatcc_type();
}

std::string pointer_information::flatcc_reference() const {
	return (*type) -> flatcc_reference();
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
	type_lookup["char"] = std::shared_ptr<type_information>(
		new primitive_type_information("signed char", "int8"));
	// TODO: add more primitive types
}

std::vector<token> preparse(const std::vector<raw_token>& raw_tokens) {
	std::vector<token> tokens;
	tokens.resize(raw_tokens.size());
	// copy raw tokens
	for (size_t i = 0; i < tokens.size(); ++i)
		tokens[i].raw = raw_tokens[i];
	// find parentheses
	const std::string parentheses = "{}[]()";
	std::vector<std::pair<size_t, size_t>> stack;	// the location and the type of the parenthesis
	for (size_t i = 0; i < tokens.size(); ++i) {
		tokens[i].parenthesis_type = 0;	// default to non-parenthesis
		for (size_t j = 0; j < parentheses.size(); j += 2) {
			if (tokens[i].raw.token[0] == parentheses[j]) {	// left parenthesis
				tokens[i].parenthesis_type = 1;	// left
				stack.push_back(std::make_pair(i, j));
			}
		}
		for (size_t j = 1; j < parentheses.size(); j += 2) {
			if (tokens[i].raw.token[0] == parentheses[j]) {	// right parenthesis
				tokens[i].parenthesis_type = 2;	// right
				if (stack.empty() || stack.back().second != j - 1) {	// mismatch
					throw std::to_string(tokens[i].raw.row) + ", " +
						std::to_string(tokens[i].raw.col) + ": " +
						"Unexpected \"" + tokens[i].raw.token[0] + "\".";
				}
				// Everything is good, so put it in.
				tokens[stack.back().first].correspondant = i;
				tokens[i].correspondant = stack.back().first;
				stack.pop_back();
			}
		}
	}
	if (!stack.empty()) {	// mismatch
		size_t p = stack.back().first;
		throw std::to_string(tokens[p].raw.row) + ", " +
			std::to_string(tokens[p].raw.col) + ": " +
			"Unmatched \"" + tokens[p].raw.token[0] + "\".";
	}
	// find statement ends
	for (size_t i = tokens.size(); i > 0; --i) {
		if (tokens[i - 1].raw.token == ";") {				// on statement end
			tokens[i - 1].statement_end = i - 1;
		} else if (tokens[i - 1].parenthesis_type == 1) {	// on left parenthesis
			tokens[i - 1].statement_end = tokens[tokens[i - 1].correspondant].statement_end;
		} else {
			if (i >= tokens.size()) {						// unfinished statement
				throw "Expected \";\" at the end of file.";
			}
			tokens[i - 1].statement_end = tokens[i].statement_end;
		}
	}
	return tokens;
}

type_indicator parse_type(
	const std::vector<token>& tokens, size_t begin, size_t end,
	std::vector<std::string> array_length) {
	// pointer detection
	if (tokens[end - 1].raw.token == "*") {
		type_pool.push_back(std::shared_ptr<pointer_information>(
			new pointer_information(parse_type(tokens, begin, end - 1, array_length))));
		return type_indicator(type_pool.size() - 1);
	}
	// array detection
	if (tokens[end - 1].raw.token == "]") {
		std::string length;
		for (size_t i = tokens[end - 1].correspondant + 1; i < end - 1; ++i) {
			length += tokens[i].raw.token;
			if (i + 1 < end - 1) length += " ";
		}
		array_length.push_back(length);
		return parse_type(tokens, begin, tokens[end - 1].correspondant, array_length);
	}
	type_indicator parsed = type_indicator(tokens[begin].raw.token);
	if (!array_length.empty()) {
		type_pool.push_back(std::shared_ptr<array_information>(
			new array_information(array_length, parsed)));
		return type_indicator(type_pool.size() - 1);
	}
	return parsed;
}

std::shared_ptr<element_information> parse_element(
	const std::vector<token>& tokens, size_t begin, size_t end,
	std::vector<std::string> array_length) {
	// array detection
	if (tokens[end - 1].raw.token == "]") {
		std::string length;
		for (size_t i = tokens[end - 1].correspondant + 1; i < end - 1; ++i) {
			length += tokens[i].raw.token;
			if (i + 1 < end - 1) length += " ";
		}
		array_length.push_back(length);
		return parse_element(tokens, begin, tokens[end - 1].correspondant, array_length);
	}
	std::shared_ptr<element_information> parsed(new element_information());
	parsed -> name = tokens[end - 1].raw.token;
	parsed -> type = parse_type(tokens, begin, end - 1);
	if (!parsed -> type) return std::shared_ptr<element_information>();
	if (!array_length.empty()) {
		type_pool.push_back(std::shared_ptr<array_information>(
			new array_information(array_length, parsed -> type)));
		parsed -> type = type_indicator(type_pool.size() - 1);
	}
	return parsed;
}

size_t global_function_index = 1;
std::shared_ptr<function_information> parse_function(
	const std::vector<token>& tokens, size_t begin, size_t end) {
	size_t rparenthesis;
	for (rparenthesis = end; rparenthesis > begin &&
		tokens[rparenthesis - 1].raw.token != ")"; --rparenthesis); 
	if (rparenthesis == begin) return std::shared_ptr<function_information>();	// ill-formed function
	size_t lparenthesis = tokens[rparenthesis - 1].correspondant;
	std::shared_ptr<function_information> parsed(new function_information());
	// name
	parsed -> name = tokens[lparenthesis - 1].raw.token;
	// return type
	parsed -> return_type = parse_type(tokens, begin, lparenthesis - 1);
	if (!parsed -> return_type) return std::shared_ptr<function_information>();
	// arguments
	for (size_t i = lparenthesis + 1, j = lparenthesis + 1; j < rparenthesis; ++j) {
		if (tokens[j].raw.token == "," || tokens[j].raw.token == ")") {
			std::shared_ptr<element_information> arg = parse_element(tokens, i, j);
			if (!arg) return std::shared_ptr<function_information>();
			parsed -> arguments.push_back(arg);
			i = j + 1;
		}
	}
	parsed -> index = global_function_index++;
	return parsed;
}

std::shared_ptr<struct_information> parse_struct(const std::vector<token>& tokens, size_t begin, size_t end) {
	std::shared_ptr<struct_information> parsed(new struct_information());
	size_t rparenthesis;
	for (rparenthesis = end; rparenthesis > begin &&
		tokens[rparenthesis - 1].raw.token != "}"; --rparenthesis); 
	size_t lparenthesis = tokens[rparenthesis - 1].correspondant;
	// name
	parsed -> name = tokens[lparenthesis - 1].raw.token;
	// elements
	for (size_t i = lparenthesis + 1; i < rparenthesis - 1; i = tokens[i].statement_end + 1) {
		parsed -> members.push_back(parse_element(tokens, i, tokens[i].statement_end));
	}
	return parsed;
}

int parse_statement(
	const std::vector<token>& tokens, size_t begin, size_t end) {
	if (tokens[begin].raw.token == "struct") {
		std::shared_ptr<struct_information> str = parse_struct(tokens, begin, end);
		struct_pool.push_back(str);
		type_lookup[str -> name] = str;
		return 0;
	}
	std::shared_ptr<function_information> function = parse_function(tokens, begin, end);
	function_lookup[function -> name] = function;
	return 0;
}

