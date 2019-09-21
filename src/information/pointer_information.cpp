#include "index.h"

std::string pointer_information::str(const std::string& name) const {
	return (*type) -> str(std::string("*") + name);
}

std::string pointer_information::fbs_type() const {
	return std::string("__pointer_") + (*type) -> flatcc_prefix();
}

std::string pointer_information::flatcc_prefix() const {
	return std::string("__pointer_") + (*type) -> flatcc_prefix();
}

std::string pointer_information::flatcc_type() const {
	return flatcc_prefix() + "_table_t";
}

std::string pointer_information::flatcc_reference() const {
	return flatcc_prefix() + "_ref_t";
}

