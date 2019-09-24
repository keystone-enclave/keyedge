std::string emit_serialize_array(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<array_information> type, size_t indent = 0);
std::string emit_deserialize_array(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<array_information> type, size_t indent = 0);

