#pragma once

#include <string>
#include <fstream>
#include "scan_wrapper.h"

enum class Lex{
    Ident,
    IntNumber,
    FloatNumber,
};

class Scanner{
public:
    Lex lex;
    std::string name;
    int int_value;
    float float_value;

    Scanner(const std::string& file_name);
    ~Scanner();
    Lex get_next();
    void ident();
    void number();
private:
    ScanWrapper* wrap;
};