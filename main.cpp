#include <iostream>
#include "scanner.h"
#include "converter.h"
#include <cstdio>
#include "parser/parser.h"
#include <sstream>
#include "codegen.h"

int main(){
    
    std::string file_name("c-code-files/sample_for_gen.c");
    std::ifstream fin(file_name);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    try{
        AST* headAstNode = parse(buffer.str());
        generate_native_code(headAstNode);
        std::cout << "\n\n";
        std::cout << "#######################\n";
        std::cout << "####### УСПЕШНО #######\n";
        std::cout << "#######################\n";
    } catch (ParsingException e){
        std::cout << e.what() << "\n";
    } catch (ExpectedLexException e){
        std::cout << e.what() << "\n";
    } catch (std::exception e){
        std::cout << "неизвестная ошибка парсинга\n";
    }
    ast_manager.delete_all_nodes();
}