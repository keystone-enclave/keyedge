#include "index.h"

std::string emit_indent(size_t indent) {
	std::stringstream buffer;
	for (size_t i = 0; i < indent; ++i) buffer << "\t";
	return buffer.str();
}

std::string emit(const std::string& str,
	const std::vector<std::pair<std::string, std::string>>& var,
	size_t indent) { 
	std::regex eol("\n");
	std::string ret = std::regex_replace(str, eol, std::string("\n") + emit_indent(indent));
	for (auto& var_pair : var) {
		std::regex rep(var_pair.first);
		ret = std::regex_replace(ret, rep, var_pair.second);
	}
	return ret;
}

