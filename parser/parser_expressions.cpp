#include "parser_expressions.h"

#include "try_parse.h"
#include "parser_others.h"
#include "parser_declarations.h"
#include <algorithm>

void parse_primary_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_primary_expression\n");
    if (try_parse(parse_identifier, scan, ast)){

    } else if (scan.current_state.lex == Lex::StringLiteral){
        scan.get_next();
    } else if (try_parse(parse_constant, scan, ast)){
       
    } else if (scan.current_state.lex == Lex::LBrace){
        scan.get_next();
        parse_expression(scan, ast);
        check(scan, Lex::RBrace);
    } else {
        throw ExpectedLexException("идентификатор, константа, строковый литерал или ( выражение )", scan.get_line_for_compiler_msg());
    }

}

void parse_postfix_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_postfix_expression\n");
    if (try_parse(parse_primary_expression, scan, ast)){
        
    } else if (scan.current_state.lex == Lex::LBrace){
        scan.get_next();
        parse_type_name(scan, ast);
        check(scan, Lex::RBrace);
        check(scan, Lex::LCurlyBrace);
        parse_initializer_list(scan, ast);
        if (scan.current_state.lex == Lex::Comma){
            scan.get_next();
        }
        check(scan, Lex::RCurlyBrace);
    } else {
        throw ExpectedLexException("postfix-expression", scan.get_line_for_compiler_msg());
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
            scan.get_next();
            parse_identifier(scan, ast);
        } else if (scan.current_state.lex == Lex::LSquareBrace){
            scan.get_next();
            parse_expression(scan, ast);
            check(scan, Lex::RSquareBrace);
        } else {
            check(scan, Lex::LBrace);
            try_parse(parse_argument_expression_list, scan, ast);
            check(scan, Lex::RBrace);
        }
    }
    
}

void parse_argument_expression_list(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_argument_expression_list\n");
    parse_assignment_expression(scan, ast);
    while (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_assignment_expression(scan, ast);
    }
}

void parse_unary_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_unary_expression\n");
    if (scan.current_state.lex == Lex::Increment || scan.current_state.lex == Lex::Decrement){
        scan.get_next();
        parse_unary_expression(scan, ast);
    } else if (scan.current_state.lex == Lex::Sizeof){
        scan.get_next();
        if (scan.current_state.lex == Lex::LBrace){
            scan.get_next();
            parse_type_name(scan, ast);
            check(scan, Lex::RBrace);
            return;
        } else {
            parse_unary_expression(scan, ast);
        }
    } else if (try_parse(parse_postfix_expression, scan, ast)){
    
    } else if (try_parse(parse_unary_operator, scan, ast)){
        if (!try_parse(parse_cast_expression, scan, ast)){
            throw ExpectedLexException("unary expression", scan.get_line_for_compiler_msg());
        }
    }  else {
        throw ExpectedLexException("unary expression", scan.get_line_for_compiler_msg());
    }
 }

void parse_unary_operator(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_unary_operator\n");
    std::vector<Lex> operations{Lex::BitwiseAnd, Lex::Asterisk, Lex::Plus, Lex::Minus, Lex::Tilde, Lex::Exclamation};
    if (std::find(operations.begin(), operations.end(), scan.current_state.lex) != operations.end()){
        scan.get_next();
    } else {
        throw ExpectedLexException("& * + - ~ !", scan.get_line_for_compiler_msg());
    }
}

void parse_cast_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_cast_expression\n");
    if (scan.current_state.lex == Lex::LBrace){
        scan.get_next();
        parse_type_name(scan, ast);
        check(scan, Lex::RBrace);
        parse_cast_expression(scan, ast);
    } else {
        try_parse(parse_unary_expression, scan, ast);
    }
}

void parse_multiplicative_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_multiplicative_expression\n");
    parse_cast_expression(scan, ast);
    while (scan.current_state.lex == Lex::Asterisk
        || scan.current_state.lex == Lex::Slash
        || scan.current_state.lex == Lex::Mod)
    {
        scan.get_next();
        parse_cast_expression(scan, ast);
    }
}

