#include "parser_statements.h"

#include "try_parse.h"
#include "parser_others.h"
#include "parser_expressions.h"
#include "parser_declarations.h"

void parse_statement(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_statement\n");
    if (try_parse(parse_labeled_statement, scan, ast)){

    } else if (try_parse(parse_compound_statement, scan, ast)){

    } else if (try_parse(parse_expression_statement, scan, ast)){

    } else if (try_parse(parse_selection_statement, scan, ast)){

    } else if (try_parse(parse_iteration_statement, scan, ast)){

    } else if (try_parse(parse_jump_statement, scan, ast)){

    } else {
        throw ExpectedLexException("statement", scan.get_line_for_compiler_msg());
    }
}

void parse_labeled_statement(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_labeled_statement\n");
    if (scan.current_state.lex == Lex::Case){
        std::cout << "sad: " << to_string(scan.current_state.lex) << "\n\n";
        scan.get_next();
        parse_constant_expression(scan, ast);
        check(scan, Lex::Colon);
        parse_statement(scan, ast);
    } else if (scan.current_state.lex == Lex::Default){
        scan.get_next();
        check(scan, Lex::Colon);
        parse_statement(scan, ast);
    } else {
        parse_identifier(scan, ast);
        check(scan, Lex::Colon);
        parse_statement(scan, ast);
    }
}

void parse_compound_statement(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_compound_statement\n");
    check(scan, Lex::LCurlyBrace);
    try_parse(parse_block_item_list, scan, ast);
    check(scan, Lex::RCurlyBrace);
}

void parse_block_item_list(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_block_item_list\n");
    parse_block_item(scan, ast);
    if (scan.current_state.lex == Lex::RCurlyBrace){
        //иначе в бесконечный цикл уходит
        return;
    }
    while (try_parse(parse_block_item, scan, ast)){
        if (scan.current_state.lex == Lex::RCurlyBrace){
            //иначе в бесконечный цикл уходит
            return;
        }
    }
}

void parse_block_item(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_block_item\n");
    if (try_parse(parse_declaration, scan, ast)){
        
    } else {
        parse_statement(scan, ast);
    }
}

void parse_expression_statement(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_expression_statement\n");
    try_parse(parse_expression, scan, ast);
    check(scan, Lex::SemiColon);
}

void parse_selection_statement(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_selection_statement\n");
    if (scan.current_state.lex == Lex::If){
        scan.get_next();
        check(scan, Lex::LBrace);
        parse_expression(scan, ast);
        check(scan, Lex::RBrace);
        parse_statement(scan, ast);
        if (scan.current_state.lex == Lex::Else){
            scan.get_next();
            parse_statement(scan, ast);
        }
    } else {
        check(scan, Lex::Switch);
        check(scan, Lex::LBrace);
        parse_expression(scan, ast);
        check(scan, Lex::RBrace);
        parse_statement(scan, ast);
    }
}

void parse_iteration_statement(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_iteration_statement\n");
    if (scan.current_state.lex == Lex::While){
        scan.get_next();
        check(scan, Lex::LBrace);
        parse_expression(scan, ast);
        check(scan, Lex::RBrace);
        parse_statement(scan, ast);
    } else if (scan.current_state.lex == Lex::Do){
        scan.get_next();
        parse_statement(scan, ast);
        check(scan, Lex::While);
        check(scan, Lex::LBrace);
        parse_expression(scan, ast);
        check(scan, Lex::RBrace);
        check(scan, Lex::SemiColon);
    } else {
        check(scan, Lex::For);
        check(scan, Lex::LBrace);
        if (try_parse(parse_declaration, scan, ast)){
            try_parse(parse_expression, scan, ast);
            check(scan, Lex::SemiColon);
            try_parse(parse_expression, scan, ast);
            check(scan, Lex::RBrace);
            parse_statement(scan, ast);
        } else {
            try_parse(parse_expression, scan, ast);
            check(scan, Lex::SemiColon);
            try_parse(parse_expression, scan, ast);
            check(scan, Lex::SemiColon);
            try_parse(parse_expression, scan, ast);
            check(scan, Lex::RBrace);
            parse_statement(scan, ast);
        }
    }
}

void parse_jump_statement(Scanner& scan, AST*& ast){
    DEBUG_PRINT("parse_jump_statement\n");
    if (scan.current_state.lex == Lex::Goto){
        scan.get_next();
        parse_identifier(scan, ast);
        check(scan, Lex::SemiColon);
    } else if (scan.current_state.lex == Lex::Continue){
        scan.get_next();
        check(scan, Lex::SemiColon);
    } else if (scan.current_state.lex == Lex::Break){
        scan.get_next();
        check(scan, Lex::SemiColon);
    } else if (scan.current_state.lex == Lex::Return){
        scan.get_next();
        try_parse(parse_expression, scan, ast);
        check(scan, Lex::SemiColon);
    } else {
        throw ExpectedLexException("jump statement", scan.get_line_for_compiler_msg());
    }
}
