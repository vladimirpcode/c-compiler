#include <iostream>
#include "scanner.h"
#include "converter.h"
#include <cstdio>
#include "parser.h"
#include <sstream>

int main(){
    std::string file_name("c-code-files/sample.c");
    std::ifstream fin(file_name);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    Scanner scan{buffer.str()};
    for (int i = 0; i < 13; ++i){
        scan.get_next();
    }
    std::cout << scan.get_line_for_compiler_msg() << "\n";
    scan.save_state();
    for (int i = 0; i < 5; ++i){
        scan.get_next();
    }
    std::cout << scan.get_line_for_compiler_msg() << "\n";
    scan.save_state();
    for (int i = 0; i < 4; ++i){
        scan.get_next();
    }
    std::cout << scan.get_line_for_compiler_msg() << "\n";
    scan.load_state();
    std::cout << scan.get_line_for_compiler_msg() << "\n";
    scan.load_state();
    std::cout << scan.get_line_for_compiler_msg() << "\n";
    return 0;
    /*
    std::string file_name("c-code-files/sample.c");
    std::ifstream fin(file_name);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    try{
        parse(buffer.str());
        std::cout << "OK\n";
    } catch (ParsingException e){
        std::cout << e.what() << "\n";
    } catch (std::exception e){
        std::cout << "неизвестная ошибка парсинга\n";
    }
    */
}