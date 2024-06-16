#pragma once

#include "../scanner.h"
#include "../ast.h"

void parse_identifier(Scanner& scan, AST*& ast, int depth);
void parse_constant(Scanner& scan, AST*& ast, int depth);
void parse_enumeration_constant(Scanner& scan, AST*& ast, int depth);