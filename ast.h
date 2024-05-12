#pragma once

#include "scanner.h"

enum class AstNodeType{
    Int,
    Float,
    String,
    Char,
    Variable,
    Pointer,
    Struct,
    Function,
    Operation,
    None,
};

enum class Operation{
    Plus,
    Minus,
    UnaryPlus,
    UnaryMinus,
    PostfixIncrement,
    PostfixDecrement,
    PrefixIncrement,
    PrefixDecrement,
    PointerDereference,
    FunctionCall,
    None
};

class AST{
public:
    AST();
    AST *left;
    AST *right;
    LexerValue value;
    AstNodeType type;
    Operation operation;
};