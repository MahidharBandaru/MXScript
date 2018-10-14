#pragma once

#include <exception>
#include <sstream>

enum class Type;
class TypeException : public std::exception
{
public:
    TypeException (Type t1, Type t2)
    {
        std::stringstream ss;
        ss << "Types don't fit " << (int)t1 << " and " << (int)t2;
        m_Msg = ss.str();
    }
    char const* what() const noexcept override {return m_Msg.c_str();}

private:
    std::string m_Msg;
};