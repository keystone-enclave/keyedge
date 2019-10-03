std::string emit_serialize_struct(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<struct_information> type, size_t indent = 0);
std::string emit_deserialize_struct(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<struct_information> type, size_t indent = 0);

