#include "index.h"

std::string emit_serialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent) {
	return emit(
		"{\n"
		"\t$(SERIALIZE)\n"
		"}", {
			{"$(SERIALIZE)", [&]() -> std::string {
				if (typeid(*type) == typeid(array_information)) {
					std::shared_ptr<array_information> cast =
						std::dynamic_pointer_cast<array_information>(type);
					return emit_serialize_array(name, serialized_name, cast, indent + 1);
				} else if (typeid(*type) == typeid(struct_information)) {
					std::shared_ptr<struct_information> cast =
						std::dynamic_pointer_cast<struct_information>(type);
					return emit_serialize_struct(name, serialized_name, cast, indent + 1);
				} else if (typeid(*type) == typeid(pointer_information)) {
					std::shared_ptr<pointer_information> cast =
						std::dynamic_pointer_cast<pointer_information>(type);
					return emit_serialize_pointer(name, serialized_name, cast, indent + 1);
				}
				return emit(
					"$(SERIALIZED_NAME) = $(NAME);", {
						// the variable we want to serialize our data to
						{"$(SERIALIZED_NAME)", serialized_name},
						// the variable containing the original data
						{"$(NAME)", name}
					}, indent + 1);
			}()}
		}, indent);
}

std::string emit_deserialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent) {
	return emit(
		"{\n"
		"\t$(DESERIALIZE)\n"
		"}", {
			{"$(DESERIALIZE)", [&]() -> std::string {
				if (typeid(*type) == typeid(array_information)) {
					std::shared_ptr<array_information> cast =
						std::dynamic_pointer_cast<array_information>(type);
					return emit_deserialize_array(name, serialized_name, cast, indent + 1);
				} else if (typeid(*type) == typeid(struct_information)) {
					std::shared_ptr<struct_information> cast =
						std::dynamic_pointer_cast<struct_information>(type);
					return emit_deserialize_struct(name, serialized_name, cast, indent + 1);
				} else if (typeid(*type) == typeid(pointer_information)) {
					std::shared_ptr<pointer_information> cast =
						std::dynamic_pointer_cast<pointer_information>(type);
					return emit_deserialize_pointer(name, serialized_name, cast, indent + 1);
				}
				return emit(
					"$(NAME) = $(SERIALIZED_NAME);", {
						// the variable we want to deserialize our data to
						{"$(NAME)", name},
						// the variable we want to deserialize our data from
						{"$(SERIALIZED_NAME)", serialized_name}
					}, indent + 1);
			}()}
		}, indent);
}

