#include "index.h"

std::string emit_serialize_member(const std::string& name, std::shared_ptr<element_information> member,
	size_t indent) {
	// the variable name of flatcc reference
	std::string flatcc_reference_var = emit("__$(INDENT)_$(MEMBER_NAME)", {
		// the amount of indentation, used as identifier
		{"$(INDENT)", std::to_string(indent)},
		// the name of the member
		{"$(MEMBER_NAME)", member -> name}
	});
	return emit(
		"$(FLATCC_REFERENCE) $(FLATCC_REFERENCE_VAR);\n"
		"$(SERIALIZE_MEMBER)\n", {
			// the reference used in flatcc
			{"$(FLATCC_REFERENCE)", (*member -> type) -> flatcc_reference()},
			{"$(FLATCC_REFERENCE_VAR)", flatcc_reference_var},
			// the serialization of the member
			{"$(SERIALIZE_MEMBER)", emit_serialize(
				name + "." + member -> name, flatcc_reference_var,
				*member -> type, indent)}
		}, indent);
}

std::string emit_serialize_struct(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<struct_information> type, size_t indent) {
	// grab the sizes of vla members
	std::queue<std::string> sizes;
	for (std::shared_ptr<element_information>& member : cast -> members) {
		if (member -> attr_flag & ATTRIBUTE_SIZE) {
			sizes.push(name + "." + member -> name);
		}
	}
	return emit(
		"$(STRUCT_NAME)_start(&builder);\n"
		"$(SERIALIZE_NON_VLA_MEMBERS)"
		"$(SERIALIZE_VLA_MEMBERS)"
		"$(BUILD_STRUCT)\n"
		"$(SERIALIZED_NAME) = $(STRUCT_NAME)_end(&builder);" , {
			// the name of the struct
			{"$(STRUCT_NAME)", cast -> name},
			// the serialization of non-vla members
			{"$(SERIALIZE_NON_VLA_MEMBERS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& member : cast -> members) {
					if (!(member -> attr_flag & ATTRIBUTE_VLA)) {
						ret += emit_serialize_member(name, member, indent);
					}
				}
				return ret;
			}()},
			{"$(SERIALIZE_VLA_MEMBERS)", [&]() -> std::string {
				std::string ret;
				for (std::shared_ptr<element_information>& member : cast -> members) {
					if (member -> attr_flag & ATTRIBUTE_VLA) {
						if (member -> attr_flag & ATTRIBUTE_STR) {
							std::string var_name = emit("__$(INDENT)_$(MEMBER_NAME)_keyedge_size", {
								{"$(INDENT)", std::to_string(indent)},
								{"$(MEMBER_NAME)", member -> name}
							});
							ret += emit("size_t $(VAR_NAME) = 
						}
						
