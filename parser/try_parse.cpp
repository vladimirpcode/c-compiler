#include "try_parse.h"

#include "../scanner.h"
#include <iostream>

bool try_parse(parsing_function func, Scanner& scan, AST*&){
    try{
        scan.save_state();
        func(scan);
        scan.delete_state();
        return true;
    } catch (std::exception e){
        scan.load_state();
        return false;
    } catch (ExpectedLexException e){
        scan.load_state();
        return false;
    }
    return true;
}
