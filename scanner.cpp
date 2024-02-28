#include "scanner.h"

#include <sstream>
#include <iostream>
#include "converter.h"

Scanner::Scanner(const std::string& program_text){
    wrap = new ScanWrapper(program_text);
    get_next();
}

Scanner::~Scanner(){
    delete wrap;
}

Lex Scanner::get_next(){
    skip_whitespace_CLRF();
    skip_comments();
    skip_whitespace_CLRF();
    
    if (ScanWrapper::is_alpha(wrap->ch)){
        ident();
    } else if (ScanWrapper::is_number(wrap->ch)){
        number();
    } else if (wrap->ch == '\''){
        char_literal();
    } else if (wrap->ch == '\"'){
        string_literal();
    } else if (wrap->ch == '+'){
        lex = Lex::Plus;
        wrap->get_next();
        if (wrap->ch == '+'){
            lex = Lex::Increment;
            wrap->get_next();
        }
    } else if (wrap->ch == '-'){
        lex = Lex::Minus;
        wrap->get_next();
        if (wrap->ch == '-'){
            lex = Lex::Decrement;
            wrap->get_next();
        } else if (wrap->ch == '>'){
            lex = Lex::Arrow;
            wrap->get_next();
        }
    } else if (wrap->ch == '/'){
        lex = Lex::Slash;
        wrap->get_next();
        if (wrap->ch == '='){
            lex = Lex::AssigmentDivide;
            wrap->get_next();
        }
    } else if (wrap->ch == '|'){
        lex = Lex::BitwiseOr;
        wrap->get_next();
        if (wrap->ch == '|'){
            lex = Lex::LogicalOr;
            wrap->get_next();
        }
    } else if (wrap->ch == '&'){
        lex = Lex::BitwiseAnd;
        wrap->get_next();
        if (wrap->ch == '&'){
            lex = Lex::LogicalAnd;
            wrap->get_next();
        }
    } else if (wrap->ch == '='){
        lex = Lex::Assigment;
        wrap->get_next();
        if (wrap->ch == '='){
            lex = Lex::Compare;
            wrap->get_next();
        }
    } else if (wrap->ch == '!'){
        lex = Lex::Exclamation;
        wrap->get_next();
        if (wrap->ch == '='){
            lex = Lex::CompareNegative;
            wrap->get_next();
        }
    } else if (wrap->ch == '>'){
        lex = Lex::GreaterThen;
        wrap->get_next();
        if (wrap->ch == '='){
            lex = Lex::GreaterEqualThen;
            wrap->get_next();
        }
    } else if (wrap->ch == '<'){
        lex = Lex::LessThen;
        wrap->get_next();
        if (wrap->ch == '='){
            lex = Lex::LessEqualThen;
            wrap->get_next();
        }
    } else{
        switch (wrap->ch){
            case ScanWrapper::EOT:
                lex = Lex::EOT;
                break;
            case '(':
                lex = Lex::LBrace;
                break;
            case ')':
                lex = Lex::RBrace;
                break;
            case '{':
                lex = Lex::LCurlyBrace;
                break;
            case '}':
                lex = Lex::RCurlyBrace;
                break;
            case '[':
                lex = Lex::LSquareBrace;
                break;
            case ']':
                lex = Lex::RSquareBrace;
                break;
            case '*':
                lex = Lex::Asterisk;
                break;
            case ':':
                lex = Lex::Colon;
                break;
            case ';':
                lex = Lex::SemiColon;
                break;
            case '.':
                lex = Lex::Dot;
                break;
            case ',':
                lex = Lex::Comma;
                break;
            case '^':
                lex = Lex::Xor;
                break;
            case '?':
                lex = Lex::QuestionMark;
                break;
            case '%':
                lex = Lex::Mod;
                break;
        }
        wrap->get_next();
    }

    return lex;
}

void Scanner::check(char c){
    using namespace std::string_literals;
    if (wrap->ch != c){
        throw ExpectedLexException{""s + c, wrap->line, wrap->line_number, wrap->symbol_number_in_line};
    }
    wrap->get_next();
}