void parse_additive_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_additive_expression\n");
    parse_multiplicative_expression(scan, ast);
    try{
        int v = std::get<int>(ast->value);
        std::cout << "VOVA " << std::to_string(v) << "\n";}
    catch (std::exception e) {}
    while (scan.current_state.lex == Lex::Plus || scan.current_state.lex == Lex::Minus){
        AST *tmp = ast;
        ast = ast_manager.get_new_ast_instance();
        ast->value = Operation::Minus;
        if (scan.current_state.lex == Lex::Plus){
            ast->value = Operation::Plus;
        }
        ast->left = tmp;
        scan.get_next();
        parse_multiplicative_expression(scan, ast->left);
    }
}

void parse_shift_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_shift_expression\n");
    parse_additive_expression(scan, ast);
    while (scan.current_state.lex == Lex::LeftShift || scan.current_state.lex == Lex::RightShift){
        scan.get_next();
        parse_additive_expression(scan, ast);
    }
}

void parse_relational_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_relational_expression\n");
    parse_shift_expression(scan, ast);
    while (scan.current_state.lex == Lex::GreaterThen
        || scan.current_state.lex == Lex::LessThen
        || scan.current_state.lex == Lex::GreaterEqualThen
        || scan.current_state.lex == Lex::LessEqualThen)
    {
        scan.get_next();
        parse_shift_expression(scan, ast);
    }
}

void parse_equality_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_equality_expression\n");
    parse_relational_expression(scan, ast);
    while (scan.current_state.lex == Lex::Compare || scan.current_state.lex == Lex::CompareNegative){
        scan.get_next();
        parse_relational_expression(scan, ast);
    }
}

void parse_and_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_and_expression\n");
    parse_equality_expression(scan, ast);
    while (scan.current_state.lex == Lex::BitwiseAnd){
        scan.get_next();
        parse_equality_expression(scan, ast);
    }
}

void parse_exclusive_or_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_exclusive_or_expression\n");
    parse_and_expression(scan, ast);
    while (scan.current_state.lex == Lex::Xor){
        scan.get_next();
        parse_and_expression(scan, ast);
    }
}

void parse_inclusive_or_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_inclusive_or_expression\n");
    parse_exclusive_or_expression(scan, ast);
    while (scan.current_state.lex == Lex::BitwiseOr){
        parse_exclusive_or_expression(scan, ast);
    }
}

void parse_logical_and_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_logical_and_expression\n");
    parse_inclusive_or_expression(scan, ast);
    while (scan.current_state.lex == Lex::LogicalAnd){
        scan.get_next();
        parse_inclusive_or_expression(scan, ast);
    }
}

void parse_logical_or_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_logical_or_expression\n");
    parse_logical_and_expression(scan, ast);
    while (scan.current_state.lex == Lex::LogicalOr){
        scan.get_next();
        parse_logical_and_expression(scan, ast);
    }
}

void parse_conditional_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_conditional_expression\n");
    parse_logical_or_expression(scan, ast);
    if (scan.current_state.lex == Lex::QuestionMark){
        scan.get_next();
        parse_expression(scan, ast);
        check(scan, Lex::Colon);
        parse_conditional_expression(scan, ast);
    }
}

void parse_assignment_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_assignment_expression\n");
    scan.save_state();
    if (try_parse(parse_unary_expression, scan, ast)){
        if (!try_parse(parse_assigment_operator, scan, ast)){
            scan.load_state();
            parse_conditional_expression(scan, ast);
        } else {
            scan.delete_state();
            parse_assignment_expression(scan, ast);
        }
    } else {
        scan.load_state();
        parse_conditional_expression(scan, ast);
    }
}

void parse_assigment_operator(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_assigment_operator\n");
    std::vector<Lex> ops = {Lex::Assigment, Lex::AssigmentMultiply, Lex::AssigmentDivide, Lex::AssigmentMod,
        Lex::AssigmentPlus, Lex::AssigmentMinus, Lex::AssigmentLeftShift, Lex::AssigmentRightShift, Lex::AssigmentAnd,
        Lex::AssigmentXor, Lex::AssigmentOr};
    if (std::find(ops.begin(), ops.end(), scan.current_state.lex) != ops.end()){
        scan.get_next();
    } else {
        throw ExpectedLexException("= *= /= %= += -= <<= >>= &= ^= |=", scan.get_line_for_compiler_msg());
    }
}

void parse_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_expression\n");
    parse_assignment_expression(scan, ast);
    while (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_assignment_expression(scan, ast);
    }
}

void parse_constant_expression(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_constant_expression\n");
    parse_conditional_expression(scan, ast);
}