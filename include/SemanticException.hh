#pragma once

#include <exception>
#include <sstream>

enum class SemanticError {
    UndeclaredIdentifier,
    UndeclaredFunction,
    UnknownOperation,
};

class SemanticException : public std::exception {
public:
    SemanticException (SemanticError error)
    {
        std::stringstream ss;
        ss << "SemanticException: " << (int)error;
        m_Msg = ss.str();
    }
    const char* what() const noexcept override {return m_Msg.c_str();}
    
private:
    std::string m_Msg;
};