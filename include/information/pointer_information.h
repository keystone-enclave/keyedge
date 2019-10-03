// pointers
struct pointer_information : type_information {
	type_indicator type;
	pointer_information(type_indicator type) : type(type) {}
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
};

