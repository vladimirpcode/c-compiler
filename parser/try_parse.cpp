#include "try_parse.h"

#include "../scanner.h"
#include <iostream>

bool try_parse(parsing_function func, Scanner& scan, AST*& ast){
    try{
        ast->save_state();
        scan.save_state();
        func(scan, ast);
        scan.delete_state();
        ast->delete_state();
        return true;
    } catch (std::exception e){
        scan.load_state();
        ast->load_state();
        return false;
    } catch (ExpectedLexException e){
        scan.load_state();
        ast->load_state();
        return false;
    }
    return true;
}
