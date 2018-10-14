#pragma once

struct Var;

struct Expr;
struct LiteralExpr;
struct IdentifierExpr;
struct BinaryExpr;
struct GroupExpr;
struct FuncCallExpr;

struct ExprVisitor {
    virtual Var visit_LiteralExpr(LiteralExpr*) = 0;
    virtual Var visit_IdentifierExpr(IdentifierExpr*) = 0;
    virtual Var visit_BinaryExpr(BinaryExpr*) = 0;
    virtual Var visit_GroupExpr(GroupExpr*) = 0;
    virtual Var visit_FuncCallExpr(FuncCallExpr*) = 0;
};



struct Stmt;
struct ExprStmt;
struct BlockStmt;
struct WhileStmt;
struct ReturnStmt;
struct CondStmt;
struct VarDeclStmt;
struct FuncDeclStmt;
struct FuncCallStmt;

struct StmtVisitor {
    virtual void visit_ExprStmt(ExprStmt*) = 0;
    virtual void visit_BlockStmt(BlockStmt*) = 0;
    virtual void visit_WhileStmt(WhileStmt*) = 0;
    virtual void visit_ReturnStmt(ReturnStmt*) = 0;
    virtual void visit_CondStmt (CondStmt*) = 0;
    virtual void visit_VarDeclStmt(VarDeclStmt*) = 0;
    virtual void visit_FuncDeclStmt(FuncDeclStmt*) = 0;
    virtual void visit_FuncCallStmt(FuncCallStmt*) = 0;
};



struct VarDecl;
struct FuncDecl;

struct DeclVisitor {
    virtual void visit_VarDecl(VarDecl*) = 0;
    virtual void visit_FuncDecl(FuncDecl*) = 0;
};