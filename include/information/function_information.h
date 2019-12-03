// functions
struct function_information {
	std::string name;
	size_t index;
	std::string buffer_size;
	int attr_flag;
	type_indicator return_type;
	std::vector<std::shared_ptr<element_information>> arguments;
};

