#pragma once

#include "Env.hh"
#include "Visitor.hh"

struct Function;
struct BuiltInPrint;
struct BuiltInPrintLn;
struct BuiltInInt;
struct BuiltInStr;

struct Interpreter final: public ExprVisitor, StmtVisitor, DeclVisitor
{
public:
    Interpreter();
    void Evaluate(std::string const& s);

private:
//  ExprVisitor
    void visit_LiteralExpr (LiteralExpr*) override;
    void visit_IdentifierExpr(IdentifierExpr*) override;
    void visit_BinaryExpr (BinaryExpr*) override;
    void visit_GroupExpr (GroupExpr*) override;
    void visit_FuncCallExpr (FuncCallExpr*) override;
    void visit_MethodCallExpr (MethodCallExpr*) override;
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
    void visit_StructDecl (StructDecl *) override;

    // void MethodCallExpr (Object * o, FuncCallExpr * func_call_expr);
    Var Eval(Expr*);
    Var m_ExprVal;
    void Execute(Stmt*);

    Env m_Env;

    friend BuiltInPrint;
    friend BuiltInPrintLn;
    friend Function;
    friend BuiltInInt;
    friend BuiltInStr;
};