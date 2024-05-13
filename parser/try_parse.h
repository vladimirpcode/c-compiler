#pragma once

#include "../scanner.h"
#include "../ast.h"

using parsing_function = void (*)(Scanner&, AST*&);

bool try_parse(parsing_function func, Scanner& scan, AST*& ast);