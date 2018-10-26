#include "Callable.hh"
#include "Decl.hh"
#include "Var.hh"
#include "Return.hh"
#include "Interpreter.hh"
#include "Object.hh"

Callable::Callable() {}
// Callable::Callable(FuncDecl* f)
//         : m_FuncDecl(f) {}
Callable::Callable(Decl * d)
        : m_Decl (d) {}

Function::Function (FuncDecl* func_decl)
        : Callable(func_decl) {}

Var Function::call (Interpreter* i)
{
    try {
        i->Execute(((FuncDecl*)m_Decl)->m_Block);

    } catch(ReturnException& r)
    {
        return r.m_Val;
    }
    return Var();
}


// Struct
Struct::Struct (StructDecl * struct_decl)
    : Callable (struct_decl) , m_StructDecl(struct_decl){}

Var Struct::call (Interpreter * i)
{
    return (Var (new Object(this)));
}

Callable * Struct::fetch (std::string & s) const
{
    LOG(s);
    return m_Methods.at (s);
}