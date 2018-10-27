#pragma  once

#include "Decl.hh"
#include "Callable.hh"
#include "Interpreter.hh"

#include <string>
#include <cmath>

FuncDecl * BuiltInPrintDecl = new FuncDecl(std::string("print"), {std::string("x")}, {});
FuncDecl * BuiltInPrintLnDecl = new FuncDecl(std::string("println"), {std::string("x")}, {});
FuncDecl * BuiltInInputDecl = new FuncDecl(std::string("input"), {}, {});
FuncDecl * BuiltInIntDecl = new FuncDecl(std::string("int"), {std::string("x")}, {});
FuncDecl * BuiltInStrDecl = new FuncDecl(std::string("str"), {std::string("x")}, {});
FuncDecl * BuiltInSqrtDecl = new FuncDecl (std::string("sqrt"), {std::string("x")}, {});

std::string const arg {"x"};

struct BuiltInPrint : public Callable
{
    BuiltInPrint()
        : Callable (BuiltInPrintDecl) {}

    Var call (Interpreter* i) override
    {
        i->m_Env.at(arg).Print();
        return Var();
    }
};



struct BuiltInPrintLn : public Callable
{
    BuiltInPrintLn()
        : Callable (BuiltInPrintLnDecl) {}

    Var call (Interpreter* i) override
    {
        i->m_Env.at(arg).Print();
        std::cout << '\n';
        return Var();
    }
};



struct BuiltInInput : public Callable
{
    BuiltInInput()
        : Callable (BuiltInInputDecl) {}

    Var call (Interpreter* i) override
    {
        std::string x;
        std::cin >> x;
        return Var(x);
    }
};


struct BuiltInInt : public Callable
{
    BuiltInInt ()
        : Callable(BuiltInIntDecl) {}

    Var call (Interpreter* i) override
    {
        return Var(int(i->m_Env.at(arg)));
    }
};


struct BuiltInStr : public Callable
{
    BuiltInStr ()
        : Callable(BuiltInStrDecl) {}

    Var call (Interpreter* i) override
    {
        return Var(std::string(i->m_Env.at(arg)));
    }
};

struct BuiltInSqrt : public Callable
{
    BuiltInSqrt ()
        : Callable (BuiltInSqrtDecl) {}
    
    Var call (Interpreter* i) override
    {
        return Var(std::sqrt(int(i->m_Env.at(arg))));
    }
};