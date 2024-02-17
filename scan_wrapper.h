#pragma once

#include <string>

class ScanWrapper{
public:
    static constexpr char CR = 13u;
    static constexpr char LF = 10u;
    static constexpr char EOT = 4u;
    
    char ch;
    
    ScanWrapper(const std::string& str);
    char get_next();
    static bool is_alpha(char c);
    static bool is_number(char c);
private:
    std::string data;
    size_t index;

    bool is_whitespace(char c);
};