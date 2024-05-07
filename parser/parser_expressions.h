#pragma once

#include "parser.h"

// ISO/IEC 9899:1999 (E) A.2.1 Expressions
void parse_primary_expression(Scanner& scan);
void parse_postfix_expression(Scanner& scan);
void parse_argument_expression_list(Scanner& scan);
void parse_unary_expression(Scanner& scan);
void parse_unary_operator(Scanner& scan);
void parse_cast_expression(Scanner& scan);
void parse_multiplicative_expression(Scanner& scan);
void parse_additive_expression(Scanner& scan);
void parse_shift_expression(Scanner& scan);
void parse_relational_expression(Scanner& scan);
void parse_equality_expression(Scanner& scan);
void parse_and_expression(Scanner& scan);
void parse_exclusive_or_expression(Scanner& scan);
void parse_inclusive_or_expression(Scanner& scan);
void parse_logical_and_expression(Scanner& scan);
void parse_logical_or_expression(Scanner& scan);
void parse_conditional_expression(Scanner& scan);
void parse_assignment_expression(Scanner& scan);
void parse_assigment_operator(Scanner& scan);
void parse_expression(Scanner& scan);
void parse_constant_expression(Scanner& scan);