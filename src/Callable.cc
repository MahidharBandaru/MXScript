#include "Callable.hh"
#include "Decl.hh"
#include "Var.hh"
#include "Return.hh"
#include "Interpreter.hh"

Callable::Callable() {}
Callable::Callable(FuncDecl* f)
        : m_FuncDecl(f) {}

Function::Function (FuncDecl* func_decl)
        : Callable(func_decl) {}

Var Function::call (Interpreter* i)
{
    try {
        i->Execute(m_FuncDecl->m_Block);

    } catch(ReturnException& r)
    {
        return r.m_Val;
    }
    return Var();
}