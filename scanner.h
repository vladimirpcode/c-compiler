#pragma once

#include <string>
#include <fstream>
#include "scan_wrapper.h"
#include <variant>

enum class Lex;

using LexerValue = std::variant<std::string, char, int, float>;

class Scanner{
public:
    Lex lex;
    LexerValue value;
    Scanner(const std::string& program_text);
    ~Scanner();
    Lex get_next();
    std::string get_line_for_compiler_msg() const;
private:
    ScanWrapper* wrap;
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
    BadLexException(const std::string& msg) : m_msg(msg){}

    virtual const char* what() const noexcept override{
        return m_msg.c_str();
    }
protected:
    const std::string m_msg;
};

class ExpectedLexException : public BadLexException{
public:
    ExpectedLexException(const std::string& expected_msg, const std::string line, size_t line_number, size_t char_pos) 
        : BadLexException(expected_msg),
        m_line(line),
        m_line_number(line_number),
        m_char_pos(char_pos){}

    virtual const char* what() const noexcept override{
        using namespace std::string_literals;
        std::string msg = "в строке "s + std::to_string(m_line_number) + ":\n"s;
        msg += m_line;
        for (size_t i = 2; i <= m_char_pos; ++i){
            msg += " "s;
        }
        msg += "^";
        return msg.c_str();
    }
private:
    const std::string m_line;
    const size_t m_line_number;
    const size_t m_char_pos;
};

enum class Lex{
    EOT, // конец текста
    String,
    Char,
    Ident,

    IntNumber,
    FloatNumber,

    Comma, // ,
    Dot, // .

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

    And, // &
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
    //QuotationMark, // '
    //DoubleQuotationMark, // "
    Colon, // :
    SemiColon, // ;
    Arrow, // ->

    PrimitiveType, // int, float, etc...
    TypeSpecifier, // auto, static, etc...
    StructOrUnion, // struct | union
    TypeQualifier, // const | volatile
    Sizeof,
    Enum,
    While,
    For,
    Do,
    LabeledStatement, //case | default
    If,
    Switch,
    Goto,
    Return,
    Break,
    Continue,

    Increment, // ++
    Decrement, // --
};

std::string to_string(Lex lex);