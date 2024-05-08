#include "scanner.h"

#include <sstream>
#include <iostream>
#include "converter.h"
#include <map>
#include "name_table.h"

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
    
    if (ScanWrapper::is_alpha(wrap->current_state.ch) || wrap->current_state.ch == '_'){
        ident();
    } else if (ScanWrapper::is_number(wrap->current_state.ch)){
        number();
    } else if (wrap->current_state.ch == '\''){
        char_literal();
    } else if (wrap->current_state.ch == '\"'){
        string_literal();
    } else if (wrap->current_state.ch == '+'){
        current_state.lex = Lex::Plus;
        wrap->get_next();
        if (wrap->current_state.ch == '+'){
            current_state.lex = Lex::Increment;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '-'){
        current_state.lex = Lex::Minus;
        wrap->get_next();
        if (wrap->current_state.ch == '-'){
            current_state.lex = Lex::Decrement;
            wrap->get_next();
        } else if (wrap->current_state.ch == '>'){
            current_state.lex = Lex::Arrow;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '/'){
        current_state.lex = Lex::Slash;
        wrap->get_next();
        if (wrap->current_state.ch == '='){
            current_state.lex = Lex::AssigmentDivide;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '|'){
        current_state.lex = Lex::BitwiseOr;
        wrap->get_next();
        if (wrap->current_state.ch == '|'){
            current_state.lex = Lex::LogicalOr;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '&'){
        current_state.lex = Lex::BitwiseAnd;
        wrap->get_next();
        if (wrap->current_state.ch == '&'){
            current_state.lex = Lex::LogicalAnd;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '='){
        current_state.lex = Lex::Assigment;
        wrap->get_next();
        if (wrap->current_state.ch == '='){
            current_state.lex = Lex::Compare;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '!'){
        current_state.lex = Lex::Exclamation;
        wrap->get_next();
        if (wrap->current_state.ch == '='){
            current_state.lex = Lex::CompareNegative;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '>'){
        current_state.lex = Lex::GreaterThen;
        wrap->get_next();
        if (wrap->current_state.ch == '='){
            current_state.lex = Lex::GreaterEqualThen;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '<'){
        current_state.lex = Lex::LessThen;
        wrap->get_next();
        if (wrap->current_state.ch == '='){
            current_state.lex = Lex::LessEqualThen;
            wrap->get_next();
        }
    } else if (wrap->current_state.ch == '.'){
        current_state.lex = Lex::Dot;
        wrap->get_next();
        if (wrap->current_state.ch == '.'){
            wrap->get_next();
            if (wrap->current_state.ch == '.'){
                current_state.lex = Lex::TripleDot;
                wrap->get_next();
            } else {
                wrap->previous();
            }
        }
    } else{
        switch (wrap->current_state.ch){
            case ScanWrapper::EOT:
                current_state.lex = Lex::EOT;
                break;
            case '(':
                current_state.lex = Lex::LBrace;
                break;
            case ')':
                current_state.lex = Lex::RBrace;
                break;
            case '{':
                current_state.lex = Lex::LCurlyBrace;
                break;
            case '}':
                current_state.lex = Lex::RCurlyBrace;
                break;
            case '[':
                current_state.lex = Lex::LSquareBrace;
                break;
            case ']':
                current_state.lex = Lex::RSquareBrace;
                break;
            case '*':
                current_state.lex = Lex::Asterisk;
                break;
            case ':':
                current_state.lex = Lex::Colon;
                break;
            case ';':
                current_state.lex = Lex::SemiColon;
                break;
            case ',':
                current_state.lex = Lex::Comma;
                break;
            case '^':
                current_state.lex = Lex::Xor;
                break;
            case '?':
                current_state.lex = Lex::QuestionMark;
                break;
            case '%':
                current_state.lex = Lex::Mod;
                break;
        }
        wrap->get_next();
    }

    return current_state.lex;
}


std::string Scanner::get_line_for_compiler_msg() const{
    std::string result = std::to_string(wrap->current_state.line_number) + ") "s + wrap->current_state.line + "\n"s;
    for (int i = 0; i < std::to_string(wrap->current_state.line_number).size() + 1 + wrap->current_state.symbol_number_in_line; ++i){
        result += " "s;
    }
    result += "^"s;
    return result; 
}


void Scanner::save_state(){
    wrap->save_state();
    states.push(current_state);
}

void Scanner::load_state(){
    if (states.size() == 0){
        throw std::exception();
    }
    wrap->load_state();
    current_state = states.top();
    states.pop();
}


void Scanner::delete_state(){
    states.pop();
}

void Scanner::check(char c){
    using namespace std::string_literals;
    if (wrap->current_state.ch != c){
        throw ExpectedLexException{""s + c, get_line_for_compiler_msg()};
    }
    wrap->get_next();
}

// первый символ уже буква или _
void Scanner::ident(){
    using namespace std::string_literals;
    current_state.lex = Lex::Ident;
    std::string str_value = std::string("");
    str_value += wrap->current_state.ch;
    wrap->get_next();
    while (ScanWrapper::is_alpha(wrap->current_state.ch) || ScanWrapper::is_number(wrap->current_state.ch) || wrap->current_state.ch == '_'){
        str_value += wrap->current_state.ch;
        wrap->get_next();
    }
    current_state.value = str_value;
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
        current_state.lex = lexIdent[str_value];
    }
    NameTableEntryType nameTableEntryType = NameTableEntryType::ScopeClose;
    if (name_table.has_name_in_this_scopes(str_value, nameTableEntryType)){
        if (nameTableEntryType == NameTableEntryType::TypeName){
            current_state.lex = Lex::UserDefinedType;
        } else if (nameTableEntryType == NameTableEntryType::EnumConst){
            current_state.lex = Lex::EnumConstant;
        }
    }
}

// первый символ уже цифра
void Scanner::number(){
    std::string num_str;
    while (ScanWrapper::is_number(wrap->current_state.ch)){
        num_str += wrap->current_state.ch;
        wrap->get_next();
    }
    if (wrap->current_state.ch == '.'){
        num_str += wrap->current_state.ch;
        wrap->get_next();
        while (ScanWrapper::is_number(wrap->current_state.ch)){
            num_str += wrap->current_state.ch;
        }
        current_state.lex = Lex::FloatLiteral;
        current_state.value = Converter::to_float(num_str);
    } else {
        current_state.lex = Lex::IntLiteral;
        current_state.value = Converter::to_int32(num_str);
    }
}

// первый символ уже '
void Scanner::char_literal(){
    using namespace std::string_literals;
    wrap->get_next();
    if (!ScanWrapper::is_alpha(wrap->current_state.ch)){
        throw ExpectedLexException{"символ\n"s, get_line_for_compiler_msg()};
    }
    if (wrap->current_state.ch == '\\'){
        current_state.value = backslash_symbol();
    } else {
        current_state.value = wrap->current_state.ch;
        wrap->get_next();
    }
    check('\'');
    current_state.lex = Lex::CharLiteral;
}

// первый символ уже "
void Scanner::string_literal(){
    using namespace std::string_literals;
    wrap->get_next();
    std::string str_value = ""s;
    while(ScanWrapper::is_alpha(wrap->current_state.ch)){
        if (wrap->current_state.ch == '\\'){
            str_value += backslash_symbol();
        } else {
            str_value += wrap->current_state.ch;
            wrap->get_next();
        }
    }
    current_state.value = str_value;
    check('\"');
    current_state.lex = Lex::StringLiteral;
}

// первый символ уже backslash
char Scanner::backslash_symbol(){
    wrap->get_next();
    char result;
    switch (wrap->current_state.ch){
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
    while (ScanWrapper::is_CRLF(wrap->current_state.ch) || ScanWrapper::is_whitespace(wrap->current_state.ch)){
        wrap->skip_whitespace();
        wrap->skip_CRLF();
    }
}

void Scanner::skip_comments(){
    while (wrap->current_state.ch == '/'){
        wrap->get_next();
        if (wrap->current_state.ch != '/' && wrap->current_state.ch != '*'){
            wrap->previous();
            return;
        }
        if (wrap->current_state.ch == '/'){
            wrap->get_next();
            while (wrap->current_state.ch != ScanWrapper::EOT && !ScanWrapper::is_CRLF(wrap->current_state.ch)){
                wrap->get_next();
            }
        } else {
            wrap->get_next();
            while (wrap->current_state.ch != ScanWrapper::EOT){
                if (wrap->current_state.ch == '*'){
                    wrap->get_next();
                    if (wrap->current_state.ch == '/'){
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
