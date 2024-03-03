#pragma once

#include "scanner.h"

bool is_correct_expression_start(Lex lex);
void parse_expression(Scanner& scan);