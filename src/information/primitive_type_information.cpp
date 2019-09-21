#include "index.h"

std::string primitive_type_information::str(const std::string& name) const {
	return __str + (name.empty() ? std::string() : std::string(" ") + name);
}

std::string primitive_type_information::fbs_type() const {
	return __flatcc_type;
}

std::string primitive_type_information::flatcc_prefix() const {
	return "flatbuffers_" + __flatcc_type;
}

std::string primitive_type_information::flatcc_type() const {
	return str();
}

std::string primitive_type_information::flatcc_reference() const {
	return str();
}

