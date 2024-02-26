#include "parser.h"

#include "scanner.h"
#include <string>
#include <iostream>


using namespace std::string_literals;

enum class IdentTokenType{
    JumpStatement,
    IterationStatement,
    SelectionStatement,
    LabeledStatement,
    EnumSpecifier,
    UnaryExpression,
    TypeQualifier,
    StructOrUnion,
    TypeSpecifier,
    StorageClassSpecifier,
    Another
};

IdentTokenType get_ident_type(std::string ident){
    using namespace std::string_literals;
    if (ident == "goto"s || ident == "continue"s 
        || ident == "break"s || ident == "return"s){
        return IdentTokenType::JumpStatement;
    }  else if (ident == "while"s || ident == "do"s 
        || ident == "for"s){
        return IdentTokenType::IterationStatement;
    } else if (ident == "if"s || ident == "switch"s){
        return IdentTokenType::SelectionStatement;
    } else if (ident == "case"s || ident == "default"s){
        return IdentTokenType::LabeledStatement;
    } else if (ident == "enum"s){
        return IdentTokenType::EnumSpecifier;
    } else if (ident == "sizeof"s){
        return IdentTokenType::UnaryExpression;
    } else if (ident == "const"s || ident == "volatile"s){
        return IdentTokenType::TypeQualifier;
    } else if (ident == "struct"s || ident == "union"s){
        return IdentTokenType::StructOrUnion;
    } else if (ident == "void"s || ident == "char"s
        || ident == "short"s || ident == "int"s
        || ident == "long"s || ident == "float"s
        || ident == "double"s || ident == "signed"s
        || ident == "unsigned"){
        return IdentTokenType::TypeSpecifier;
    } else if (ident == "auto"s || ident == "register"s
        || ident == "static"s || ident == "extern"s
        || ident == "typedef"){
        return IdentTokenType::StorageClassSpecifier;
    }

    return IdentTokenType::Another;
}

void check(Scanner& scan, Lex lex){
    if (scan.lex == lex){
        scan.get_next();
    } else {
        throw ParsingException("expected ..."s);
    }
}

void check(Scanner& scan, IdentTokenType identTokenType){
    if (scan.lex != Lex::Ident){
        throw ParsingException(""s);
    }
    std::string name = std::get<std::string>(scan.value);
    if (get_ident_type(name) != identTokenType){
        throw ParsingException(""s);
    }
    scan.get_next();
}

void parse_function_parameters(Scanner& scan){
    if (scan.lex != Lex::Ident){
        throw ParsingException(""s);
    }

    std::string name = std::get<std::string>(scan.value);
    if (name == "void"s){
        return;
    }
    check(scan, IdentTokenType::TypeSpecifier);
    if (scan.lex == Lex::Asterisk){
        scan.get_next();
    }
    check(scan, IdentTokenType::Another);
    while (scan.lex == Lex::Comma){
        scan.get_next();
        check(scan, IdentTokenType::TypeSpecifier);
        if (scan.lex == Lex::Asterisk){
            scan.get_next();
        }
        check(scan, IdentTokenType::Another);
    }

}

void parse_function_body(Scanner& scan){

}

void parse_expression(Scanner& scan){

}

void parse(const std::string& translation_unit_text){
    Scanner scan(translation_unit_text);
    // функция или глобальная переменная
    check(scan, IdentTokenType::TypeSpecifier);
    check(scan, IdentTokenType::Another);
    if (scan.lex == Lex::LBrace){
        // функция
        scan.get_next();
        parse_function_parameters(scan);
        check(scan, Lex::RBrace);
        check(scan, Lex::LCurlyBrace);
        parse_function_body(scan);
        check(scan, Lex::RCurlyBrace);

    } else {
        // глобальная переменная
    }

}
