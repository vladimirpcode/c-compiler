#pragma once

#include "../scanner.h"
#include "../ast.h"

using parsing_function = void (*)(Scanner&);

bool try_parse(parsing_function func, AST*& ast);