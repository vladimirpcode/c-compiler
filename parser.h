#pragma once

#include <string>

void parse(const std::string& translation_unit_text);

class ParsingException : public std::exception{
public:
    ParsingException(const std::string& msg);
    virtual const char* what() const noexcept override;
protected:
    const std::string m_msg;
};