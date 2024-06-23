#include "ast.h"

#include "algorithm"

AST::AST()
    : left(nullptr),
    right(nullptr),
    value(0)
{
}


AST::~AST(){

}

AST* AstManager::get_new_ast_instance(){
    AST* ast = new AST();
    ast_pointer_list.push_back(ast);
    return ast;
}

void AstManager::delete_all_nodes(){
    for (auto ast_ptr : ast_pointer_list){
        delete ast_ptr;
    }
    ast_pointer_list.clear();
}

AstManager ast_manager;