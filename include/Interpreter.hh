#pragma once

#include "Env.hh"
#include "Visitor.hh"

struct Function;
struct BuiltInPrint;
struct BuiltInPrintLn;

struct Interpreter final: public ExprVisitor, StmtVisitor, DeclVisitor
{
public:
    Interpreter();
    void Evaluate(std::string const& s);

private:
//  ExprVisitor
    Var visit_LiteralExpr (LiteralExpr*) override;
    Var visit_IdentifierExpr(IdentifierExpr*) override;
    Var visit_BinaryExpr (BinaryExpr*) override;
    Var visit_GroupExpr (GroupExpr*) override;
    Var visit_FuncCallExpr (FuncCallExpr*) override;
//  StmtVisitor
    void visit_ExprStmt (ExprStmt*) override;
    void visit_BlockStmt (BlockStmt*) override;
    void visit_WhileStmt (WhileStmt*) override;
    void visit_ReturnStmt (ReturnStmt*) override;
    void visit_CondStmt (CondStmt*) override;
    void visit_VarDeclStmt (VarDeclStmt*) override;
    void visit_FuncDeclStmt (FuncDeclStmt*) override;
    void visit_FuncCallStmt (FuncCallStmt*) override;
//  DeclVisitor
    void visit_VarDecl(VarDecl*) override;
    void visit_FuncDecl(FuncDecl*) override;


    
    Var Eval(Expr*);
    void Execute(Stmt*);

    Env m_Env;

    friend BuiltInPrint;
    friend BuiltInPrintLn;
    friend Function;
};