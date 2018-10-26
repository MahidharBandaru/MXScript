#pragma once

#include "Visitor.hh"

#include <string>
#include <vector>

struct Expr;
struct Function;
struct Interpreter;
struct IRCodeGen;

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


    void Declare (DeclVisitor* dv) override
    {
        dv->visit_VarDecl(this);
    }

private:
    std::string m_VarName;
    Expr* m_Expr;

    friend Interpreter;
    friend IRCodeGen;
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
    
    ~FuncDecl ()
    {
        // delete m_Block;
    }
private:
    std::string m_FuncName;
    std::vector<std::string> m_Signature;
    Stmt* m_Block;

    friend Interpreter;
    friend Function;
    friend IRCodeGen;
};


struct StructDecl : public Decl
{
    StructDecl (std::string struct_name)
        : m_StructName (std::move (struct_name)) {}
        //         std::unique_ptr <ConstructorDeclStmt> constructor,
        //         std::unique_ptr <std::string> attributes)
        // : m_StructName (std::move (struct_name)),
        //   m_Constructor (std::move(constructor)),
        //   m_Attributes (std::move (attributes)) {}

    void Declare (DeclVisitor* dv) override
    {
        dv->visit_StructDecl (this);
    }
    ~StructDecl ()
    {
        
    }
private:
    std::string m_StructName;
    // std::unique_ptr <ConstructorDeclStmt> m_Constructor;
    std::vector <std::string> m_Attributes;
    // std::unique_ptr <FuncDecl> m_Methods;

    friend Interpreter;

};