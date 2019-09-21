// primitive types
struct primitive_type_information : type_information {
	std::string __str;
	std::string __flatcc_type;
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
	primitive_type_information(const std::string& str, const std::string& flatcc_type)
		: __str(str), __flatcc_type(flatcc_type) {}
};

