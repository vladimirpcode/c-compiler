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
    if (ScanWrapper::is_alpha(wrap->ch)){
        ident();
    } else if (ScanWrapper::is_number(wrap->ch)){
        number();
    } else if (wrap->ch == '\''){
        char_literal();
    } else if (wrap->ch == '\"'){
        string_literal();
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
            case '=':
                lex = Lex::Assigment;
                break;
            case '+':
                lex = Lex::Plus;
                break;
            case '-':
                lex = Lex::Minus;
                break;
            case '/':
                lex = Lex::Slash;
                break;
            case '*':
                lex = Lex::Asterisk;
                break;
            case '>':
                lex = Lex::GreaterThen;
                break;
            case '<':
                lex = Lex::LessThen;
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
        }
        wrap->get_next();
    }
    while (ScanWrapper::is_CRLF(wrap->ch) || ScanWrapper::is_whitespace(wrap->ch)){
        wrap->skip_whitespace();
        wrap->skip_CRLF();
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
    lex = Lex::Ident;
    std::string str_value = std::string("");
    while (ScanWrapper::is_alpha(wrap->ch)){
        str_value += wrap->ch;
        wrap->get_next();
    }
    value = str_value;
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