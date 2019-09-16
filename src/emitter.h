#ifndef __EMITTER_H
#define __EMITTER_H

#include "parser.h"

#include <memory>
#include <sstream>
#include <typeinfo>

std::string emit_common(size_t indent = 0);
std::string emit_eapp(size_t indent = 0);
std::string emit_host(size_t indent = 0);
std::string emit_fbs(size_t indent = 0);

#endif

