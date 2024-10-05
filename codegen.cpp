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

std::string gen_code_for_int_value(AST* ast, GenCodeSide side){
    int int_value = std::get<int>(ast->value);
    std::string register_name;
    if (side == GenCodeSide::Left){
        register_name = "rax"s;
    } else {
        register_name = "rbx"s;
    }
    return "\tmov "s + register_name + ", "s + std::to_string(int_value) + "\n"s;
}

std::string gen_code_for_operation(AST* ast, GenCodeSide side){
    Operation operation = std::get<Operation>(ast->value);
    if (operation == Operation::Plus){
        std::string command = "\tadd rax, rbx\n"s;
        if (side == GenCodeSide::Right){
            command += "\tmov rbx, rax\n"s;
        }
        return command;
    }
    if (operation == Operation::Minus){
        std::string command = "\tsub rax, rbx\n"s;
        if (side == GenCodeSide::Right){
            command += "\tmov rbx, rax\n"s;
        }
        return command;
    }
}

std::string gen_code_for_ast(AST* ast, GenCodeSide side){
    if (!ast){
        return ""s;
    }
    std::string result = ""s;
    result += gen_code_for_ast(ast->left, GenCodeSide::Left);
    result += gen_code_for_ast(ast->right, GenCodeSide::Right);
    if (std::get_if<int>(&(ast->value))){
        result += gen_code_for_int_value(ast, side);
    } else if (std::get_if<Operation>(&(ast->value))){
        result += gen_code_for_operation(ast, side);
    }
    return result;
}

void generate_native_code(AST* headAstNode){
    std::string asm_file_text = "extern printf\n";
    asm_file_text += "section .data\n\tresult_msg db \"result = %d\",10\n";
    asm_file_text += "section .text\n\tglobal main\nmain:\n";
    asm_file_text += "\tpush rbp\n\tmov rbp, rsp\n";
    asm_file_text += gen_code_for_ast(headAstNode, GenCodeSide::Left);
    asm_file_text += "\tmov rsi, rax\n";
    asm_file_text += "\tmov rdi, result_msg\n";
    asm_file_text += "\tmov rax, 0\n";
    asm_file_text += "\tcall printf\n";
    asm_file_text += "\tleave\n\tret\n\n";
    std::ofstream fout;
    fout.open("program.asm");
    if (fout.is_open()){
        fout << asm_file_text;
    }
    fout.close();
    system("nasm -f elf64 -g -F dwarf program.asm");
    system("gcc -o generated_program program.o");

}