#pragma once

#include "parser.h"

// ISO/IEC 9899:1999 (E) A.2.4 External definitions
void parse_translation_unit(Scanner& scan, AST*& ast, int depth);
void parse_external_declaration(Scanner& scan, AST*& ast, int depth);
void parse_function_definition(Scanner& scan, AST*& ast, int depth);
void parse_declaration_list(Scanner& scan, AST*& ast, int depth);
