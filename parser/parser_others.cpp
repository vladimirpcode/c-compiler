#include "parser_others.h"

#include "parser.h"
#include <iostream>

void parse_identifier(Scanner& scan){
    DEBUG_PRINT("parse_identifier\n");
    check(scan, Lex::Ident);
}

void parse_constant(Scanner& scan){
    DEBUG_PRINT("parse_constant\n");
    if (scan.current_state.lex == Lex::IntLiteral
        || scan.current_state.lex == Lex::FloatLiteral
        || scan.current_state.lex == Lex::CharLiteral
        || scan.current_state.lex == Lex::EnumConstant)
    {
        scan.get_next();
    } else {
        throw ExpectedLexException("константа", scan.get_line_for_compiler_msg());
    }
}

void parse_enumeration_constant(Scanner& scan){
    DEBUG_PRINT("parse_enumeration_constant\n");
    parse_identifier(scan);
}