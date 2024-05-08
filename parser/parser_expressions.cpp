#include "parser_expressions.h"

#include "try_parse.h"
#include "parser_others.h"
#include "parser_declarations.h"
#include <algorithm>

void parse_primary_expression(Scanner& scan){
    if (scan.current_state.lex == Lex::StringLiteral){
        scan.get_next();
        return;
    }
    if (try_parse(parse_constant, scan)){
        return;
    }
    if (scan.current_state.lex == Lex::LBrace){
        scan.get_next();
        parse_expression(scan);
        check(scan, Lex::RBrace);
    } else {
        throw ExpectedLexException("идентификатор, константа, строковый литерал или ( выражение )", scan.get_line_for_compiler_msg());
    }

}

void parse_postfix_expression(Scanner& scan){
    if (try_parse(parse_primary_expression, scan)){
        
    } else if (scan.current_state.lex == Lex::LBrace){
        scan.get_next();
        parse_type_name(scan);
        check(scan, Lex::RBrace);
        check(scan, Lex::LCurlyBrace);
        parse_initializer_list(scan);
        if (scan.current_state.lex == Lex::Comma){
            scan.get_next();
        }
        check(scan, Lex::RCurlyBrace);
    }
    while (scan.current_state.lex == Lex::LSquareBrace
        || scan.current_state.lex == Lex::LBrace
        || scan.current_state.lex == Lex::Dot
        || scan.current_state.lex == Lex::Arrow
        || scan.current_state.lex == Lex::Increment
        || scan.current_state.lex == Lex::Decrement)
    {
        if (scan.current_state.lex == Lex::Decrement || scan.current_state.lex == Lex::Increment){
            scan.get_next();
        } else if (scan.current_state.lex == Lex::Dot || scan.current_state.lex == Lex::Arrow){
            parse_identifier(scan);
        } else if (scan.current_state.lex == Lex::LSquareBrace){
            scan.get_next();
            parse_expression(scan);
            check(scan, Lex::RSquareBrace);
        } else {
            check(scan, Lex::LBrace);
            try_parse(parse_argument_expression_list, scan);
            check(scan, Lex::RBrace);
        }
    }
    throw ExpectedLexException("postfix-expression", scan.get_line_for_compiler_msg());
}

void parse_argument_expression_list(Scanner& scan){
    parse_assignment_expression(scan);
    while (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_assignment_expression(scan);
    }
}

void parse_unary_expression(Scanner& scan){
    if (try_parse(parse_postfix_expression, scan)){
        return;
    }
    if (scan.current_state.lex == Lex::Increment || scan.current_state.lex == Lex::Decrement){
        parse_unary_expression(scan);
        return;
    }
    if (try_parse(parse_unary_operator, scan)){
        parse_cast_expression(scan);
        return;
    }
    if (scan.current_state.lex == Lex::Sizeof){
        scan.get_next();
        if (scan.current_state.lex == Lex::LBrace){
            scan.get_next();
            parse_type_name(scan);
            check(scan, Lex::RBrace);
        } else {
            parse_unary_expression(scan);
        }
        return;
    }
    throw ExpectedLexException("unary expression", scan.get_line_for_compiler_msg());
}

void parse_unary_operator(Scanner& scan){
    std::vector<Lex> operations{Lex::BitwiseAnd, Lex::Asterisk, Lex::Plus, Lex::Minus, Lex::Tilde, Lex::Exclamation};
    if (std::find(operations.begin(), operations.end(), scan.current_state.lex) != operations.end()){
        scan.get_next();
    } else {
        throw ExpectedLexException("& * + - ~ !", scan.get_line_for_compiler_msg());
    }
}

void parse_cast_expression(Scanner& scan){
    if (scan.current_state.lex == Lex::LBrace){
        scan.get_next();
        parse_type_name(scan);
        check(scan, Lex::RBrace);
        parse_cast_expression(scan);
    } else {
        parse_unary_expression(scan);
    }
}

