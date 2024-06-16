#include "try_parse.h"

#include "../scanner.h"
#include <iostream>

AST* recursive_copy(AST* ast){
    if (!ast){
        return nullptr;
    }
    AST *ast_copy = ast_manager.get_new_ast_instance();
    ast_copy->value = ast->value;
    ast_copy->left = recursive_copy(ast->left);
    ast_copy->right = recursive_copy(ast->right);
    return ast_copy;
}

bool try_parse(parsing_function func, Scanner& scan, AST*& ast, int depth){
    AST* ast_copy;
    try{
        ast_copy = recursive_copy(ast);
        scan.save_state();
        func(scan, ast, depth + 1);
        scan.delete_state();
        return true;
    } catch (std::exception e){
        scan.load_state();
        ast = ast_copy;
        return false;
    } catch (ExpectedLexException e){
        scan.load_state();
        ast = ast_copy;
        return false;
    }
    return true;
}


bool try_parse(std::vector<parsing_function> functions, Scanner& scan, AST*& ast, int depth){
    AST* ast_copy = recursive_copy(ast);
    scan.save_state();
    bool success = true;
    for (auto& func : functions){
        success = success && try_parse(func, scan, ast, depth);
        if (!success){
            break;
        }
    }
    if (!success){
        scan.load_state();
        ast = ast_copy;
        return false;
    }
    return true;
}