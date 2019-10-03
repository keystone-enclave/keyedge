// structs
struct struct_information : type_information {
	std::string name;
	std::vector<std::shared_ptr<element_information>> members;
	virtual std::string str(const std::string& name = "") const;
	virtual std::string fbs_type() const;
	virtual std::string flatcc_prefix() const;
	virtual std::string flatcc_type() const;
	virtual std::string flatcc_reference() const;
};

