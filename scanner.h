#pragma once

#include <string>
#include <fstream>
#include "scan_wrapper.h"
#include <variant>
#include <stack>
#include <vector>
#include <iostream>

enum class Lex;

using LexerValue = std::variant<std::string, char, int, float>;

struct ScannerState{
    Lex lex;
    LexerValue value;
};

class Scanner{
public:
    ScannerState current_state;
    Scanner(const std::string& program_text);
    ~Scanner();
    Lex get_next();
    std::string get_line_for_compiler_msg() const;
    void save_state();
    void load_state();
    void delete_state();
private:
    ScanWrapper* wrap;
    std::stack<ScannerState> states;
    void check(char c);
    void ident();
    void number();
    void char_literal();
    void string_literal();
    char backslash_symbol();
    void skip_whitespace_CLRF();
    void skip_comments();
};

class BadLexException : public std::exception{
public:
    BadLexException(const std::string& msg) : m_msg(msg){
        
    }

    virtual const char* what() const noexcept override{
        return m_msg.c_str();
    }
protected:
    const std::string m_msg;
};

class ExpectedLexException : public BadLexException{
public:
    ExpectedLexException(const std::string& expected_msg, const std::string& context_msg) 
        : BadLexException(expected_msg), m_context_msg(context_msg) 
    {
        using namespace std::string_literals;
        m_what_msg = ("Ожидалось:  "s +  m_msg.c_str() + "\n" + m_context_msg);
    }

    virtual const char* what() const noexcept override{
        return m_what_msg.c_str();
    }
private:
    std::string m_context_msg;
    std::string m_what_msg;
};

enum class Lex{
    EOT, // конец текста
    StringLiteral,
    CharLiteral,
    Ident,
    IntLiteral,
    FloatLiteral,
    EnumConstant,

    Comma, // ,
    Dot, // .
    TripleDot, // ...
    Assigment,  // =
    AssigmentMultiply, // *=
    AssigmentDivide, // /=
    AssigmentMod, // %=
    AssigmentPlus, // +=
    AssigmentMinus, // -=
    AssigmentLeftShift, // <<=
    AssigmentRightShift, // >>=
    AssigmentAnd, // &=
    AssigmentXor, // ^=
    AssigmentOr, // |=
    Compare, // ==
    CompareNegative, // !=
    Asterisk, // *
    Plus,
    Minus,
    Tilde, // ~
    Exclamation, // !
    QuestionMark, // ?
    Xor,    // ^
    BitwiseAnd, // &
    BitwiseOr,  // |
    LogicalAnd, // &&
    LogicalOr,  // ||
    LeftShift, // <<
    RightShift, // >>
    LBrace, // (
    RBrace, // )
    LCurlyBrace, // {
    RCurlyBrace, // }
    LSquareBrace, // [
    RSquareBrace, // ]
    Slash, // /
    Mod, // %
    GreaterThen, // >
    LessThen, // <
    GreaterEqualThen, // >=
    LessEqualThen, // <=
    Colon, // :
    SemiColon, // ;
    Arrow, // ->
    Increment, // ++
    Decrement, // --

    // keywords ключевые слова
    Auto,
    Break,
    Case,
    Char,
    Const,
    Continue,
    Default,
    Do,
    Double,
    Else,
    Enum,
    Extern,
    Float,
    For,
    Goto,
    If,
    Inline,
    Int,
    Long,
    Register,
    Restrict,
    Return,
    Short,
    Signed,
    Sizeof,
    Static,
    Struct,
    Switch,
    Typedef,
    Union,
    Unsigned,
    Void,
    Volatile,
    While,
    Bool_,
    Complex_,
    Imaginary_,

    UserDefinedType, // for lexer hack
};

std::string to_string(Lex lex);