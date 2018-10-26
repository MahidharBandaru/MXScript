#pragma once

#include "Env.hh"
#include "Expr.hh"

#include <memory>
#include <map>
#include <string>

struct Var;
struct StructDecl;

struct Object
{
    Object (Struct * callable)
        : m_Callable (callable) {}

    Var call (Interpreter * i, FuncCallExpr * func_call_expr)
    {
        Callable * c = m_Callable->fetch (func_call_expr->m_FuncName);
        return c->call (i);
    }
    Var call (Interpreter * i, IdentifierExpr * idfier_expr)
    {

    }

    Struct * m_Callable;
    Env m_Env;
};