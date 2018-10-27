#pragma once

#include "Visitor.hh"
#include "Expr.hh"

#include <string>
#include <vector>

struct Expr;
struct Function;
struct Interpreter;
struct IRCodeGen;
struct Struct;

struct Decl
{
    virtual void Declare (DeclVisitor*) = 0;
    virtual ~Decl() = default;
    virtual std::vector<std::string> GetSignature () {return {};}
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
    FuncDecl(std::string func_name, std::vector<std::string> signature, Stmt* block)
        : m_FuncName(std::move(func_name)), m_Signature(std::move(signature)), m_Block(block) {}

    void Declare (DeclVisitor* dv) override
    {
        dv->visit_FuncDecl(this);
    }

    std::vector<std::string> GetSignature () override
    {
        return m_Signature;
    }
    
    ~FuncDecl () override
    {
        delete m_Block;
    }
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
    StructDecl (std::string struct_name, std::vector<FuncDecl *> & method_decl_stmts, FuncDecl* ctor_decl)
        : m_StructName (std::move (struct_name)), m_MethodDecls (std::move (method_decl_stmts)),
            m_CtorDecl (ctor_decl) {}

    void Declare (DeclVisitor* dv) override
    {
        dv->visit_StructDecl (this);
    }
    ~StructDecl () override
    {
        for (auto & e : m_MethodDecls)
        {
            delete e;
        }
        delete m_CtorDecl;
    }
    std::vector<std::string> GetSignature () override
    {
        return m_CtorDecl->GetSignature();
    }
private:
    std::string m_StructName;
    std::vector<FuncDecl *> m_MethodDecls;
    FuncDecl * m_CtorDecl;

    friend Interpreter;
    friend Struct;
};