#include "index.h"

std::string emit_serialize_pointer(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<pointer_information> type, size_t indent) {
	std::string data_name = std::string("__keyedge_pointer_data_") + std::to_string(indent);
	return emit(
		"if ($(NAME)) {\n"
		"\t$(POINTEE_FLATCC_REFERENCE) $(DATA_NAME);\n"
		"\t$(SERIALIZE_POINTEE)\n"
		"\t$(SERIALIZED_NAME) = $(FLATCC_PREFIX)_create(&builder, 0, $(DATA_NAME));\n"
		"} else {\n"
		"\t$(FLATCC_PREFIX)_start(&builder);\n"
		"\t$(FLATCC_PREFIX)___is_null_add(&builder, 1);\n"
		"\t$(SERIALIZED_NAME) = $(FLATCC_PREFIX)_end(&builder);\n"
		"}", {
			{"$(NAME)", name},
			{"$(POINTEE_FLATCC_REFERENCE)", (*type -> type) -> flatcc_reference()},
			{"$(DATA_NAME)", data_name},
			{"$(SERIALIZE_POINTEE)", emit_serialize(emit("(*$(NAME))", {{"$(NAME)", name}}), data_name,
				*type -> type, indent + 1)},
			{"$(SERIALIZED_NAME)", serialized_name},
			{"$(FLATCC_PREFIX)", type -> flatcc_prefix()}
		}, indent);
}

std::string emit_deserialize_pointer(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<pointer_information> type, size_t indent) {
	return emit(
		"if ($(FLATCC_PREFIX)___is_null($(SERIALIZED_NAME))) {\n"
		"\t$(NAME) = 0;\n"
		"} else {\n"
		"\t$(NAME) = ($(TYPE)) malloc(sizeof($(POINTEE_TYPE)));\n"
		"\t$(DESERIALIZE_POINTEE)\n"
		"}", {
		{"$(FLATCC_PREFIX)", type -> flatcc_prefix()},
		{"$(SERIALIZED_NAME)", serialized_name},
		{"$(NAME)", name},
		{"$(TYPE)", type -> str()},
		{"$(POINTEE_TYPE)", (*type -> type) -> str()},
		{"$(DESERIALIZE_POINTEE)", emit_deserialize(emit("(*$(NAME))", {{"$(NAME)", name}}),
			emit("$(FLATCC_PREFIX)___data($(SERIALIZED_NAME))", {{"$(FLATCC_PREFIX)", type -> flatcc_prefix()},
			{"$(SERIALIZED_NAME)", serialized_name}}), *type -> type, indent + 1)}
		}, indent);
}

