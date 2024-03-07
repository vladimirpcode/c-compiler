#include "scanner.h"

#include <sstream>
#include <iostream>
#include "converter.h"
#include <map>

using namespace std::string_literals;

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


std::string Scanner::get_line_for_compiler_msg() const{
    std::string result = std::to_string(wrap->line_number) + ") "s + wrap->line + "\n"s;
    for (int i = 0; i < std::to_string(wrap->line_number).size() + 2 + wrap->symbol_number_in_line; ++i){
        result += " "s;
    }
    result += "^"s;
    return result; 
}

void Scanner::check(char c){
    using namespace std::string_literals;
    if (wrap->ch != c){
        throw ExpectedLexException{""s + c, wrap->line, wrap->line_number, wrap->symbol_number_in_line};
    }
    wrap->get_next();
}

// первый символ уже буква или _
void Scanner::ident(){
    using namespace std::string_literals;
    lex = Lex::Ident;
    std::string str_value = std::string("");
    while (ScanWrapper::is_alpha(wrap->ch)){
        str_value += wrap->ch;
        wrap->get_next();
    }
    value = str_value;
    std::map<std::string, Lex> lexIdent;
    lexIdent["auto"] = Lex::Auto;
    lexIdent["break"] = Lex::Break;
    lexIdent["case"] = Lex::Case;
    lexIdent["char"] = Lex::Char;
    lexIdent["const"] = Lex::Const;
    lexIdent["continue"] = Lex::Continue;
    lexIdent["default"] = Lex::Default;
    lexIdent["do"] = Lex::Do;
    lexIdent["double"] = Lex::Double;
    lexIdent["else"] = Lex::Else;
    lexIdent["enum"] = Lex::Enum;
    lexIdent["extern"] = Lex::Extern;
    lexIdent["float"] = Lex::Float;
    lexIdent["for"] = Lex::For;
    lexIdent["goto"] = Lex::Goto;
    lexIdent["if"] = Lex::If;
    lexIdent["inline"] = Lex::Inline;
    lexIdent["int"] = Lex::Int;
    lexIdent["long"] = Lex::Long;
    lexIdent["register"] = Lex::Register;
    lexIdent["restrict"] = Lex::Restrict;
    lexIdent["return"] = Lex::Return;
    lexIdent["short"] = Lex::Short;
    lexIdent["signed"] = Lex::Signed;
    lexIdent["sizeof"] = Lex::Sizeof;
    lexIdent["static"] = Lex::Static;
    lexIdent["struct"] = Lex::Struct;
    lexIdent["switch"] = Lex::Switch;
    lexIdent["typedef"] = Lex::Typedef;
    lexIdent["union"] = Lex::Union;
    lexIdent["unsigned"] = Lex::Unsigned;
    lexIdent["void"] = Lex::Void;
    lexIdent["volatile"] = Lex::Auto;
    lexIdent["while"] = Lex::Break;
    lexIdent["_Bool"] = Lex::Bool_;
    lexIdent["_Complex"] = Lex::Complex_;
    lexIdent["_Imaginary"] = Lex::Imaginary_;
    if (lexIdent.find(str_value) != lexIdent.end()){
        lex = lexIdent[str_value];
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
        lex = Lex::FloatLiteral;
        value = Converter::to_float(num_str);
    } else {
        lex = Lex::IntLiteral;
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
    lex = Lex::CharLiteral;
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
    lex = Lex::StringLiteral;
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
    std::map<Lex, std::string> lexStr;
    lexStr[Lex::EOT] = "конец текста"s;
    lexStr[Lex::StringLiteral] = "строковый литерал"s;
    lexStr[Lex::CharLiteral] = "символьный литерал"s;
    lexStr[Lex::Ident] = "идентификатор"s;
    lexStr[Lex::IntLiteral] = "целое число"s;
    lexStr[Lex::FloatLiteral] = "число с плавающей точки"s;
    lexStr[Lex::Comma] = ","s;
    lexStr[Lex::Dot] = "."s;
    lexStr[Lex::Assigment] = "="s;
    lexStr[Lex::AssigmentMultiply] = "*="s;
    lexStr[Lex::AssigmentDivide] = "/="s;
    lexStr[Lex::AssigmentMod] = "%="s;
    lexStr[Lex::AssigmentPlus] = "+="s;
    lexStr[Lex::AssigmentMinus] = "-="s;
    lexStr[Lex::AssigmentLeftShift] = "<<="s;
    lexStr[Lex::AssigmentRightShift] = ">>="s;
    lexStr[Lex::AssigmentAnd] = "&="s;
    lexStr[Lex::AssigmentXor] = "^="s;
    lexStr[Lex::AssigmentOr] = "|="s;
    lexStr[Lex::And] = "&"s;
    lexStr[Lex::Asterisk] = "*"s;
    lexStr[Lex::Plus] = "+"s;
    lexStr[Lex::Minus] = "-"s;
    lexStr[Lex::Tilde] = "~"s;
    lexStr[Lex::Exclamation] = "!"s;
    lexStr[Lex::LBrace] = "("s;
    lexStr[Lex::RBrace] = ")"s;
    lexStr[Lex::LCurlyBrace] = "{"s;
    lexStr[Lex::RCurlyBrace] = "}"s;
    lexStr[Lex::LSquareBrace] = "["s;
    lexStr[Lex::RSquareBrace] = "]"s;
    lexStr[Lex::Slash] = "/"s;
    lexStr[Lex::GreaterThen] = ">"s;
    lexStr[Lex::LessThen] = "<"s;
    lexStr[Lex::Colon] = ":"s;
    lexStr[Lex::SemiColon] = ";"s;
    lexStr[Lex::Xor] = "^"s;
    lexStr[Lex::BitwiseAnd] = "&"s;
    lexStr[Lex::BitwiseOr] = "|"s;
    lexStr[Lex::LogicalAnd] = "&&"s;
    lexStr[Lex::LogicalOr] = "||"s;
    lexStr[Lex::QuestionMark] = "?"s;
    lexStr[Lex::Compare] = "=="s;
    lexStr[Lex::CompareNegative] = "!="s;
    lexStr[Lex::LeftShift] = "<<"s;
    lexStr[Lex::RightShift] = ">>"s;
    lexStr[Lex::Mod] = "%"s;
    lexStr[Lex::Arrow] = "->"s;
    //ключевые слова
    lexStr[Lex::Auto] = "auto"s;
    lexStr[Lex::Break] = "break"s;
    lexStr[Lex::Case] = "case"s;
    lexStr[Lex::Char] = "char"s;
    lexStr[Lex::Const] = "const"s;
    lexStr[Lex::Continue] = "continue"s;
    lexStr[Lex::Default] = "default"s;
    lexStr[Lex::Do] = "do"s;
    lexStr[Lex::Double] = "double"s;
    lexStr[Lex::Else] = "else"s;
    lexStr[Lex::Enum] = "enum"s;
    lexStr[Lex::Extern] = "extern"s;
    lexStr[Lex::Float] = "float"s;
    lexStr[Lex::For] = "for"s;
    lexStr[Lex::Goto] = "goto"s;
    lexStr[Lex::If] = "if"s;
    lexStr[Lex::Inline] = "inline"s;
    lexStr[Lex::Int] = "int"s;
    lexStr[Lex::Long] = "long"s;
    lexStr[Lex::Register] = "register"s;
    lexStr[Lex::Restrict] = "restrict"s;
    lexStr[Lex::Return] = "return"s;
    lexStr[Lex::Short] = "short"s;
    lexStr[Lex::Signed] = "signed"s;
    lexStr[Lex::Sizeof] = "sizeof"s;
    lexStr[Lex::Static] = "static"s;
    lexStr[Lex::Struct] = "struct"s;
    lexStr[Lex::Switch] = "switch"s;
    lexStr[Lex::Typedef] = "typedef"s;
    lexStr[Lex::Union] = "union"s;
    lexStr[Lex::Unsigned] = "unsigned"s;
    lexStr[Lex::Void] = "void"s;
    lexStr[Lex::Volatile] = "volatile"s;
    lexStr[Lex::While] = "while"s;
    lexStr[Lex::Bool_] = "_Bool"s;
    lexStr[Lex::Complex_] = "_Complex"s;
    lexStr[Lex::Imaginary_] = "_Imaginary"s;
    if (lexStr.find(lex) != lexStr.end()){
        return lexStr[lex];
    }
    return "unknown"s;
}
