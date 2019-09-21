// arrays
struct array_information : type_information {
	std::vector<std::string> lengths;
	type_indicator element_type;
	bool is_vla;
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
	array_information(const std::vector<std::string>& lengths,
		type_indicator element_type, bool is_vla = false) :
		lengths(lengths), element_type(element_type), is_vla(is_vla) {}
};

