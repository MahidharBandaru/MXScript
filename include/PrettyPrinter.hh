#pragma once

#include "Visitor.hh"

struct PrettyPrinter : public ExprVisitor, StmtVisitor, DeclVisitor {

    void visit_LiteralExpr (LiteralExpr*) override;
    void visit_UnaryExpr(UnaryExpr *) override;
    void visit_IdentifierExpr(IdentifierExpr*) override;
    void visit_BinaryExpr (BinaryExpr*) override;
    void visit_GroupExpr (GroupExpr*) override;
    void visit_CallExpr (CallExpr*) override;
    void visit_AttributeAccessExpr (AttributeAccessExpr*) override;

//  StmtVisitor
    void visit_ExprStmt (ExprStmt*) override;
    void visit_BlockStmt (BlockStmt*) override;
    void visit_WhileStmt (WhileStmt*) override;
    void visit_ReturnStmt (ReturnStmt*) override;
    void visit_CondStmt (CondStmt*) override;
    void visit_VarDeclStmt (VarDeclStmt*) override;
    void visit_FuncDeclStmt (FuncDeclStmt*) override;
    void visit_CallStmt (CallStmt *) override;
    void visit_StructDeclStmt (StructDeclStmt *) override;
    void visit_AttributeVarDeclStmt (AttributeVarDeclStmt *) override;

//  DeclVisitor
    void visit_VarDecl(VarDecl*) override;
    void visit_FuncDecl(FuncDecl*) override;
    void visit_StructDecl (StructDecl *) override;
    void visit_AttributeVarDecl (AttributeVarDecl *) override;


    void Eval (Expr * );
};