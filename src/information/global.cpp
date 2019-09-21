#include "index.h"

std::map<std::string, std::shared_ptr<type_information>> type_lookup;
std::vector<std::shared_ptr<type_information>> type_pool;
std::map<std::string, std::shared_ptr<function_information>> function_lookup;
std::vector<std::shared_ptr<struct_information>> struct_pool;
std::vector<std::shared_ptr<pointer_information>> pointer_pool;

