#include "scanner.h"

#include <sstream>
#include <iostream>

Scanner::Scanner(const std::string& file_name){
    std::ifstream fin(file_name);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    wrap = new ScanWrapper(buffer.str());
}

Scanner::~Scanner(){
    delete wrap;
}

Lex Scanner::get_next(){
    if (ScanWrapper::is_alpha(wrap->ch)){
        ident();
    } else if (ScanWrapper::is_number(wrap->ch)){
        number();
    } else{
        switch (wrap->ch){

        }
    }
}


void Scanner::ident(){

}

void Scanner::number(){

}