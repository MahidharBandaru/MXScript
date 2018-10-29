#pragma once

#include "Var.hh"
#include "Visitor.hh"

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
    friend Object;
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
        delete m_Left;
        delete m_Right;
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

struct CallExpr final : public Expr {
    explicit CallExpr(std::string call_name, std::vector<Expr*> args)
        : m_CallName(call_name), m_Args(std::move(args)) {}

    void Accept(ExprVisitor* v) override
    {
        v->visit_CallExpr(this);
    }
    ~CallExpr() override
    {
        for(auto& e : m_Args)
        {
            delete e;
        }
        m_Args.clear();
    }
private:
    std::string m_CallName;
    std::vector<Expr*> m_Args;

    friend Interpreter;
    friend Object;
};

struct AttributeAccessExpr : public Expr
{
    AttributeAccessExpr (Expr * object, Expr * method)
        : m_Object (object), m_Attribute (method) {}
    
    void Accept (ExprVisitor * v) override
    {
        v->visit_AttributeAccessExpr (this);
    }
    ~AttributeAccessExpr ()
    {
        delete m_Object;
        delete m_Attribute;
    }
    Expr * m_Object, * m_Attribute;
};