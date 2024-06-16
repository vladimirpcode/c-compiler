#pragma once

#include "parser.h"

// ISO/IEC 9899:1999 (E) A.2.3 Statements
void parse_statement(Scanner& scan, AST*& ast, int depth);
void parse_labeled_statement(Scanner& scan, AST*& ast, int depth);
void parse_compound_statement(Scanner& scan, AST*& ast, int depth);
void parse_block_item_list(Scanner& scan, AST*& ast, int depth);
void parse_block_item(Scanner& scan, AST*& ast, int depth);
void parse_expression_statement(Scanner& scan, AST*& ast, int depth);
void parse_selection_statement(Scanner& scan, AST*& ast, int depth);
void parse_iteration_statement(Scanner& scan, AST*& ast, int depth);
void parse_jump_statement(Scanner& scan, AST*& ast, int depth);