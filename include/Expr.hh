#pragma once

#include "Var.hh"
#include "Visitor.hh"

#define LOG(x)  std::cout << x << std::endl;

enum class Token;
struct Interpreter;

struct Expr {
    virtual Var Eval(ExprVisitor*) = 0;
    virtual ~Expr() {}
};

struct LiteralExpr : public Expr
{
    explicit LiteralExpr(Var const& value)
        : m_Value(value) {}

    Var Eval(ExprVisitor* v) override
    {
        return v->visit_LiteralExpr(this);
    }
    ~LiteralExpr() override = default;
private:
    Var m_Value;

    friend Interpreter;
};

struct IdentifierExpr : public Expr{
    explicit IdentifierExpr(std::string const& name)
        : m_IdentifierName(name) {}

    Var Eval(ExprVisitor* v) override
    {
        return v->visit_IdentifierExpr(this);
    }
    ~IdentifierExpr() override = default;
private:
    std::string m_IdentifierName;

    friend Interpreter;
};

struct BinaryExpr : public Expr {
    explicit BinaryExpr(Expr* left, Token op, Expr* right)
        : m_Left(left), m_Op(op), m_Right(right) {}

    Var Eval(ExprVisitor* v) override
    {
        return v->visit_BinaryExpr(this);
    }

    ~BinaryExpr() override
    {
        delete m_Left, m_Right;
    }

private:
    Expr* m_Left, *m_Right;
    Token m_Op;

    friend Interpreter;
};

struct GroupExpr : public Expr {
    friend Interpreter;

    explicit GroupExpr(Expr* expr)
        : m_Expr(expr) {}

    Var Eval(ExprVisitor* v) override
    {
        return v->visit_GroupExpr(this);
    }
    ~GroupExpr() override
    {
        delete m_Expr;
    }
private:
    Expr* m_Expr;
};

struct FuncCallExpr final : public Expr {
    explicit FuncCallExpr(std::string func_name, std::vector<Expr*> args)
        : m_FuncName(func_name), m_Args(std::move(args)) {}

    Var Eval(ExprVisitor* v) override
    {
        return v->visit_FuncCallExpr(this);
    }
    ~FuncCallExpr() override
    {
        for(auto& e : m_Args)
        {
            delete e;
        }
        m_Args.clear();
    }
private:
    std::string m_FuncName;
    std::vector<Expr*> m_Args;

    friend Interpreter;    
};