// первый символ уже буква
void Scanner::ident(){
    using namespace std::string_literals;
    lex = Lex::Ident;
    std::string str_value = std::string("");
    while (ScanWrapper::is_alpha(wrap->ch)){
        str_value += wrap->ch;
        wrap->get_next();
    }
    value = str_value;
    if (str_value == "void"s || str_value == "char"s
        || str_value == "short"s || str_value == "int"s
        || str_value == "long"s || str_value == "float"s
        || str_value == "double"s || str_value == "signed"s
        || str_value == "unsigned"s){
        lex = Lex::PrimitiveType;
    } else if (str_value == "auto"s || str_value == "register"s
        || str_value == "static"s || str_value == "extern"s
        || str_value == "typedef"){
        lex = Lex::TypeSpecifier;
    } else if (str_value == "struct"s || str_value == "union"s){
        lex = Lex::StructOrUnion;
    } else if (str_value == "const"s || str_value == "volatile"s){
        lex = Lex::TypeQualifier;
    } else if (str_value == "sizeof"s){
        lex = Lex::Sizeof;
    } else if (str_value == "enum"s){
        lex = Lex::Enum;
    } else if (str_value == "do"s){
        lex = Lex::Do;
    } else if (str_value == "while"s){
        lex = Lex::While;
    } else if (str_value == "for"s){
        lex = Lex::For;
    } else if (str_value == "case"s || str_value == "default"s){
        lex = Lex::LabeledStatement;
    } else if (str_value == "if"s){
        lex = Lex::If;
    } else if (str_value == "switch"s){
        lex = Lex::Switch;
    } else if (str_value == "goto"s){
        lex = Lex::Goto;
    } else if (str_value == "return"s){
        lex = Lex::Return;
    } else if (str_value == "break"s){
        lex = Lex::Break;
    } else if (str_value == "continue"s){
        lex = Lex::Continue;
    } 
}

// первый символ уже цифра
void Scanner::number(){
    std::string num_str;
    while (ScanWrapper::is_number(wrap->ch)){
        num_str += wrap->ch;
        wrap->get_next();
    }
    if (wrap->ch == '.'){
        num_str += wrap->ch;
        wrap->get_next();
        while (ScanWrapper::is_number(wrap->ch)){
            num_str += wrap->ch;
        }
        lex = Lex::FloatNumber;
        value = Converter::to_float(num_str);
    } else {
        lex = Lex::IntNumber;
        value = Converter::to_int32(num_str);
    }
}

// первый символ уже '
void Scanner::char_literal(){
    using namespace std::string_literals;
    wrap->get_next();
    if (!ScanWrapper::is_alpha(wrap->ch)){
        throw ExpectedLexException{"символ"s, wrap->line, wrap->line_number, wrap->symbol_number_in_line};
    }
    if (wrap->ch == '\\'){
        value = backslash_symbol();
    } else {
        value = wrap->ch;
        wrap->get_next();
    }
    check('\'');
    lex = Lex::Char;
}

// первый символ уже "
void Scanner::string_literal(){
    using namespace std::string_literals;
    wrap->get_next();
    std::string str_value = ""s;
    while(ScanWrapper::is_alpha(wrap->ch)){
        if (wrap->ch == '\\'){
            str_value += backslash_symbol();
        } else {
            str_value += wrap->ch;
            wrap->get_next();
        }
    }
    value = str_value;
    check('\"');
    lex = Lex::String;
}

// первый символ уже backslash
char Scanner::backslash_symbol(){
    wrap->get_next();
    char result;
    switch (wrap->ch){
        case 'n':
            result = '\n';
            break;
        case 't':
            result = '\t';
            break;
        case '0':
            result = '\0';
            break;
        default:
            result = ' ';
    }
    wrap->get_next();
    return result;
}


void Scanner::skip_whitespace_CLRF(){
    while (ScanWrapper::is_CRLF(wrap->ch) || ScanWrapper::is_whitespace(wrap->ch)){
        wrap->skip_whitespace();
        wrap->skip_CRLF();
    }
}

