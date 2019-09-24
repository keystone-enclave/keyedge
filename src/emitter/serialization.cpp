#include "index.h"

std::string emit_serialize(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<type_information> type, size_t indent) {
	if (typeid(*type) == typeid(array_information)) {
		// get the exact data type
		std::shared_ptr<array_information> cast =
			std::dynamic_pointer_cast<array_information>(type);
		// the name of the temporary array holding all elements
		std::string data_name = emit("__keyedge_array_data_$(INDENT)", {{"$(INDENT)", std::to_string(indent)}});
		// the name of the looping variable
		auto var_name = [&](size_t i) -> std::string {
			return emit("i$(INDENT)_$(I)", {
				{"$(INDENT)", std::to_string(indent)},
				{"$(I)", std::to_string(i)}
			});
		};
		return emit(
			"{\n"
			"\t$(DATA_TYPE)* $(DATA_NAME) = ($(DATA_TYPE)*) malloc($(LENGTH));\n"
			"\t$(LOOP_BEGIN) {\n"
			"\t$(SERIALIZE_ELEMENT)\n"
			"\t}\n"
			"\t$(SERIALIZED_NAME) = $(FLATCC_PREFIX)_create(&builder, $(CONVERSION) $(DATA_NAME), $(LENGTH));\n"
			"\tfree($(DATA_NAME));\n"
			"}", {
				// the element type represented in flatcc
				{"$(DATA_TYPE)", (*cast -> element_type) -> flatcc_reference() == "void" ? "char" :
					(*cast -> element_type) -> flatcc_reference()},
				// the name of the temporary array holding all elements
				{"$(DATA_NAME)", data_name},
				// the length of the array, compressed to one-dimension
				{"$(LENGTH)", [&]() -> std::string {
					std::string ret = "1";
					for (size_t i = 0; i < cast -> lengths.size(); ++i) {
						ret += emit(" * ($(ITH_DIMENSION_LENGTH))", {
							{"$(ITH_DIMENSION_LENGTH)", cast -> lengths[i]}
						});
					}
					return ret;
				}()},
				// the header of the loop
				{"$(LOOP_BEGIN)", [&]() -> std::string {
					std::string ret;
					for (size_t i = 0; i < cast -> lengths.size(); ++i) {
						ret += emit (
							"for (size_t $(VAR) = 0; $(VAR) < (size_t) ($(ITH_DIMENSION_LENGTH)); ++$(VAR)) ", {
								{"$(VAR)", var_name(i)},
								{"$(ITH_DIMENSION_LENGTH)", cast -> lengths[i]}
							});
					}
					return ret;
				}()},
				// the serialization of each element
				{"$(SERIALIZE_ELEMENT)", [&]() -> std::string {
					std::string element_name = emit("($(CONVERSION) $(NAME))$(SUFFIX)", {
						{"$(CONVERSION)", (*cast -> element_type) -> str() == "void" ? "(char*)" : ""},
						{"$(NAME)", name},
						{"$(SUFFIX)", [&]() -> std::string {
							std::string ret;
							for (size_t i = 0; i < cast -> lengths.size(); ++i) {
								ret = emit("[$(VAR)]", {{"$(VAR)", var_name(i)}}) + ret;
							}
							return ret;
						}()}
					});
					std::string element_serialized_name = emit("$(DATA_NAME)[$(INDEX)]", {
						{"$(DATA_NAME)", data_name},
						{"$(INDEX)", [&]() -> std::string {
							std::string ret;
							for (size_t i = 0; i < cast -> lengths.size(); ++i) {
								ret += emit("$(VAR) + ($(ITH_DIMENSION_LENGTH)) * (", {
									{"$(VAR)", var_name(i)},
									{"$(ITH_DIMENSION_LENGTH)", cast -> lengths[i]}
								});
							}
							ret += "0";
							for (size_t i = 0; i < cast -> lengths.size(); ++i) {
								ret += ")";
							}
							return ret;
						}()}
					});
					return emit_serialize(element_name, element_serialized_name,
						*cast -> element_type, indent + 1);
				}()},
				// the variable we want to serialize our data to
				{"$(SERIALIZED_NAME)", serialized_name},
				// the flatcc prefix of the type
				{"$(FLATCC_PREFIX)", cast -> flatcc_prefix()},
				// g++ forbids implicit conversion between "signed char*" and "char*" / "void*", so cast it for flatcc
				{"$(CONVERSION)", (*cast -> element_type) -> str() == "char" ||
					(*cast -> element_type) -> str() == "void" ? "(signed char*)" : ""}
			}, indent);
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

