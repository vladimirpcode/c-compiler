#pragma once

#include <string>

void parse(const std::string& translation_unit_text);

class ParsingException : public std::exception{
public:
    ParsingException(const std::string& msg) : m_msg(msg){}

    virtual const char* what() const noexcept override{
        return m_msg.c_str();
    }
protected:
    const std::string m_msg;
};