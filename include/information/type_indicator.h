// type indicators
struct type_indicator {
	// If it is a primitive type or a struct, use the name.
	std::string str;
	// Otherwise, use the index of type_pool.
	size_t p;
	bool is_null;
	// Show if the type it is pointing to is "const"-ed.
	bool is_const;
	type_indicator() : is_null(true), is_const(false) {}
	type_indicator(const std::string& str, bool is_const = false) : str(str), is_null(false), is_const(is_const) {}
	type_indicator(size_t p, bool is_const = false) : p(p), is_null(false), is_const(is_const) {}
	bool operator !() const;
	std::shared_ptr<type_information> operator *() const;
};

