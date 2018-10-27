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
    Object (Struct * struc, Interpreter * i)
        : m_Struct (struc)
    {
        Function * f = m_Struct->m_Ctor;

        Env g = i->m_GEnv;
        i->m_GEnv += m_Env;
        i->m_GEnv += m_Struct->m_Env;
        i->m_GEnv.insert ({std::string("self"), this});

        f->call (i);
        i->m_GEnv = g;
    }

    Var call (Interpreter * i, CallExpr * call_expr)
    {
        Callable * c;
        try {
            c = m_Struct->fetch (call_expr->m_CallName);

        } catch (std::out_of_range & e)
        {
            throw std::out_of_range("out of range");
        }
        Env g = i->m_GEnv;
        i->m_GEnv += m_Env;
        i->m_GEnv += m_Struct->m_Env;
        i->m_GEnv.insert ({std::string("self"), this});
        Var ret = i->visit_Callable (c, call_expr->m_Args);
        i->m_GEnv = g;
        return ret;
    }
    Var call (Interpreter * i, IdentifierExpr * idfier_expr)
    {
        try {
            return m_Env.at (idfier_expr->m_IdentifierName);
        }
        catch (std::out_of_range & e)
        {
            return Var();
        }
    }

    void set (Interpreter * i, std::string & attr, Var v)
    {
        m_Env . insert ({std::string(attr), v});
    }

    Struct * m_Struct;
    Env m_Env;
};