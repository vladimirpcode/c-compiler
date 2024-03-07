#include "parser.h"

#include "scanner.h"
#include <string>
#include <iostream>
#include "name_table.h"

using namespace std::string_literals;

void error(const Scanner& scan, const std::string& msg){
    throw ParsingException(scan.get_line_for_compiler_msg() + "\n"s + msg);
}

void expected(const Scanner& scan, const std::string& expected_value, const std::string& actual_value){
    throw ParsingException(scan.get_line_for_compiler_msg() + "\n"s + "ожидалось "s + expected_value + " но, "s + actual_value);
}

void check(Scanner& scan, Lex lex){
    if (scan.lex == lex){
        scan.get_next();
    } else {
        expected(scan, to_string(lex), to_string(scan.lex));
    }
}

/* ISO/IEC 9899:1999(E) A.2.4
function-definition:
    declaration-specifiers declarator [declaration-list] compound-statement
*/
void parse_function_definition(Scanner& scan){

}

/* ISO/IEC 9899:1999(E) A.2.2
declarator:
    [pointer] direct-declarator
*/
void parse_declarator(Scanner& scan){
    
}

/* ISO/IEC 9899:1999(E) A.2.2
init-declarator:
    declarator
    declarator = initializer
*/
void parse_init_declarator(Scanner& scan){
    parse_declarator(scan);
    if (scan.lex == Lex::Assigment){
        scan.get_next();
        parse_initializer(scan);
    }
}

/* ISO/IEC 9899:1999(E) A.2.2
declaration:
    declaration-specifiers [init-declarator-list] ;
init-declarator-list:
    init-declarator
    init-declarator-list , init-declarator
*/
void parse_declaration(Scanner& scan){
    parse_declaration_specifiers(scan);
    if (scan.lex == Lex::SemiColon){
        scan.get_next();
        return;
    }
    parse_init_declarator(scan);
    while (scan.lex == Lex::Comma){
        scan.get_next();
        parse_init_declarator(scan);
    }
    check(scan, Lex::SemiColon);

}

/* ISO/IEC 9899:1999(E) A.2.4
translation-unit:
    external-declaration
    translation-unit external-declaration
external-declaration:
    function-definition
    declaration
*/
void parse_translation_unit(Scanner& scan){
    //look ahead
    while (scan.lex != Lex::EOT){
        try {
            parse_function_definition(scan);
            return;
        } catch (ParsingException e){}
        
        parse_declaration(scan);
    }
}

void parse(const std::string& translation_unit_text){
    Scanner scan(translation_unit_text);
    // глобальная область видимость
    name_table.entries.clear(); //ToDo отрефакторить, определить где лежать таблице имен
    name_table.open_scope();
    

}
