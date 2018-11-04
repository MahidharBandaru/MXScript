#pragma once

#include <exception>
#include <sstream>

enum class SyntaxError {
    InvalidToken,
    UnexpectedToken,
};

class SyntaxException : public std::exception {
public:
    SyntaxException(std::stringstream & ss, std::pair <size_t, size_t> cursor)
    {
        m_Msg = (ss.str());
    }

    const char* what() const noexcept override {return m_Msg.c_str();}

private:
    std::string m_Msg;
};