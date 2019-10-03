// functions
struct function_information {
	std::string name;
	size_t index;
	type_indicator return_type;
	std::vector<std::shared_ptr<element_information>> arguments;
};

