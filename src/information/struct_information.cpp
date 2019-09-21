#include "index.h"

std::string struct_information::str(const std::string& name) const {
	return this -> name + (name.empty() ? std::string() : std::string(" ") + name);
}

std::string struct_information::fbs_type() const {
	return str();
}

std::string struct_information::flatcc_prefix() const {
	return str(); 
}

std::string struct_information::flatcc_type() const {
	return flatcc_prefix() + "_table_t";
}

std::string struct_information::flatcc_reference() const {
	return flatcc_prefix() + "_ref_t";
}

