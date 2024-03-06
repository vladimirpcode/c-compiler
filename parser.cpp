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

void parse_function_parameters(Scanner& scan){
    if (!(scan.lex == Lex::IntNumber
        || scan.lex == Lex::FloatNumber
        || scan.lex == Lex::Ident)){
        return;
    }
    parse_expression(scan);
    while (scan.lex == Lex::Comma){
        scan.get_next();
        parse_expression(scan);
    }
}

void parse_function_parameters_declaration(Scanner& scan){
    if (scan.lex != Lex::PrimitiveType){
       expected(scan, "тип аргумента функции, но "s, to_string(scan.lex));
    }

    std::string name = std::get<std::string>(scan.value);
    if (name == "void"s){
        scan.get_next();
        return;
    }
    check(scan, Lex::PrimitiveType);
    if (scan.lex == Lex::Asterisk){
        scan.get_next();
    } else {
        check(scan, Lex::Ident);
    }
    while (scan.lex == Lex::Comma){
        scan.get_next();
        check(scan, Lex::PrimitiveType);
        if (scan.lex == Lex::Asterisk){
            scan.get_next();
        }
        check(scan, Lex::Ident);
    }
}

void parse_function_body(Scanner& scan){
    while (is_correct_expression_start(scan.lex)){
        if (scan.lex == Lex::PrimitiveType){
            // объявление переменной
            scan.get_next();
            check(scan, Lex::Ident);
            if (scan.lex == Lex::Assigment){
                scan.get_next();
                parse_expression(scan);
            }
            check(scan, Lex::SemiColon);
        } else if (scan.lex == Lex::Return){
            scan.get_next();
            parse_expression(scan);
            check(scan, Lex::SemiColon); 
        } else {
            // выражение
            parse_expression(scan);
            check(scan, Lex::SemiColon);
        }
    }
}

PrimitiveType parse_type(Scanner& scan){
    if (scan.lex == Lex::Ident){

    } else {
        
    }
}

void parse(const std::string& translation_unit_text){
    Scanner scan(translation_unit_text);
    // глобальная область видимость
    name_table.entries.clear(); //ToDo отрефакторить, определить где лежать таблице имен
    name_table.open_scope();
    // функция или глобальная переменная
    if (scan.lex != Lex::PrimitiveType){
        expected(scan, "идентификатор"s, to_string(scan.lex));
    }
    PrimitiveType name = std::get<std::string>(scan.value);
    if (scan.lex != Lex::Ident){
        expected(scan, "идентификатор"s, to_string(scan.lex));
    }
    std::string name = std::get<std::string>(scan.value);
    scan.get_next();
    if (name_table.has_name_in_this_scope(name)){
        error(scan, "имя "s + name + " уже существует"s);
    }
    if (scan.lex == Lex::LBrace){
        // функция
        name_table.add_name(name, NameTableEntryType::Function, )
        scan.get_next();
        parse_function_parameters_declaration(scan);
        check(scan, Lex::RBrace);
        check(scan, Lex::LCurlyBrace);
        parse_function_body(scan);
        check(scan, Lex::RCurlyBrace);

    } else {
        // глобальная переменная
        if (scan.lex == Lex::Assigment){
            scan.get_next();
            parse_expression(scan);
        }
        check(scan, Lex::SemiColon);
    }

}
