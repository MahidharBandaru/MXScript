#pragma once

#include <exception>
#include <sstream>

enum class SyntaxError {
    InvalidToken,
    UnexpectedToken,
};

class SyntaxException : public std::exception {
public:
    SyntaxException(SyntaxError error, Token t)
    {
        std::stringstream ss;
        ss << "SyntaxError: " << (int)error << " Token: " << (int)t;
        m_Msg = (ss.str());
    }

    const char* what() const noexcept override {return m_Msg.c_str();}

private:
    std::string m_Msg;
};