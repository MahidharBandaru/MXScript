#pragma once

#include "Visitor.hh"

#include <string>
#include <vector>

struct Expr;
struct Function;
struct Interpreter;

struct Decl
{
    virtual void Declare (DeclVisitor*) = 0;
    virtual ~Decl() = default;
};

struct VarDecl : public Decl
{
    VarDecl(std::string var_name, Expr* expr)
        : m_VarName(var_name), m_Expr(expr) {}


    void Declare (DeclVisitor* dv) override
    {
        dv->visit_VarDecl(this);
    }

private:
    std::string m_VarName;
    Expr* m_Expr;

    friend Interpreter;
};



struct FuncDecl : public Decl
{
    FuncDecl(std::string func_name, std::vector<std::string> signature, Stmt* block)
        : m_FuncName(std::move(func_name)), m_Signature(std::move(signature)), m_Block(block) {}

    void Declare (DeclVisitor* dv) override
    {
        dv->visit_FuncDecl(this);
    }

private:
    std::string m_FuncName;
    std::vector<std::string> m_Signature;
    Stmt* m_Block;

    friend Interpreter;
    friend Function;
};