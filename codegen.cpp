#include "codegen.h"

#include <iostream>
#include <string>
#include <fstream>

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

void debug_ast_print(AST* headAstNode){
    std::cout << "====================================\n";
    print_ast(headAstNode, 0);
}

enum class GenCodeSide{
    Left,
    Right
};

std::string gen_code_for_ast(AST* ast, GenCodeSide side){
    if (!ast){
        return ""s;
    }
    std::string result = ""s;
    result += gen_code_for_ast(ast->left, GenCodeSide::Left);
    result += gen_code_for_ast(ast->right, GenCodeSide::Right);
    try{
        int int_value = std::get<int>(ast->value);
        std::string register_name;
        if (side == GenCodeSide::Left){
            register_name = "%rax"s;
        } else {
            register_name = "%rbx"s;
        }
        return result + "\tmovq $"s + std::to_string(int_value) + ", "s + register_name + "\n"s;
    } catch (std::exception e){}
    try{
        Operation operation = std::get<Operation>(ast->value);
        if (operation == Operation::Plus){
            std::string command = "\taddq %rbx, %rax\n"s;
            if (side == GenCodeSide::Right){
                command += "\tmovq %rax, %rbx\n"s;
            }
            return result + command;
        }
        if (operation == Operation::Minus){
            std::string command = "\tsubq %rbx, %rax\n"s;
            if (side == GenCodeSide::Right){
                command += "\tmovq %rax, %rbx\n"s;
            }
            return result + command;
        }
        
    } catch (std::exception e) {}
}

void generate_native_code(AST* headAstNode){
    std::string asm_file_text = ".global _start\n_start:\n";
    asm_file_text += gen_code_for_ast(headAstNode, GenCodeSide::Left);
    asm_file_text += "\tmovq %rax, %rdi\n";
    asm_file_text += "\tmovq $60, %rax\n";
    asm_file_text += "\tsyscall\n";
    std::ofstream fout;
    fout.open("program.asm");
    if (fout.is_open()){
        fout << asm_file_text;
    }
    fout.close();
    system("as program.asm -o program.o");
    system("ld program.o -o program");

}