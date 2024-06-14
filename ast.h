#pragma once

#include "scanner.h"
#include <stack>

enum class Operation{
    Plus,
    Minus,
    Multiply,
    Divide,
    DebugPrint
};

using AstNodeValue = std::variant<
    Operation,
    int
>;

class AST;

struct AstState{
    AST *left;
    AST *right;
    AstNodeValue value;
};

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
    AstNodeValue value;
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

extern AstManager ast_manager;