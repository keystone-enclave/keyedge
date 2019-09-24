#include "index.h"

std::string emit_serialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent) {
	if (typeid(*type) == typeid(array_information)) {
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
		return emit_serialize_array(name, serialized_name, cast, indent);
	} else if (typeid(*type) == typeid(struct_information)) {
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
		return emit_serialize_struct(name, serialized_name, cast, indent);
	}
	return emit(
		"{\n"
		"\t$(SERIALIZED_NAME) = $(NAME);\n"
		"}", {
			// the variable we want to serialize our data to
			{"$(SERIALIZED_NAME)", serialized_name},
			// the variable containing the original data
			{"$(NAME)", name}
		}, indent);
}

