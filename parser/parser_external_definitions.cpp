#include "parser_external_definitions.h"

#include "parser_declarations.h"
#include "parser_statements.h"
#include "parser_declarations.h"
#include "try_parse.h"

void parse_translation_unit(Scanner& scan){
    parse_external_declaration(scan);
    while (scan.current_state.lex != Lex::EOT){
        parse_external_declaration(scan);
    }
}

void parse_external_declaration(Scanner& scan){
    if (try_parse(parse_function_definition, scan)){
        return;
    }

    parse_declaration(scan);
}

void parse_function_definition(Scanner& scan){
    parse_declaration_specifiers(scan);
    parse_declarator(scan);
    try_parse(parse_declaration_list, scan);
    parse_compound_statement(scan);
}

void parse_declaration_list(Scanner& scan){
    parse_declaration(scan);
    while (try_parse(parse_declaration, scan)){
        
    }
}
