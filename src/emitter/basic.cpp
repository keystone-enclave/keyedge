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
		std::string rep_string;
		for (char c : var_pair.first) {
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
				rep_string.push_back(c);
			} else {
				rep_string.push_back('\\');
				rep_string.push_back(c);
			}
		}
		std::regex rep(rep_string);
		ret = std::regex_replace(ret, rep, var_pair.second);
	}
	return ret;
}

