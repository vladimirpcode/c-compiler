#pragma once

#include "ast.h"

void debug_ast_print(AST* headAstNode);
void generate_native_code(AST* headAstNode);