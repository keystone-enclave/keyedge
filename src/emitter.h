#ifndef __EMITTER_H
#define __EMITTER_H

#include "tokenizer.h"
#include "parser.h"

#include <memory>
#include <sstream>
#include <typeinfo>

std::string emit_header_eapp(size_t indent = 0);
std::string emit_header_host(size_t indent = 0);
std::string emit_end_eapp(size_t indent = 0);
std::string emit_end_host(size_t indent = 0);
std::string emit_function_eapp(std::shared_ptr<function_information> f, size_t indent = 0);
std::string emit_function_host(std::shared_ptr<function_information> f, size_t indent = 0);

#endif

