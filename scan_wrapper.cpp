#include "scan_wrapper.h"

ScanWrapper::ScanWrapper(const std::string& str) 
    : data(str)
{
    current_state.line_number = 1;
    current_state.symbol_number_in_line = 0;
    current_state.index = 0;
    get_next();
}

char ScanWrapper::get_next(){
    if (current_state.index == data.length()){
        current_state.ch = EOT;
        return EOT;
    }
    current_state.ch = data[current_state.index];
    current_state.index++;
    current_state.symbol_number_in_line++;
    current_state.line += current_state.ch;
    if (current_state.ch == LF){
        current_state.line_number++;
        current_state.symbol_number_in_line = 0;
        current_state.line = std::string("");
    }
    return current_state.ch;
}

char ScanWrapper::previous(){
    if (current_state.index == 1u){
        return current_state.ch;
    }
    current_state.ch = data[current_state.index-2u];
    current_state.index--;
    return current_state.ch;
}

void ScanWrapper::skip_whitespace(){
    while (is_whitespace(current_state.ch)){
        get_next();
    }
}

void ScanWrapper::skip_CRLF(){
    while (current_state.ch == CR || current_state.ch == LF){
        get_next();
    }
}


void ScanWrapper::save_state(){
    states.push(current_state);
}

void ScanWrapper::load_state(){
    if (states.size() == 0){
        throw std::exception();
    }
    current_state = states.top();
    states.pop();
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