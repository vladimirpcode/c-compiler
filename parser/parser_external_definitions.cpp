#include "parser_external_definitions.h"

#include "parser_declarations.h"
#include "parser_statements.h"
#include "parser_declarations.h"
#include "try_parse.h"
#include <iostream>

void parse_translation_unit(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_translation_unit\n");
    parse_external_declaration(scan, ast);
    while (scan.current_state.lex != Lex::EOT){
        parse_external_declaration(scan, ast);
    }
}

void parse_external_declaration(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_external_declaration\n");
    if (try_parse(parse_function_definition, scan, ast)){
        
    } else {
        parse_declaration(scan, ast);
    }
}

void parse_function_definition(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_function_definition\n");
    parse_declaration_specifiers(scan, ast);
    parse_declarator(scan, ast);
    try_parse(parse_declaration_list, scan, ast);
    parse_compound_statement(scan, ast);
}

void parse_declaration_list(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_declaration_list\n");
    // вызывается только в parse_function_definition
    parse_declaration(scan, ast);
    while (try_parse(parse_declaration, scan, ast)){
        
    }
}
