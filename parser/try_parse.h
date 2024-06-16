#pragma once

#include "../scanner.h"
#include "../ast.h"

using parsing_function = void (*)(Scanner&, AST*&, int);

bool try_parse(parsing_function func, Scanner& scan, AST*& ast, int depth);
bool try_parse(std::vector<parsing_function> functions, Scanner& scan, AST*& ast, int depth);