void parse_multiplicative_expression(Scanner& scan){
    parse_cast_expression(scan);
    while (scan.current_state.lex == Lex::Asterisk
        || scan.current_state.lex == Lex::Slash
        || scan.current_state.lex == Lex::Mod)
    {
        scan.get_next();
        parse_cast_expression(scan);
    }
}

void parse_additive_expression(Scanner& scan){
    parse_multiplicative_expression(scan);
    while (scan.current_state.lex == Lex::Plus || scan.current_state.lex == Lex::Minus){
        scan.get_next();
        parse_multiplicative_expression(scan);
    }
}

void parse_shift_expression(Scanner& scan){
    parse_additive_expression(scan);
    while (scan.current_state.lex == Lex::LeftShift || scan.current_state.lex == Lex::RightShift){
        scan.get_next();
        parse_additive_expression(scan);
    }
}

void parse_relational_expression(Scanner& scan){
    parse_shift_expression(scan);
    while (scan.current_state.lex == Lex::GreaterThen
        || scan.current_state.lex == Lex::LessThen
        || scan.current_state.lex == Lex::GreaterEqualThen
        || scan.current_state.lex == Lex::LessEqualThen)
    {
        scan.get_next();
        parse_shift_expression(scan);
    }
}

void parse_equality_expression(Scanner& scan){
    parse_relational_expression(scan);
    while (scan.current_state.lex == Lex::Compare || scan.current_state.lex == Lex::CompareNegative){
        scan.get_next();
        parse_relational_expression(scan);
    }
}

void parse_and_expression(Scanner& scan){
    parse_equality_expression(scan);
    while (scan.current_state.lex == Lex::BitwiseAnd){
        scan.get_next();
        parse_equality_expression(scan);
    }
}

void parse_exclusive_or_expression(Scanner& scan){
    parse_and_expression(scan);
    while (scan.current_state.lex == Lex::Xor){
        scan.get_next();
        parse_and_expression(scan);
    }
}

void parse_inclusive_or_expression(Scanner& scan){
    parse_exclusive_or_expression(scan);
    while (scan.current_state.lex == Lex::BitwiseOr){
        parse_exclusive_or_expression(scan);
    }
}

void parse_logical_and_expression(Scanner& scan){
    parse_inclusive_or_expression(scan);
    while (scan.current_state.lex == Lex::LogicalAnd){
        scan.get_next();
        parse_inclusive_or_expression(scan);
    }
}

void parse_logical_or_expression(Scanner& scan){
    parse_logical_and_expression(scan);
    while (scan.current_state.lex == Lex::LogicalOr){
        scan.get_next();
        parse_logical_and_expression(scan);
    }
}

void parse_conditional_expression(Scanner& scan){
    parse_logical_or_expression(scan);
    if (scan.current_state.lex == Lex::QuestionMark){
        scan.get_next();
        parse_expression(scan);
        check(scan, Lex::Colon);
        parse_conditional_expression(scan);
    }
}

void parse_assignment_expression(Scanner& scan){
    if (try_parse(parse_conditional_expression, scan)){
        return;
    }
    parse_unary_expression(scan);
    parse_assigment_operator(scan);
    parse_assignment_expression(scan);
}

void parse_assigment_operator(Scanner& scan){
    std::vector<Lex> ops{Lex::Assigment, Lex::AssigmentMultiply, Lex::AssigmentDivide, Lex::AssigmentMod,
        Lex::AssigmentPlus, Lex::AssigmentMinus, Lex::AssigmentLeftShift, Lex::AssigmentRightShift, Lex::AssigmentAnd,
        Lex::AssigmentXor, Lex::AssigmentOr};
    if (std::find(ops.begin(), ops.end(), scan.current_state.lex) != ops.end()){
        scan.get_next();
    } else {
        throw ExpectedLexException("= *= /= %= += -= <<= >>= &= ^= |=", scan.get_line_for_compiler_msg());
    }
}

void parse_expression(Scanner& scan){
    parse_assignment_expression(scan);
    while (scan.current_state.lex == Lex::Comma){
        parse_assignment_expression(scan);
    }
}

void parse_constant_expression(Scanner& scan){
    parse_conditional_expression(scan);
}