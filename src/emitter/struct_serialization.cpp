#include "index.h"

// the variable name of flatcc reference
std::string flatcc_reference_var(size_t indent, std::string member_name) {
	return emit("__$(INDENT)_$(MEMBER_NAME)", {
		{"$(INDENT)", std::to_string(indent)},
		{"$(MEMBER_NAME)", member_name}
	});
}

std::string emit_serialize_member(const std::string& name, std::shared_ptr<element_information> member,
	size_t indent) {
	return emit(
		"$(FLATCC_REFERENCE) $(FLATCC_REFERENCE_VAR);\n"
		"$(SERIALIZE_MEMBER)\n", {
			// the reference used in flatcc
			{"$(FLATCC_REFERENCE)", (*member -> type) -> flatcc_reference()},
			{"$(FLATCC_REFERENCE_VAR)", flatcc_reference_var(indent, member -> name)},
			// the serialization of the member
			{"$(SERIALIZE_MEMBER)", emit_serialize(
				name + "." + member -> name, flatcc_reference_var(indent, member -> name),
				*member -> type, indent)}
		}, indent);
}

std::string emit_deserialize_member(const std::string& name, const std::string& serialized_name, std::shared_ptr<struct_information> type,
	std::shared_ptr<element_information> member, size_t indent) {
	return emit("$(EMIT)\n", {{"$(EMIT)", emit_deserialize(name + "." + member -> name,
		type -> name + "_" + member -> name + "(" + serialized_name + ")",
		*member -> type, indent)}}, indent);
}

std::string emit_serialize_struct(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<struct_information> type, size_t indent) {
	// grab the sizes of vla members
	std::queue<std::string> sizes;
	for (std::shared_ptr<element_information>& member : type -> members) {
		if (member -> attr_flag & ATTRIBUTE_SIZE) {
			sizes.push(name + "." + member -> name);
		}
	}
	return emit(
		"$(STRUCT_NAME)_start(&builder);\n"
		"$(SERIALIZE_NON_VLA_MEMBERS)"
		"$(SERIALIZE_VLA_MEMBERS)"
		"$(BUILD_STRUCT)"
		"$(SERIALIZED_NAME) = $(STRUCT_NAME)_end(&builder);" , {
			// the name of the struct
			{"$(STRUCT_NAME)", type -> name},
			// the serialization of non-vla members
			{"$(SERIALIZE_NON_VLA_MEMBERS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& member : type -> members) {
					if (!(member -> attr_flag & ATTRIBUTE_VLA)) {
						ret += emit_serialize_member(name, member, indent);
					}
				}
				return ret;
			}()},
			// the serialization of vla members
			{"$(SERIALIZE_VLA_MEMBERS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& member : type -> members) {
					if (member -> attr_flag & ATTRIBUTE_VLA) {
						if (member -> attr_flag & ATTRIBUTE_STR) {
							std::string var_name = emit("__$(INDENT)_$(MEMBER_NAME)_keyedge_size", {
								{"$(INDENT)", std::to_string(indent)},
								{"$(MEMBER_NAME)", member -> name}
							});
							ret += emit("size_t $(VAR_NAME) = strlen($(NAME).$(MEMBER_NAME)) + 1;\n", {
								{"$(VAR_NAME)", var_name},
								{"$(NAME)", name},
								{"$(MEMBER_NAME)", member -> name}
							}, indent);
							std::dynamic_pointer_cast<array_information>(*member -> type) -> lengths =
								std::vector <std::string>(1, var_name);
						} else {
							std::dynamic_pointer_cast<array_information>(*member -> type) -> lengths =
								std::vector <std::string>(1, sizes.front());
							sizes.pop();
						}
						ret += emit_serialize_member(name, member, indent);
					}
				}
				return ret;
			}()},
			// build the struct
			{"$(BUILD_STRUCT)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& member : type -> members) {
					ret += emit("$(STRUCT_NAME)_$(MEMBER_NAME)_add(&builder, $(FLATCC_REFERENCE_VAR));\n", {
						{"$(STRUCT_NAME)", type -> name},
						{"$(MEMBER_NAME)", member -> name},
						{"$(FLATCC_REFERENCE_VAR)", flatcc_reference_var(indent, member -> name)}
					}, indent);
				}
				return ret;
			}()},
			// the serialized name of the instance of the struct
			{"$(SERIALIZED_NAME)", serialized_name}
		}, indent);
}

std::string emit_deserialize_struct(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<struct_information> type, size_t indent) {
	// grab the sizes of vla members
	std::queue<std::string> sizes;
	for (std::shared_ptr<element_information>& member : type -> members) {
		if (member -> attr_flag & ATTRIBUTE_SIZE) {
			sizes.push(name + "." + member -> name);
		}
	}
	return emit(
		"$(DESERIALIZE_NON_VLA_MEMBERS)"
		"$(DESERIALIZE_VLA_MEMBERS)", {
			// the deserialization of non-vla members
			{"$(DESERIALIZE_NON_VLA_MEMBERS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& member : type -> members) {
					if (!(member -> attr_flag & ATTRIBUTE_VLA)) {
						ret += emit_deserialize_member(name, serialized_name, type, member, indent);
					}
				}
				return ret;
			}()},
			// the deserialization of vla members
			{"$(DESERIALIZE_VLA_MEMBERS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& member : type -> members) {
					if (member -> attr_flag & ATTRIBUTE_VLA) {
						if (member -> attr_flag & ATTRIBUTE_STR) {
							std::string var_name = emit("__$(INDENT)_$(MEMBER_NAME)_keyedge_size", {
								{"$(INDENT)", std::to_string(indent)},
								{"$(MEMBER_NAME)", member -> name}
							});
							ret += emit("size_t $(VAR_NAME) = $(MEMBER_FLATCC_PREFIX)_len($(STRUCT_NAME)_$(MEMBER_NAME)($(SERIALIZED_NAME)));\n", {
								{"$(VAR_NAME)", var_name},
								{"$(MEMBER_FLATCC_PREFIX)", (*member -> type) -> flatcc_prefix()},
								{"$(STRUCT_NAME)", type -> name},
								{"$(MEMBER_NAME)", member -> name},
								{"$(SERIALIZED_NAME)", serialized_name}
							}, indent);
							std::dynamic_pointer_cast<array_information>(*member -> type) -> lengths =
								std::vector <std::string>(1, var_name);
						} else {
							std::dynamic_pointer_cast<array_information>(*member -> type) -> lengths =
								std::vector <std::string>(1, sizes.front());
							sizes.pop();
						}
						ret += emit_deserialize_member(name, serialized_name, type, member, indent);
					}
				}
				return ret;
			}()}
		}, indent);
}

