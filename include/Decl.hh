#pragma once

#include "Visitor.hh"
#include "Expr.hh"
#include "Decl.hh"
#include <string>
#include <vector>

struct Stmt;
struct Expr;
struct Function;
struct Interpreter;
struct IRCodeGen;
struct Struct;

struct Decl
{
    virtual void Declare (DeclVisitor*) = 0;
    virtual ~Decl() = default;
    virtual std::vector<std::string> GetSignature () const {return {};}
};

struct VarDecl : public Decl
{
    VarDecl(std::string var_name, Expr* expr)
        : m_VarName(var_name), m_Expr(expr) {}


    virtual void Declare (DeclVisitor* dv) override
    {
        dv->visit_VarDecl(this);
    }
    ~VarDecl ()
    {
        delete m_Expr;
    }

private:
    std::string m_VarName;
    Expr* m_Expr;

    friend Interpreter;
    friend IRCodeGen;
};


struct AttributeVarDecl : public Decl
{
    AttributeVarDecl (Expr * access, Expr* expr)
        : m_Access (access), m_Expr (expr) {}
        
    void Declare (DeclVisitor* dv) override
    {
        dv->visit_AttributeVarDecl(this);
    }
    ~AttributeVarDecl ()
    {
        delete m_Access;
        delete m_Expr;
    }
    Expr * m_Access, * m_Expr;
};

struct FuncDecl : public Decl
{
    FuncDecl(std::string, std::vector<std::string>, Stmt *);
    ~FuncDecl () override;
    void Declare (DeclVisitor* dv) override;
    std::vector<std::string> GetSignature () const override;

private:
    std::string m_FuncName;
    std::vector<std::string> m_Signature;
    Stmt* m_Block;

    friend Interpreter;
    friend Function;
    friend IRCodeGen;
    friend Struct;
};


struct StructDecl : public Decl
{
    StructDecl (std::string, std::vector<FuncDecl *> &, FuncDecl *);
    ~StructDecl () override;
    void Declare (DeclVisitor* dv) override;
    std::vector<std::string> GetSignature () const override;

private:
    std::string m_StructName;
    std::vector<FuncDecl *> m_MethodDecls;
    FuncDecl * m_CtorDecl;

    friend Interpreter;
    friend Struct;
};