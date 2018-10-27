#pragma once

#include "Lexer.hh"

struct Stmt;
struct Expr;

struct Parser {
    explicit Parser (Lexer const& l) noexcept;
    
    Stmt* Parse();

private:
    mutable Lexer m_Lexer;
    mutable Token m_CurrentToken;

    void Advance() noexcept;
    bool Peek(Token) noexcept;
    bool Match(Token) noexcept;

    Expr* Primary();
    Expr* Factor();
    Expr* Addition();
    Expr* Expression();
    Expr* ComparatorExpr();
    Expr* ParseCallExpr();
    Expr* ParseLiteralExpr();
    Expr* ParseIdentifierExpr();
    Expr * ParseMethodCallExpr ();
    Expr * ParseAttributeAccessExpr ();

    Stmt* Statement();
    Stmt* ParseExprStmt();
    Stmt* ParseWhileStmt();
    Stmt* ParseVarDecl();
    Stmt* ParseReturnStmt();
    Stmt* ParseCondStmt();
    Stmt* ParseFuncDeclStmt();
    Stmt* ParseBlockStmt();
    Stmt* ParseCallStmt();
    Stmt* ParseStructDecl();
    Stmt* ParseStructBlockStmt();
    Stmt* ParseConstructorDeclStmt();
    Stmt * ParseSelfVarDecl ();
    Stmt * ParseSelfFuncCall ();
    void Eat(Token);
};