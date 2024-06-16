#include "codegen.h"

#include <iostream>
#include <string>

using namespace std::string_literals;

std::string ast_to_str(AST* node){
    try {
        return std::to_string(std::get<int>(node->value));
    } catch (std::exception e){}
    try {
        auto op = std::get<Operation>(node->value);
        switch (op){
            case Operation::Divide: return "/"s;
            case Operation::Minus: return "-"s;
            case Operation::Plus: return "+"s;
            case Operation::Multiply: return "*"s;
            default: return "undefined"s;
        }
    } catch (std::exception e){}
    return "error"s;
}

void print_ast(AST* head, int n){
    if (!head){
        return;
    }
    for (int i = 0; i < n; ++i){
        std::cout << " ";
    }
    std::cout << ast_to_str(head) << "\n";
    //std::cout << ast_to_str(head) << "\n";
    print_ast(head->left, n + 2);
    print_ast(head->right, n + 2);
}

void generate_native_code(AST* headAstNode){
    std::cout << "====================================\n";
    print_ast(headAstNode, 0);
}