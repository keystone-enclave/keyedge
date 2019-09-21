// type indicators
struct type_indicator {
	//If it is a primitive type or a struct, use the name.
	std::string str;
	//Otherwise, use the index of type_pool.
	size_t p;
	bool is_null;
	type_indicator() : is_null(true) {}
	type_indicator(const std::string& str) : str(str), is_null(false) {}
	type_indicator(size_t p) : p(p), is_null(false) {}
	bool operator !() const;
	std::shared_ptr<type_information> operator *() const;
};

