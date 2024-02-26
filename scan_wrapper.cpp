#include "scan_wrapper.h"

ScanWrapper::ScanWrapper(const std::string& str) 
    : data(str), 
    index(0),
    line_number(1),
    symbol_number_in_line(0)
{
    get_next();
}

char ScanWrapper::get_next(){
    if (index == data.length()){
        ch = EOT;
        return EOT;
    }
    ch = data[index];
    index++;
    symbol_number_in_line++;
    line += ch;
    if (ch == LF){
        line_number++;
        symbol_number_in_line = 0;
        line = std::string("");
    }
    return ch;
}


void ScanWrapper::skip_whitespace(){
    while (is_whitespace(ch)){
        get_next();
    }
}

void ScanWrapper::skip_CRLF(){
    while (ch == CR || ch == LF){
        get_next();
    }
}

bool ScanWrapper::is_whitespace(char c){
    return c == ' ' || c == '\t';
}


bool ScanWrapper::is_CRLF(char c){
    return c == CR || c == LF;
}


bool ScanWrapper::is_alpha(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool ScanWrapper::is_number(char c){
    return c >= '0' && c <= '9';
}