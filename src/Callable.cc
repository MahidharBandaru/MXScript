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
    : Callable (struct_decl), m_Ctor (new Function (struct_decl->m_CtorDecl)) {
        // m_CtorDecl = struct_decl->m_CtorDecl;
        auto & func_decls = struct_decl->m_MethodDecls;
        for(auto & e: func_decls)
        {
            m_Env.insert ({e->m_FuncName, new Function (e)});
        }
    }

Var Struct::call (Interpreter * i)
{
    return (Var (new Object(this, i)));
}

Callable * Struct::fetch (std::string & s) const
{
    try {
        return m_Env.at (s);
    }
    catch (std::out_of_range & e)
    {
        throw std::out_of_range ("outta");
    }
        
}