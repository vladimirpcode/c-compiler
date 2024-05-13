#pragma once

#include "scanner.h"
#include <stack>

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

struct AstState{
    AST *left;
    AST *right;
    LexerValue value;
    AstNodeType type;
    Operation operation;
};

enum class CType{
    Int,
    Float,
    String,
    Char,
    Pointer,
};

struct FunctionDescription{

};

using AstNodeValue = std::variant<std::string, char, int, float, FunctionDescription>;

class AstManager;

class AST{
    friend class AstManager;
public:
    ~AST();
    void save_state();
    void load_state();
    void delete_state();
    AST *left;
    AST *right;
    LexerValue value;
    AstNodeType type;
    Operation operation;
private:
    AST();
    std::stack<AstState> states;
};

// не уверен, что откат состояний с удалением ветвей будет безболезненным
// поэтому все вершины дерева будут регистрироваться тут и удаляться одним методом
// вместо сборщика мусора
class AstManager{
public:
    AST* get_new_ast_instance();
    friend AST::AST();
    void add_ast_node(AST* ast_pointer);
    void remove_ast_node_from_list(AST* ast_pointer);
    void delete_all_nodes();
private:
    std::vector<AST*> ast_pointer_list;
};

static AstManager ast_manager;