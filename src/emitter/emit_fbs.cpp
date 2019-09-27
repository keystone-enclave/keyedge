#include "index.h"

std::string emit_struct_fbs(std::shared_ptr<struct_information> str, size_t indent = 0) {
	return emit(
		"table $(FBS_TYPE) {\n"
		"$(EMIT_MEMBERS)"
		"}", {
			{"$(FBS_TYPE)", str -> fbs_type()},
			{"$(EMIT_MEMBERS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& member : str -> members) {
					ret += emit("\t$(MEMBER_NAME):$(MEMBER_FBS_TYPE);\n", {
						{"$(MEMBER_NAME)", member -> name},
						{"$(MEMBER_FBS_TYPE)", (*member -> type) -> fbs_type()}
					}, indent);
				}
				return ret;
			}()}
		}, indent);
}

std::string emit_pointer_fbs(std::shared_ptr<pointer_information> p, size_t indent = 0) {
	return emit(
		"table $(FBS_TYPE) {\n"
		"\t__is_null:bool;\n"
		"\t__data:$(POINTEE_FBS_TYPE);\n"
		"}", {
			{"$(FBS_TYPE)", p -> fbs_type()},
			{"$(POINTEE_FBS_TYPE)", (*p -> type) -> fbs_type()}
		}, indent);
}

std::string emit_function_fbs(std::shared_ptr<function_information> f, size_t indent = 0) {
	return emit(
		"table __ocall_wrapper_$(NAME) {\n"
		"$(EMIT_ARGUMENTS)"
		"\t__return_value:$(RETURN_FBS_TYPE);\n"
		"}", {
			{"$(NAME)", f -> name},
			{"$(EMIT_ARGUMENTS)", [&]() -> std::string {
				std::string ret;
				for (size_t i = 0; i < f -> arguments.size(); ++i) {
					ret += emit("\t$(ARGUMENT_NAME):$(ARGUMENT_FBS_TYPE);\n", {
						{"$(ARGUMENT_NAME)", f -> arguments[i] -> name},
						{"$(ARGUMENT_FBS_TYPE)", (*f -> arguments[i] -> type) -> fbs_type()}
					}, indent);
				}
				return ret;
			}()},
			{"$(RETURN_FBS_TYPE)", (*f -> return_type) -> fbs_type()}
		}, indent);
}

std::string emit_fbs(size_t indent) {
	return emit(
		"$(EMIT_STRUCTS)"
		"$(EMIT_POINTERS)"
		"$(EMIT_FUNCTIONS)", {
			{"$(EMIT_STRUCTS)", [&]() -> std::string {
				std::string ret;
				for (auto& str : struct_pool) {
					ret += emit("$(EMIT)\n", {{"$(EMIT)", emit_struct_fbs(str, indent)}},
						indent);
				}
				return ret;
			}()},
			{"$(EMIT_POINTERS)", [&]() -> std::string {
				std::string ret;
				std::set<std::string> known_pointers;
				for (auto& pointer : pointer_pool) {
					if (known_pointers.find(pointer -> fbs_type()) == known_pointers.end()) {
						known_pointers.insert(pointer -> fbs_type());
						ret += emit("$(EMIT)\n", {{"$(EMIT)", emit_pointer_fbs(pointer, indent)}},
							indent);
					}
				}
				return ret;
			}()},
			{"$(EMIT_FUNCTIONS)", [&]() -> std::string {
				std::string ret;
				for (auto& function_pair : function_lookup) {
					ret += emit("$(EMIT)\n", {{"$(EMIT)", emit_function_fbs(function_pair.second, indent)}},
						indent);
				}
				return ret;
			}()}
		}, indent);
}

