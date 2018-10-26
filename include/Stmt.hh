#pragma once

#include "Decl.hh"
#include "Visitor.hh"

#include <string>
#include <vector>

struct Interpreter;
struct Parser;
struct Expr;

struct Stmt {
    virtual void Execute(StmtVisitor*) = 0;
    virtual ~Stmt() = default;
    friend Interpreter;
};



struct VarDeclStmt final : public Stmt {
    VarDeclStmt(std::string var_name, Expr* expr)
        : m_VarDecl(new VarDecl(var_name, expr)) {}

    void Execute(StmtVisitor* v) override {
        v->visit_VarDeclStmt(this);
    }
    ~VarDeclStmt() override {
        delete m_VarDecl;
    }
private:
    VarDecl* m_VarDecl;

    friend Interpreter;
};



struct FuncDeclStmt : public Stmt
{
    FuncDeclStmt(std::string func_name, std::vector<std::string> args, Stmt* block)
        : m_FuncDecl(new FuncDecl(func_name, args, block)) {}

    virtual void Execute(StmtVisitor* v) override
    {
        v->visit_FuncDeclStmt(this);
    }
protected:
    FuncDecl* m_FuncDecl;

    friend Interpreter;
    friend Parser;
};



struct BlockStmt final : public Stmt
{
    BlockStmt(std::vector<Stmt*>& block)
        : m_Block(std::move(block)) {}

    ~BlockStmt() override {
        for(auto& e: m_Block) {
            delete e;
        }
        m_Block.clear();
    }
    void Execute(StmtVisitor* v) override {
        v->visit_BlockStmt(this);
    }
private:
    std::vector<Stmt*> m_Block;

    friend Interpreter;
};



struct ExprStmt final : public Stmt {
    ExprStmt(Expr* expr)
        : m_Expr(expr) {}
    ~ExprStmt() override {
        delete m_Expr;
    }
    void Execute(StmtVisitor* v) override {
        v->visit_ExprStmt(this);
    }
    friend Interpreter;
private:
    Expr* m_Expr;
};



struct WhileStmt final : public Stmt
{
    WhileStmt(Expr* cond, Stmt* block)
        : m_Condition(cond), m_Block(block) {}

    ~WhileStmt() override
    {
        delete m_Condition, m_Block;
    }
    void Execute (StmtVisitor* sv) override
    {
        sv->visit_WhileStmt(this);
    }
    Expr* m_Condition;
    Stmt* m_Block;
};



struct FuncCallStmt final : public Stmt
{
    FuncCallStmt(FuncCallExpr* func_call_expr)
        : m_FuncCallExpr(func_call_expr) {}
    
    void Execute (StmtVisitor* sv) override
    {
        sv->visit_FuncCallStmt(this);
    }

private:
    FuncCallExpr* m_FuncCallExpr;

    friend Interpreter;
};

struct ReturnStmt final : public Stmt
{
    ReturnStmt(Expr* ret_expr)
        : m_RetExpr(ret_expr) {}

    void Execute (StmtVisitor* sv) override
    {
        sv->visit_ReturnStmt(this);
    }
    Expr* m_RetExpr;
};


struct CondStmt final : public Stmt
{
    CondStmt(Expr* cond_expr, Stmt* if_block, Stmt* else_block )
        : m_CondExpr(cond_expr), m_IfBlock(if_block), m_ElseBlock(else_block) {}

    void Execute (StmtVisitor* sv) override
    {
        sv->visit_CondStmt(this);
    }


    Expr* m_CondExpr;
    Stmt* m_IfBlock, *m_ElseBlock;
};



// struct StructDeclStmt final : public Stmt
// {
//     StructDeclStmt (std::string struct_name,
//                     std::unique_ptr<ConstructorDeclStmt> constructor,
//                     std::vector<std::unique_ptr<IdentifierExpr>> attributes)
//         : m_StructDecl (new StructDecl (struct_name, constructor, attributes)) {}

//     void Execute (StmtVisitor* sv) override
//     {
//         sv->visitStructDeclStmt (this);
//     }

//     std::unique_ptr<StructDecl> m_StructDecl;
// };

// struct ConstructorDeclStmt : public FuncDeclStmt
// {
//     ConstructorDeclStmt (std::string func_name, std::vector<std::string> args, Stmt* block)
//         : FuncDeclStmt(func_name, args, block) {}

//     void Execute(StmtVisitor* v) override
//     {
//         v->visit_ConstructorDeclStmt(this);
//     }
// };