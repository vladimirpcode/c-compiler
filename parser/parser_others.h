#pragma once

#include "../scanner.h"

void parse_identifier(Scanner& scan, AST*& ast);
void parse_constant(Scanner& scan, AST*& ast);
void parse_enumeration_constant(Scanner& scan, AST*& ast);