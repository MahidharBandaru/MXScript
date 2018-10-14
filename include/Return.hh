#pragma once

#include "Var.hh"

#include <exception>

struct ReturnException : public std::exception
{
    ReturnException (Var val)
        : m_Val (val) {}


    Var m_Val;
};