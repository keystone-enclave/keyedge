#include "index.h"

namespace __array_serialization {

	// the name of the looping variable
	std::string var_name(size_t i, size_t indent) {
		return emit("i$(INDENT)_$(I)", {
			{"$(INDENT)", std::to_string(indent)},
			{"$(I)", std::to_string(i)}
		});
	}

	// the header of the loop
	std::string loop_begin(std::shared_ptr<array_information> type, size_t indent) {
		std::string ret;
		for (size_t i = 0; i < type -> lengths.size(); ++i) {
			ret += emit (
				"for (size_t $(VAR) = 0; $(VAR) < (size_t) ($(ITH_DIMENSION_LENGTH)); ++$(VAR)) ", {
					{"$(VAR)", var_name(i, indent)},
					{"$(ITH_DIMENSION_LENGTH)", type -> lengths[i]}
				});
		}
		return ret;
	}

	// the length of the array, compressed to one-dimension
	std::string length(std::shared_ptr<array_information> type) {
		std::string ret = "1";
		for (size_t i = 0; i < type -> lengths.size(); ++i) {
			ret += emit(" * ($(ITH_DIMENSION_LENGTH))", {
				{"$(ITH_DIMENSION_LENGTH)", type -> lengths[i]}
			});
		}
		return ret;
	}

	// the one-dimension index of the array in the loop
	std::string index(std::shared_ptr<array_information> type, size_t indent) {
		std::string ret;
		for (size_t i = 0; i < type -> lengths.size(); ++i) {
			ret += emit("$(VAR) + ($(ITH_DIMENSION_LENGTH)) * (", {
				{"$(VAR)", var_name(i, indent)},
				{"$(ITH_DIMENSION_LENGTH)", type -> lengths[i]}
			});
		}
		ret += "0";
		for (size_t i = 0; i < type -> lengths.size(); ++i) {
			ret += ")";
		}
		return ret;
	}

	// the name of the element, used to process the elements
	std::string element_name(const std::string& name, std::shared_ptr<array_information> type, size_t indent) {
		return emit("($(CONVERSION) $(NAME))$(SUFFIX)", {
			{"$(CONVERSION)", (*type -> element_type) -> str() == "void" ? "(char*)" : ""},
			{"$(NAME)", name},
			{"$(SUFFIX)", [&]() -> std::string {
				std::string ret;
				for (size_t i = 0; i < type -> lengths.size(); ++i) {
					ret = emit("[$(VAR)]", {{"$(VAR)", var_name(i, indent)}}) + ret;
				}
				return ret;
			}()}
		});
	}

}

using namespace __array_serialization;

std::string emit_serialize_array(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<array_information> type, size_t indent) {
	// the name of the temporary array holding all elements
	std::string data_name = emit("__keyedge_array_data_$(INDENT)", {{"$(INDENT)", std::to_string(indent)}});
	return emit(
		"$(DATA_TYPE)* $(DATA_NAME) = ($(DATA_TYPE)*) malloc($(LENGTH));\n"
		"$(LOOP_BEGIN){\n"
		"\t$(SERIALIZE_ELEMENT)\n"
		"}\n"
		"$(SERIALIZED_NAME) = $(FLATCC_PREFIX)_create(&builder, $(CONVERSION) $(DATA_NAME), $(LENGTH));\n"
		"free($(DATA_NAME));", {
			// the element type represented in flatcc
			{"$(DATA_TYPE)", (*type -> element_type) -> flatcc_reference() == "void" ? "char" :
				(*type -> element_type) -> flatcc_reference()},
			// the name of the temporary array holding all elements
			{"$(DATA_NAME)", data_name},
			// the length of the array, compressed to one-dimension
			{"$(LENGTH)", length(type)},
			// the header of the loop
			{"$(LOOP_BEGIN)", loop_begin(type, indent)},
			// the serialization of each element
			{"$(SERIALIZE_ELEMENT)", [&]() -> std::string {
				std::string element_serialized_name = emit("$(DATA_NAME)[$(INDEX)]", {
					{"$(DATA_NAME)", data_name},
					{"$(INDEX)", index(type, indent)}
				});
				return emit_serialize(element_name(name, type, indent), element_serialized_name,
					*type -> element_type, indent + 1);
			}()},
			// the variable we want to serialize our data to
			{"$(SERIALIZED_NAME)", serialized_name},
			// the flatcc prefix of the type
			{"$(FLATCC_PREFIX)", type -> flatcc_prefix()},
			// g++ forbids implicit conversion between "signed char*" and "char*" / "void*", so cast it for flatcc
			{"$(CONVERSION)", (*type -> element_type) -> str() == "char" ||
				(*type -> element_type) -> str() == "void" ? "(signed char*)" : ""}
		}, indent);
}

std::string emit_deserialize_array(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<array_information> type, size_t indent) {
	return emit(
		"$(MALLOC)\n"
		"$(LOOP_BEGIN){\n"
		"\t$(DESERIALIZE_ELEMENT)\n"
		"}", {
			// malloc for the vla array
			{"$(MALLOC)", type -> is_vla ? 
				emit("$(NAME) = ($(POINTER_TYPE)) malloc(($(LENGTH)) * sizeof($(SIZE_TYPE)));", {
					// the name of the variable
					{"$(NAME)", name},
					// the type of the variable
					{"$(POINTER_TYPE)", (*type -> element_type) -> str("*")},
					// the length of the array, compressed to one-dimension
					{"$(LENGTH)", length(type)},
					// the type of size of each element
					{"$(SIZE_TYPE)", (*type -> element_type) -> str() == "void" ? "char" :
						(*type -> element_type) -> str()}
				}) : ""},
			// the header of the loop
			{"$(LOOP_BEGIN)", loop_begin(type, indent)},
			// the deserialization of each element
			{"$(DESERIALIZE_ELEMENT)", [&]() -> std::string {
				std::string element_serialized_name = emit("$(FLATCC_PREFIX)_at($(SERIALIZED_NAME), $(INDEX))", {
					{"$(FLATCC_PREFIX)", type -> flatcc_prefix()},
					{"$(SERIALIZED_NAME)", serialized_name},
					{"$(INDEX)", index(type, indent)}
				});
				return emit_deserialize(element_name(name, type, indent), element_serialized_name,
					*type -> element_type, indent + 1);
			}()}
		}, indent);
}

