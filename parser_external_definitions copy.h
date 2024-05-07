#pragma once

#include "parser.h"

// ISO/IEC 9899:1999 (E) A.2.4 External definitions
void parse_translation_unit(Scanner& scan);
void parse_external_declaration(Scanner& scan);
void parse_function_definition(Scanner& scan);
void parse_declaration_list(Scanner& scan);
