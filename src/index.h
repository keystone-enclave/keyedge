#ifndef __INDEX_H

#define __INDEX_H

// STL
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

// LibClang
#include <clang-c/Index.h>

// attributes
#include "attributes.h"

// information
#include "information/type_information.h"
#include "information/type_indicator.h"
#include "information/element_information.h"
#include "information/function_information.h"

#include "information/primitive_type_information.h"
#include "information/struct_information.h"
#include "information/array_information.h"
#include "information/pointer_information.h"

#include "information/global.h"

// parser
#include "parser.h"

// emitter
#include "emitter/basic.h"
#include "emitter/array_serialization.h"
#include "emitter/struct_serialization.h"
#include "emitter/pointer_serialization.h"
#include "emitter/serialization.h"
#include "emitter/emit_fbs.h"
#include "emitter/emit_common.h"
#include "emitter/emit_eapp.h"
#include "emitter/emit_host.h"

#endif

