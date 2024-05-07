#pragma once

#include <string>
#include <stack>

struct ScanWrapperState{
    char ch;
    std::string line;
    size_t line_number;
    size_t symbol_number_in_line;
    size_t index;
};

class ScanWrapper{
public:
    static constexpr char CR = 13u;
    static constexpr char LF = 10u;
    static constexpr char EOT = 4u;
    
    ScanWrapperState current_state;
    
    ScanWrapper(const std::string& str);
    char get_next();
    char previous();
    void skip_whitespace();
    void skip_CRLF();
    void save_state();
    void load_state();

    static bool is_alpha(char c);
    static bool is_number(char c);
    static bool is_whitespace(char c);
    static bool is_CRLF(char c);
private:
    std::string data;
    std::stack<ScanWrapperState> states;
};