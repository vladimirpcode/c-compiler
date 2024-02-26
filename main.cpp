#include <iostream>
#include "scanner.h"
#include "converter.h"
#include <cstdio>
#include "parser.h"
#include <sstream>

int main(){
    std::string file_name("sample.c");
    std::ifstream fin(file_name);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    try{
        parse(buffer.str());
        std::cout << "OK\n";
    } catch (std::exception e){
        std::cout << e.what() << "\n";
    }
}