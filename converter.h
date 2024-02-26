#pragma once

#include <string>

class Converter{
public:
    static int32_t to_int32(std::string str);
    static float to_float(std::string str);
};

class BadNumberException{};