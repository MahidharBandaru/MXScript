#include "PrettyPrinter.hh"

#include "Expr.hh"
#include "Token.hh"

#include <iostream>

#define LOG(x) std::cout << x;
#define ENDL    std::cout << '\n';


void PrettyPrinter :: Eval (Expr * expr)
{
    expr -> Accept (this);
}

void PrettyPrinter :: visit_LiteralExpr (LiteralExpr * l)
{
    LOG('(')
    LOG("LiteralExpr(")
    l->m_Value.Print ();
    LOG(')')
    LOG(')');
}

void PrettyPrinter :: visit_UnaryExpr(UnaryExpr * u)
{
    LOG('(')
    LOG("UnaryExpr(")
    LOG(u->m_Op);
    Eval (u->m_Expr);
    LOG(')')
    LOG(')');
}
void PrettyPrinter :: visit_IdentifierExpr(IdentifierExpr* id_expr)
{
    LOG('(');
    LOG("IdentifierExpr(");
    LOG(id_expr->m_IdentifierName);
    LOG(')')
    LOG(')');
}
void PrettyPrinter :: visit_BinaryExpr (BinaryExpr * binary_expr)
{
    LOG ('(');
    LOG ("BinaryExpr(");
    Eval (binary_expr->m_Left);
    LOG (binary_expr -> m_Op);
    Eval (binary_expr->m_Right);
    LOG (')')
    LOG (')');
}
void PrettyPrinter :: visit_GroupExpr (GroupExpr* grp_expr)
{
    LOG('(');
    LOG("GroupExpr(");
    Eval (grp_expr -> m_Expr);
    LOG(')')
    LOG(')');
}

void PrettyPrinter :: visit_CallExpr (CallExpr * call_expr)
{
    LOG ('(');
    LOG ("CallExpr(");
    LOG (call_expr -> m_CallName );
    for (auto & e : call_expr -> m_Args)
    {
        Eval (e);
    }
    LOG (')')
    LOG (')');
}
void PrettyPrinter :: visit_AttributeAccessExpr (AttributeAccessExpr * attr_access_expr)
{
    
}

//  StmtVisitor
void PrettyPrinter :: visit_ExprStmt (ExprStmt*) override;
void PrettyPrinter :: visit_BlockStmt (BlockStmt*) override;
void PrettyPrinter :: visit_WhileStmt (WhileStmt*) override;
void PrettyPrinter :: visit_ReturnStmt (ReturnStmt*) override;
void PrettyPrinter :: visit_CondStmt (CondStmt*) override;
void PrettyPrinter :: visit_VarDeclStmt (VarDeclStmt*) override;
void PrettyPrinter :: visit_FuncDeclStmt (FuncDeclStmt*) override;
void PrettyPrinter :: visit_CallStmt (CallStmt *) override;
void PrettyPrinter :: visit_StructDeclStmt (StructDeclStmt *) override;
void PrettyPrinter :: visit_AttributeVarDeclStmt (AttributeVarDeclStmt *) override;

//  DeclVisitor
void PrettyPrinter :: visit_VarDecl(VarDecl*) override;
void PrettyPrinter :: visit_FuncDecl(FuncDecl*) override;
void PrettyPrinter :: visit_StructDecl (StructDecl *) override;
void PrettyPrinter :: visit_AttributeVarDecl (AttributeVarDecl *) override;