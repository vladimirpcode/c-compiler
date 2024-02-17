#include "scan_wrapper.h"

ScanWrapper::ScanWrapper(const std::string& str) : data(str), index(0)
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
    while (is_whitespace(ch) || ch == CR || ch == LF){
        if (index == data.length()){
            ch = EOT;
            return EOT;
        }
        ch = data[index];
        index++;
    }
    return ch;
}

bool ScanWrapper::is_whitespace(char c){
    return c == ' ' || c == '\t';
}


static bool ScanWrapper::is_alpha(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool ScanWrapper::is_number(char c){
    return c >= '0' && c <= '9';
}