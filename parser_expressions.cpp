#include "parser_expressions.h"


void parse_primary_expression(Scanner& scan){
    if (scan.current_state.lex == Lex::Ident){
        scan.get_next();
    } else if(scan.current_state.lex == Lex::FloatLiteral
        || scan.current_state.lex == Lex::IntLiteral){
        scan.get_next();
    } else if (scan.current_state.lex == Lex::CharLiteral){
        scan.get_next();
    } else if (scan.current_state.lex == Lex::StringLiteral){
        scan.get_next();
    } else if (scan.current_state.lex == Lex::LBrace) {
        scan.get_next();
        parse_expression(scan);
        check(scan, Lex::RBrace);
    } else {
        expected(scan, "ident | constant | string literal | (expression)", to_string(scan.current_state.lex));
    }
}

void parse_postfix_expression(Scanner& scan){
    
}

void parse_argument_expression_list(Scanner& scan){

}

void parse_unary_expression(Scanner& scan){

}

void parse_unary_operator(Scanner& scan){

}

void parse_cast_expression(Scanner& scan){

}

void parse_multiplicative_expression(Scanner& scan){

}

void parse_additive_expression(Scanner& scan){

}

void parse_shift_expression(Scanner& scan){

}

void parse_relational_expression(Scanner& scan){

}

void parse_equality_expression(Scanner& scan){

}

void parse_and_expression(Scanner& scan){

}

void parse_exclusive_or_expression(Scanner& scan){

}

void parse_inclusive_or_expression(Scanner& scan){

}

void parse_logical_and_expression(Scanner& scan){

}

void parse_logical_or_expression(Scanner& scan){

}

void parse_conditional_expression(Scanner& scan){

}

void parse_assignment_expression(Scanner& scan){

}

void parse_assigment_operator(Scanner& scan){

}

void parse_expression(Scanner& scan){

}

void parse_constant_expression(Scanner& scan){

}