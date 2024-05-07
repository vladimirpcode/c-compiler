#include "try_parse.h"

bool try_parse(parsing_function func, Scanner scan){
    try{
        scan.save_state();
        func(scan);
        scan.delete_state();
    } catch (std::exception* e){
        scan.load_state();
    }
}