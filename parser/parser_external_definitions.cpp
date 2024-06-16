#include "parser_external_definitions.h"

#include "parser_declarations.h"
#include "parser_statements.h"
#include "parser_declarations.h"
#include "try_parse.h"
#include <iostream>

void parse_translation_unit(Scanner& scan, AST*& ast, int depth){
    DEBUG_PRINT(depth, "parse_translation_unit\n");
    parse_external_declaration(scan, ast, depth + 1);
    while (scan.current_state.lex != Lex::EOT){
        parse_external_declaration(scan, ast, depth + 1);
    }
}

void parse_external_declaration(Scanner& scan, AST*& ast, int depth){
    DEBUG_PRINT(depth, "parse_external_declaration\n");
    if (try_parse(parse_function_definition, scan, ast, depth + 1)){
        
    } else {
        parse_declaration(scan, ast, depth + 1);
    }
}

void parse_function_definition(Scanner& scan, AST*& ast, int depth){
    DEBUG_PRINT(depth, "parse_function_definition\n");
    parse_declaration_specifiers(scan, ast, depth + 1);
    parse_declarator(scan, ast, depth + 1);
    try_parse(parse_declaration_list, scan, ast, depth + 1);
    parse_compound_statement(scan, ast, depth + 1);
}

void parse_declaration_list(Scanner& scan, AST*& ast, int depth){
    DEBUG_PRINT(depth, "parse_declaration_list\n");
    // вызывается только в parse_function_definition
    parse_declaration(scan, ast, depth + 1);
    while (try_parse(parse_declaration, scan, ast, depth + 1)){
        
    }
}
