#include "index.h"

std::string array_information::str(const std::string& name) const {
	if (is_vla) return (*element_type) -> str(std::string("*") + name);
	std::string name_lengths = name;
	for (size_t i = lengths.size(); i > 0; --i)
		name_lengths += std::string("[") + lengths[i - 1] + "]";
	return (*element_type) -> str(name_lengths);
}

std::string array_information::fbs_type() const {
	return std::string("[") + (*element_type) -> fbs_type() + std::string("]");
}

std::string array_information::flatcc_prefix() const {
	return (*element_type) -> flatcc_prefix() + "_vec";
}

std::string array_information::flatcc_type() const {
	return flatcc_prefix() + "_t";
}

std::string array_information::flatcc_reference() const {
	return flatcc_prefix() + "_ref_t";
}

