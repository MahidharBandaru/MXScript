#pragma once

#include "Lexer.hh"

#include <unordered_set>

struct Stmt;
struct Expr;
enum class SyntaxError;
struct Parser {
    explicit Parser (Lexer const& l) noexcept;
    
    std::vector<Stmt*> Parse();

private:
    Lexer m_Lexer;
    Token m_CurrentToken;
    Position m_PrevPos;

    void Advance() noexcept;
    bool Peek(Token) noexcept;
    bool Match(Token) noexcept;
    void Eat(Token);
    void ThrowError (SyntaxError, std::stringstream & );

    Expr * Primary();
    Expr * Factor();
    Expr * Addition();
    Expr * Expression();
    Expr * ComparatorExpr();
    Expr * ParseCallExpr();
    Expr * ParseLiteralExpr();
    Expr * ParseIdentifierExpr();
    Expr * ParseMethodCallExpr ();
    Expr * ParseAttributeAccessExpr ();

    Stmt * Statement();
    Stmt * ParseExprStmt();
    Stmt * ParseWhileStmt();
    Stmt * ParseVarDecl();
    Stmt * ParseReturnStmt();
    Stmt * ParseCondStmt();
    Stmt * ParseFuncDeclStmt();
    Stmt * ParseBlockStmt(std::unordered_set <Token> const&);
    Stmt * ParseCallStmt();
    Stmt * ParseStructDecl();
    Stmt * ParseStructBlockStmt();
    Stmt * ParseConstructorDeclStmt();
    Stmt * ParseSelfVarDecl ();
    Stmt * ParseSelfFuncCall ();
};