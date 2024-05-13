#include "parser_external_definitions.h"

#include "parser_declarations.h"
#include "parser_statements.h"
#include "parser_declarations.h"
#include "try_parse.h"
#include <iostream>

void parse_translation_unit(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_translation_unit\n");
    ast->left = ast_manager.get_new_ast_instance();
    parse_external_declaration(scan, ast->left);
    while (scan.current_state.lex != Lex::EOT){
        AST *temp_ast = ast;
        ast = ast_manager.get_new_ast_instance();
        parse_external_declaration(scan, ast);
        ast->left = temp_ast;
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
    parse_declaration_specifiers(scan);
    parse_declarator(scan);
    try_parse(parse_declaration_list, scan);
    parse_compound_statement(scan);
}

void parse_declaration_list(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_declaration_list\n");
    parse_declaration(scan);
    while (try_parse(parse_declaration, scan)){
        
    }
}
