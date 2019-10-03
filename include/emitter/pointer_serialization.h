std::string emit_serialize_pointer(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<pointer_information> type, size_t indent = 0);
std::string emit_deserialize_pointer(const std::string& name, const std::string& serialized_name,
	std::shared_ptr<pointer_information> type, size_t indent = 0);
