#include "index.h"

bool type_indicator::operator !() const {
	return is_null;
}

std::shared_ptr<type_information> type_indicator::operator *() const {
	if (str.empty()) return type_pool[p];
	return type_lookup[str];
}

