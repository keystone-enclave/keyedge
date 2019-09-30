#include "index.h"

std::string emit_function_common(std::shared_ptr<function_information> f, size_t indent = 0) {
	return emit("const unsigned long __function_$(NAME) = $(INDEX);", {
		{"$(NAME)", f -> name},
		{"$(INDEX)", std::to_string(f -> index)}
	}, indent);
}

std::string emit_common(size_t indent) {
	std::string ret;
	for (auto& function_pair : function_lookup) {
		ret += emit("$(EMIT)\n", {{"$(EMIT)", emit_function_common(function_pair.second, indent)}}, indent);
	}
	return ret;
}

