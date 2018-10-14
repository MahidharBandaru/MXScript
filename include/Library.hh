#pragma  once

/*
 * Input
 * Output
 */
#include "Decl.hh"
#include "Callable.hh"
#include "Interpreter.hh"

#include <string>

FuncDecl* BuiltInPrintDecl = new FuncDecl(std::string("print"), {std::string("x")}, {});
FuncDecl* BuiltInPrintLnDecl = new FuncDecl(std::string("println"), {std::string("x")}, {});
FuncDecl* BuiltInInputDecl = new FuncDecl(std::string("input"), {}, {});

struct BuiltInPrint : public Callable
{
    BuiltInPrint()
        : Callable (BuiltInPrintDecl) {}

    Var call (Interpreter* i) override
    {
        i->m_Env.at(arg).Print();
        return Var();
    }
    static const std::string arg;
};

const std::string BuiltInPrint::arg = "x";



struct BuiltInPrintLn : public Callable
{
    BuiltInPrintLn()
        : Callable (BuiltInPrintLnDecl) {}

    Var call (Interpreter* i) override
    {
        i->m_Env.at(arg).Print();
        LOG("")
        return Var();
    }
    static const std::string arg;
};

const std::string BuiltInPrintLn::arg = "x";



struct BuiltInInput : public Callable
{
    BuiltInInput()
        : Callable (BuiltInInputDecl) {}

    Var call (Interpreter* i) override
    {
        int x;
        std::cin >> x;
        return Var(x);
    }
};