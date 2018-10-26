#pragma once

#include "Var.hh"
#include "Visitor.hh"

#define LOG(x)  std::cout << x << std::endl;

enum class Token;
struct Interpreter;
struct Object;
struct IRCodeGen;

struct Expr {
    virtual void Accept(ExprVisitor*) = 0;
    virtual ~Expr() {}
};

struct LiteralExpr : public Expr
{
    explicit LiteralExpr(Var const& value)
        : m_Value(value) {}

    void Accept(ExprVisitor* v) override
    {
        v->visit_LiteralExpr(this);
    }
    ~LiteralExpr() override = default;
private:
    Var m_Value;

    friend Interpreter;
    friend IRCodeGen;
};

struct IdentifierExpr : public Expr{
    explicit IdentifierExpr(std::string const& name)
        : m_IdentifierName(name) {}

    void Accept(ExprVisitor* v) override
    {
        v->visit_IdentifierExpr(this);
    }
    ~IdentifierExpr() override = default;
private:
    std::string m_IdentifierName;

    friend Interpreter;
    friend IRCodeGen;
};

struct BinaryExpr : public Expr {
    explicit BinaryExpr(Expr* left, Token op, Expr* right)
        : m_Left(left), m_Op(op), m_Right(right) {}

    void Accept(ExprVisitor* v) override
    {
        v->visit_BinaryExpr(this);
    }

    ~BinaryExpr() override
    {
        delete m_Left, m_Right;
    }

private:
    Expr* m_Left, *m_Right;
    Token m_Op;

    friend Interpreter;
    friend IRCodeGen;
};

struct GroupExpr : public Expr {
    friend Interpreter;

    explicit GroupExpr(Expr* expr)
        : m_Expr(expr) {}

    void Accept(ExprVisitor* v) override
    {
        v->visit_GroupExpr(this);
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

    void Accept(ExprVisitor* v) override
    {
        v->visit_FuncCallExpr(this);
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
    friend Object;
};

struct MethodCallExpr : public Expr
{
    MethodCallExpr (Expr * object, Expr * method)
        : m_Object (object), m_Method (method) {}
    
    void Accept (ExprVisitor * v) override
    {
        v->visit_MethodCallExpr (this);
    }
    ~MethodCallExpr ()
    {
        delete m_Object, m_Method;
    }
    Expr * m_Object, * m_Method;
};