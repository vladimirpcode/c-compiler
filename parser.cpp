#include "parser.h"

#include "scanner.h"
#include <string>
#include <iostream>


using namespace std::string_literals;


void check(Scanner& scan, Lex lex){
    if (scan.lex == lex){
        scan.get_next();
    } else {
        throw ParsingException("ожидалось "s + to_string(lex) + " но "s + to_string(scan.lex));
    }
}
void parse_expression(Scanner& scan);

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

void parse_end_value(Scanner& scan){
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

void parse_selector(Scanner& scan){
    parse_end_value(scan);
    if (scan.lex == Lex::LBrace){
        scan.get_next();
        parse_function_parameters(scan);
        check(scan, Lex::RBrace);
    } else if (scan.lex == Lex::LSquareBrace){
        scan.get_next();
        parse_end_value(scan);
        check(scan, Lex::RSquareBrace);
    } else if (scan.lex == Lex::Arrow){
        scan.get_next();
        check(scan, Lex::Ident);
        if (scan.lex == Lex::LBrace){
            scan.get_next();
            parse_function_parameters(scan);
            check(scan, Lex::RBrace);
        }
    } else if (scan.lex == Lex::Dot){
        scan.get_next();
        check(scan, Lex::Ident);
        if (scan.lex == Lex::LBrace){
            scan.get_next();
            parse_function_parameters(scan);
            check(scan, Lex::RBrace);
        }
    }
}

bool is_unary_operator(Lex lex){
    return lex == Lex::Exclamation
        || lex == Lex::Tilde
        || lex == Lex::Increment
        || lex == Lex::Decrement
        || lex == Lex::Plus
        || lex == Lex::Minus
        || lex == Lex::Asterisk
        || lex == Lex::BitwiseAnd
        || lex == Lex::Sizeof
        || lex == Lex::LBrace;
}

void parse_factor(Scanner& scan){
    if (is_unary_operator(scan.lex)){
        scan.get_next();
    }
    parse_selector(scan);
}

void parse_term(Scanner& scan){
    parse_factor(scan);
    while (scan.lex == Lex::Asterisk 
        || scan.lex == Lex::Slash
        || scan.lex == Lex::Mod){
        scan.get_next();
        parse_factor(scan);
    }
}

void parse_term_expression(Scanner& scan){
    parse_term(scan);
    while (scan.lex == Lex::Plus 
        || scan.lex == Lex::Minus){
        scan.get_next();
        parse_term(scan);
    }
}

// expression >> expression
void parse_shift(Scanner& scan){
    parse_term_expression(scan);
    while (scan.lex == Lex::LeftShift 
        || scan.lex == Lex::RightShift){
        scan.get_next();
        parse_term_expression(scan);
    }
}

// expression > expression
void parse_compare(Scanner& scan){
    parse_shift(scan);
    while (scan.lex == Lex::GreaterThen || scan.lex == Lex::LessThen
        || scan.lex == Lex::GreaterEqualThen || scan.lex == Lex::LessEqualThen){
        scan.get_next();
        parse_shift(scan);
    }
}

// expression == expression
void parse_equal_compare(Scanner& scan){
    parse_compare(scan);
    while (scan.lex == Lex::Compare || scan.lex == Lex::CompareNegative){
        scan.get_next();
        parse_compare(scan);
    }
}

// expression & expression
void parse_bitwise_and(Scanner& scan){
    parse_equal_compare(scan);
    while (scan.lex == Lex::BitwiseAnd){
        scan.get_next();
        parse_equal_compare(scan);
    }
}


// expression ^ expression
void parse_bitwise_xor(Scanner& scan){
    parse_bitwise_and(scan);
    while (scan.lex == Lex::Xor){
        scan.get_next();
        parse_bitwise_and(scan);
    }
}

// expression | expression
void parse_bitwise_or(Scanner& scan){
    parse_bitwise_xor(scan);
    while (scan.lex == Lex::BitwiseOr){
        scan.get_next();
        parse_bitwise_xor(scan);
    }
}

// expression && expression
void parse_logical_and(Scanner& scan){
    parse_bitwise_or(scan);
    while (scan.lex == Lex::LogicalAnd){
        scan.get_next();
        parse_bitwise_or(scan);
    }
}

// expression || expression
void parse_logical_or(Scanner& scan){
    parse_logical_and(scan);
    while (scan.lex == Lex::LogicalOr){
        scan.get_next();
        parse_logical_and(scan);
    }
}

// expression ? expression : expression
// условие    знач. если да  знач. если нет
void parse_ternary(Scanner& scan){
    parse_logical_or(scan);
    if (scan.lex == Lex::QuestionMark){
        scan.get_next();
        parse_logical_or(scan);
        check(scan, Lex::Colon);
        parse_logical_or(scan);
    }
}

// var += expression
void parse_assigment(Scanner& scan){
    parse_ternary(scan);
}

bool is_assigment(Lex lex){
    return lex == Lex::Assigment
        || lex == Lex::AssigmentPlus
        || lex == Lex::AssigmentMinus
        || lex == Lex::AssigmentDivide
        || lex == Lex::AssigmentMod
        || lex == Lex::AssigmentMultiply
        || lex == Lex::AssigmentLeftShift
        || lex == Lex::AssigmentRightShift
        || lex == Lex::AssigmentAnd
        || lex == Lex::AssigmentOr
        || lex == Lex::AssigmentXor;
}

// expression , expression
void parse_comma_separated_expr(Scanner& scan){
    parse_assigment(scan);
    while (is_assigment(scan.lex)){
        scan.get_next();
        parse_assigment(scan);
    }
}

void parse_expression(Scanner& scan){
    parse_comma_separated_expr(scan);
    if (scan.lex == Lex::Comma){
        scan.get_next();
        parse_comma_separated_expr(scan);
    }
}

bool is_correct_expression_start(Lex lex){
    return lex == Lex::PrimitiveType 
        || lex == Lex::Return
        || lex == Lex::Goto
        || lex == Lex::IntNumber
        || lex == Lex::FloatNumber
        || lex == Lex::Ident;
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

void parse(const std::string& translation_unit_text){
    Scanner scan(translation_unit_text);
    // функция или глобальная переменная
    check(scan, Lex::PrimitiveType);
    check(scan, Lex::Ident);
    if (scan.lex == Lex::LBrace){
        // функция
        scan.get_next();
        parse_function_parameters_declaration(scan);
        check(scan, Lex::RBrace);
        check(scan, Lex::LCurlyBrace);
        parse_function_body(scan);
        check(scan, Lex::RCurlyBrace);

    } else {
        // глобальная переменная
    }

}
