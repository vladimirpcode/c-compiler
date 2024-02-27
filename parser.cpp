#include "parser.h"

#include "scanner.h"
#include <string>
#include <iostream>


using namespace std::string_literals;


ParsingException::ParsingException(const std::string& msg) : m_msg(msg){
    
}

const char* ParsingException::what() const noexcept{
    return m_msg.c_str();
}

void check(Scanner& scan, Lex lex){
    if (scan.lex == lex){
        scan.get_next();
    } else {
        throw ParsingException("ожидалось "s + to_string(lex) + " но "s + to_string(scan.lex));
    }
}

void parse_function_parameters(Scanner& scan){
    if (scan.lex != Lex::PrimitiveType){
        throw ParsingException("Ожидался тип аргумента функции, но "s + to_string(scan.lex));
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

void parse_expression(Scanner& scan);

void parse_factor(Scanner& scan){
    if (scan.lex == Lex::IntNumber || scan.lex == Lex::FloatNumber){
        scan.get_next();
    } else if (scan.lex == Lex::Ident){
        scan.get_next();
    } else if (scan.lex == Lex::Plus || scan.lex == Lex::Minus){
        scan.get_next();
    } else if (scan.lex == Lex::Increment || scan.lex == Lex::Decrement){
        scan.get_next();
    } else if (scan.lex == Lex::LBrace){
        scan.get_next();
        parse_expression(scan);
        check(scan, Lex::RBrace);
    }
}

void parse_term(Scanner& scan){
    parse_factor(scan);
    while (scan.lex == Lex::Asterisk || scan.lex == Lex::Slash){
        scan.get_next();
        parse_factor(scan);
    }
}

void parse_expression(Scanner& scan){
    parse_term(scan);
    while (scan.lex == Lex::Plus || scan.lex == Lex::Minus){
        scan.get_next();
        parse_term(scan);
    }
}

void parse_function_body(Scanner& scan){
    if (scan.lex == Lex::PrimitiveType){
        // объявление переменной
    } else if (scan.lex == Lex::Return){
        scan.get_next();
        parse_expression(scan);
        check(scan, Lex::SemiColon); 
    }
}


void parse(const std::string& translation_unit_text){
    Scanner scan(translation_unit_text);
    // функция или глобальная переменная
    check(scan, Lex::PrimitiveType);
    check(scan, Lex::Ident);
    if (scan.lex == Lex::LBrace){
        // функция
        scan.get_next();
        std::cout << "123\n";
        parse_function_parameters(scan);
        check(scan, Lex::RBrace);
        check(scan, Lex::LCurlyBrace);
        parse_function_body(scan);
        check(scan, Lex::RCurlyBrace);

    } else {
        // глобальная переменная
    }

}
