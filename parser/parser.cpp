#include "parser.h"

#include "../scanner.h"
#include <string>
#include <iostream>
#include "../name_table.h"
#include "parser_external_definitions.h"
#include "../ast.h"

using namespace std::string_literals;

void error(const Scanner& scan, const std::string& msg){
    throw ParsingException(scan.get_line_for_compiler_msg() + "\n"s + msg);
}

void expected(const Scanner& scan, const std::string& expected_value, const std::string& actual_value){
    throw ParsingException(scan.get_line_for_compiler_msg() + "\n"s + "ожидалось "s + expected_value + " но, "s + actual_value);
}

void check(Scanner& scan, Lex lex){
    if (scan.current_state.lex == lex){
        scan.get_next();
    } else {
        expected(scan, to_string(lex), to_string(scan.current_state.lex));
    }
}

void parse(const std::string& translation_unit_text){
    Scanner scan(translation_unit_text);
    // глобальная область видимость
    name_table.current_state.entries.clear(); //ToDo отрефакторить, определить где лежать таблице имен
    name_table.open_scope();
    AST *ast = ast_manager.get_new_ast_instance();
    parse_translation_unit(scan, ast);
    ast_manager.delete_all_nodes();
}
