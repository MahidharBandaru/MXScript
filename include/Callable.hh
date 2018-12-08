#pragma once

#include "Var.hh"
#include "Env.hh"

#include <map>

struct Decl;
struct FuncDecl;
struct StructDecl;
struct Interpreter;

struct Callable
{
    Callable();
    explicit Callable (Decl *);
    virtual Var call(Interpreter* i) = 0;

    Decl * m_Decl;
};

struct Function : public Callable
{
    explicit  Function (FuncDecl *);

    Var call (Interpreter* i) override;
};

struct Struct : public Callable
{
    Struct (StructDecl *);

    Var call (Interpreter *) override;
    Callable * fetch (std::string &) const;
    
    std::string get_name () const noexcept {
        return m_Name;
    }
    Function * m_Ctor;
    std::string m_Name;
    Env m_Env;
};