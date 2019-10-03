// type information
struct type_information {
	// build a type declaration with the name specified
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
	type_information() {}
	virtual ~type_information() {}
};