void Scanner::skip_comments(){
    while (wrap->ch == '/'){
        wrap->get_next();
        if (wrap->ch != '/' && wrap->ch != '*'){
            wrap->previous();
            return;
        }
        if (wrap->ch == '/'){
            wrap->get_next();
            while (wrap->ch != ScanWrapper::EOT && !ScanWrapper::is_CRLF(wrap->ch)){
                wrap->get_next();
            }
        } else {
            wrap->get_next();
            while (wrap->ch != ScanWrapper::EOT){
                if (wrap->ch == '*'){
                    wrap->get_next();
                    if (wrap->ch == '/'){
                        wrap->get_next();
                        break;
                    }
                } else {
                    wrap->get_next();
                }
            }
        }
        skip_whitespace_CLRF();
    }
}

std::string to_string(Lex lex){
    using namespace std::string_literals;
    switch (lex){
        case Lex::EOT: return "конец текста"s;
        case Lex::String: return "строковый литерал"s;
        case Lex::Char: return "символьный литерал"s;
        case Lex::Ident: return "идентификатор"s;
        case Lex::IntNumber: return "целое число"s;
        case Lex::FloatNumber: return "число с плавающей точки"s;
        case Lex::Comma: return ","s;
        case Lex::Dot: return "."s;
        case Lex::Assigment: return "="s;
        case Lex::AssigmentMultiply: return "*="s;
        case Lex::AssigmentDivide: return "/="s;
        case Lex::AssigmentMod: return "%="s;
        case Lex::AssigmentPlus: return "+="s;
        case Lex::AssigmentMinus: return "-="s;
        case Lex::AssigmentLeftShift: return "<<="s;
        case Lex::AssigmentRightShift: return ">>="s;
        case Lex::AssigmentAnd: return "&="s;
        case Lex::AssigmentXor: return "^="s;
        case Lex::AssigmentOr: return "|="s;
        case Lex::And: return "&"s;
        case Lex::Asterisk: return "*"s;
        case Lex::Plus: return "+"s;
        case Lex::Minus: return "-"s;
        case Lex::Tilde: return "~"s;
        case Lex::Exclamation: return "!"s;
        case Lex::LBrace: return "("s;
        case Lex::RBrace: return ")"s;
        case Lex::LCurlyBrace: return "{"s;
        case Lex::RCurlyBrace: return "}"s;
        case Lex::LSquareBrace: return "["s;
        case Lex::RSquareBrace: return "]"s;
        case Lex::Slash: return "/"s;
        case Lex::GreaterThen: return ">"s;
        case Lex::LessThen: return "<"s;
        case Lex::Colon: return ":"s;
        case Lex::SemiColon: return ";"s;
        case Lex::PrimitiveType: return "примитивный тип"s;
        case Lex::TypeSpecifier: return "спецификатор типа"s;
        case Lex::StructOrUnion: return "struct или union"s;
        case Lex::TypeQualifier: return "квалификатор типа"s;
        case Lex::Sizeof: return "sizeof"s;
        case Lex::Do: return "do"s;
        case Lex::While: return "while"s;
        case Lex::For: return "for"s;
        case Lex::LabeledStatement: return "case | default"s;
        case Lex::If: return "if"s;
        case Lex::Switch: return "switch"s;
        case Lex::Goto: return "goto"s;
        case Lex::Return: return "return"s; 
        case Lex::Break: return "break"s;
        case Lex::Continue: return "continue"s;
        case Lex::Xor: return "^"s;
        case Lex::BitwiseAnd: return "&"s;
        case Lex::BitwiseOr: return "|"s;
        case Lex::LogicalAnd: return "&&"s;
        case Lex::LogicalOr: return "||"s;
        case Lex::QuestionMark: return "?"s;
        case Lex::Compare: return "=="s;
        case Lex::CompareNegative: return "!="s;
        case Lex::LeftShift: return "<<"s;
        case Lex::RightShift: return ">>"s;
        case Lex::Mod: return "%"s;
        case Lex::Arrow: return "->"s;
    }
    return "unknown"s;
}
