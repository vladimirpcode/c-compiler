#pragma once

#include <string>
#include "../scanner.h"
#include "cstdio"
#include "../ast.h"

AST* parse(const std::string& translation_unit_text);
void check(Scanner& scan, Lex lex);
void error(const Scanner& scan, const std::string& msg);
void expected(const Scanner& scan, const std::string& expected_value, const std::string& actual_value);

class ParsingException : public std::exception{
public:
    ParsingException(const std::string& msg) : m_msg(msg){

    }
    virtual const char* what() const noexcept override{
        return m_msg.c_str();
    }
protected:
    const std::string m_msg;
};

class Tree{
public:
    Tree *left;
    Tree *right;
    Lex operation_or_value_type;
    LexerValue value;
};

void dbg_print(int n, const char* s);

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(n, s) (dbg_print(n, s))
#else 
#define DEBUG_PRINT(s)
#endif