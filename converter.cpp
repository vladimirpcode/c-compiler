#include "converter.h"

#include <cstdio>

int32_t Converter::to_int32(std::string str){
    int32_t result = 0;
    for (char c : str){
        if (!(c >= '0' && c <= '9')){
            throw BadNumberException{};
        }
        result = result * 10 + c - '0';
    }
    return result;
}

float Converter::to_float(std::string str){
    float result = 0;
    bool int_part = true;
    float divider = 10;
    for (char c : str){
        if (c == '.'){
            int_part = false;
            continue;
        }
        if (!(c >= '0' && c <= '9')){
            throw BadNumberException{};
        }
        if (int_part){
            result = result * 10 + c - '0';
        } else {
            result += static_cast<float>(c - '0') / divider;
            printf("%d\t%f\n", c - '0', divider);
            divider *= 10.0;
        }
    }
    return result;
}