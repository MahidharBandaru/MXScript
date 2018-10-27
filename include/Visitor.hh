#pragma once

struct Var;

struct Expr;
struct LiteralExpr;
struct IdentifierExpr;
struct BinaryExpr;
struct GroupExpr;
struct CallExpr;
struct AttributeAccessExpr;

struct ExprVisitor {
    virtual void visit_LiteralExpr(LiteralExpr*) = 0;
    virtual void visit_IdentifierExpr(IdentifierExpr*) = 0;
    virtual void visit_BinaryExpr(BinaryExpr*) = 0;
    virtual void visit_GroupExpr(GroupExpr*) = 0;
    virtual void visit_CallExpr(CallExpr *) = 0;
    virtual void visit_AttributeAccessExpr (AttributeAccessExpr *) = 0;
};



struct Stmt;
struct ExprStmt;
struct BlockStmt;
struct WhileStmt;
struct ReturnStmt;
struct CondStmt;
struct VarDeclStmt;
struct FuncDeclStmt;
struct CallStmt;
struct StructDeclStmt;
struct ConstructorDeclStmt;
struct AttributeVarDeclStmt;

struct StmtVisitor {
    virtual void visit_ExprStmt(ExprStmt*) = 0;
    virtual void visit_BlockStmt(BlockStmt*) = 0;
    virtual void visit_WhileStmt(WhileStmt*) = 0;
    virtual void visit_ReturnStmt(ReturnStmt*) = 0;
    virtual void visit_CondStmt (CondStmt*) = 0;
    virtual void visit_VarDeclStmt(VarDeclStmt*) = 0;
    virtual void visit_FuncDeclStmt(FuncDeclStmt*) = 0;
    virtual void visit_CallStmt (CallStmt *) = 0;
    virtual void visit_StructDeclStmt (StructDeclStmt *) = 0;
    virtual void visit_AttributeVarDeclStmt (AttributeVarDeclStmt *) = 0;
    // virtual void visit_ConstructorDeclStmt(ConstructorDeclStmt*) = 0;
};



struct VarDecl;
struct FuncDecl;
struct StructDecl;
struct AttributeVarDecl;

struct DeclVisitor {
    virtual void visit_VarDecl(VarDecl*) = 0;
    virtual void visit_FuncDecl(FuncDecl*) = 0;
    virtual void visit_StructDecl (StructDecl *) = 0;
    virtual void visit_AttributeVarDecl (AttributeVarDecl *) = 0;
};