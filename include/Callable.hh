#pragma once

#include "Var.hh"

#include <map>

struct Decl;
struct FuncDecl;
struct StructDecl;
struct Interpreter;

#define LOG(x) std::cout << x << std::endl;

struct Callable
{
    Callable();
    // explicit Callable(FuncDecl* f);
    explicit Callable (Decl *);
    virtual Var call(Interpreter* i) = 0;

    // FuncDecl* m_FuncDecl;
    Decl * m_Decl;
};

struct Function : public Callable
{
    explicit  Function (FuncDecl *);

    Var call (Interpreter* i) override;
};

struct Struct : public Callable
{
    explicit Struct (StructDecl *);

    Var call (Interpreter *) override;
    Callable * fetch (std::string &) const;
    
    std::shared_ptr <StructDecl> m_StructDecl;
    std::map <std::string, Callable*> m_Methods;
};