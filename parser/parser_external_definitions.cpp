#include "parser_external_definitions.h"

#include "parser_declarations.h"
#include "parser_statements.h"
#include "parser_declarations.h"
#include "try_parse.h"
#include <iostream>

void parse_translation_unit(Scanner& scan){
    DEBUG_PRINT("parse_translation_unit\n");
    parse_external_declaration(scan);
    while (scan.current_state.lex != Lex::EOT){
        parse_external_declaration(scan);
    }
}

void parse_external_declaration(Scanner& scan){
    DEBUG_PRINT("parse_external_declaration\n");
    if (try_parse(parse_function_definition, scan)){
        return;
    }

    parse_declaration(scan);
}

void parse_function_definition(Scanner& scan){
    DEBUG_PRINT("parse_function_definition\n");
    parse_declaration_specifiers(scan);
    parse_declarator(scan);
    try_parse(parse_declaration_list, scan);
    parse_compound_statement(scan);
}

void parse_declaration_list(Scanner& scan){
    DEBUG_PRINT("parse_declaration_list\n");
    parse_declaration(scan);
    while (try_parse(parse_declaration, scan)){
        
    }
}
