#pragma once

#include "Var.hh"

struct FuncDecl;
struct Interpreter;

#define LOG(x) std::cout << x << std::endl;

struct Callable
{
    Callable();
    explicit Callable(FuncDecl* f);
    virtual Var call(Interpreter* i) = 0;

    FuncDecl* m_FuncDecl;
};

struct Function : public Callable
{
    explicit  Function (FuncDecl* func_decl);

    Var call (Interpreter* i) override;
};