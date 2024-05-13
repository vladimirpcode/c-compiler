#include "ast.h"

#include "algorithm"

AST::AST()
    : left(nullptr),
    right(nullptr),
    value(0),
    type(AstNodeType::None),
    operation(Operation::None)
{
    ast_manager.add_ast_node(this);
}


AST::~AST(){
    delete left;
    delete right;
    // при ручном удалении нужно удалить указатель из списка AstManager-a
    ast_manager.remove_ast_node_from_list(this);
}


void AST::save_state(){
    AstState state;
    state.left = left;
    state.right = right;
    state.value = value;
    state.type = type;
    state.operation = operation;
}

void AST::load_state(){
    if (states.size() == 0){
        return;
    }
    delete left;
    delete right;
    AstState state = states.top();
    states.pop();
    left = state.left;
    right = state.right;
    value = state.value;
    type = state.type;
    operation = state.operation;
}


void AST::delete_state(){
    if (states.size() == 0){
        return;
    }
    states.pop();
}


AST* AstManager::get_new_ast_instance(){
    return new AST();
}

void AstManager::add_ast_node(AST* ast_pointer){
    if (!ast_pointer){
        return;
    }
    ast_pointer_list.push_back(ast_pointer);
}

void AstManager::remove_ast_node_from_list(AST* ast_pointer){
    if (!ast_pointer){
        return;
    }
    auto iter = std::find(ast_pointer_list.begin(), ast_pointer_list.end(), ast_pointer);
    if (iter == ast_pointer_list.end()){
        return;
    }
    ast_pointer_list.erase(iter);
}

void AstManager::delete_all_nodes(){
    for (auto ast_ptr : ast_pointer_list){
        delete ast_ptr;
    }
}