#include "Parser.hh"

#include "Expr.hh"
#include "Stmt.hh"
#include "SyntaxException.hh"

#include <vector>

#define endl '\n';
#define LOG(x)  std::cout << x << endl;

Parser::Parser(Lexer const& l) noexcept: m_Lexer(l)
{
    m_PrevPos = {1, 0};
    Advance();
}

std::vector<Stmt*> Parser::Parse() {
    std::vector <Stmt *> stmts;
    while (!m_Lexer.AtEnd ())
    {
        stmts.push_back (Statement ());
    }
    
    return stmts;
}

void Parser::Advance() noexcept {
    m_PrevPos = m_Lexer.GetPosition ();
    m_CurrentToken = m_Lexer.Read();
}

bool Parser::Match(Token t) noexcept {
    return (m_CurrentToken == t);
}

bool Parser::Peek(Token t) noexcept {
    return  (m_Lexer.Peek() == t);
}

void Parser::ThrowError (SyntaxError s, std::stringstream & se)
{
    std::stringstream ss;
    auto pos = m_PrevPos;
    ss << "Syntax Error: " << "Line: ";
    switch (s) {
        case SyntaxError :: InvalidToken : {
            ss << m_Lexer.GetPosition () .LineNo << " Column: " << m_Lexer.GetPosition () . ColNo + 1;
            break;
        }
        case SyntaxError :: UnexpectedToken : {
            ss << pos.LineNo << " Column: " << pos.ColNo + 1;
        }
            
    }
    ss << endl;
    ss << se.str();
    throw SyntaxException (ss);
}

Expr* Parser::Expression()
{
    Expr* ex = ComparatorExpr();
    switch (m_CurrentToken)
    {
        case Token::OR : {
            Advance();
            ex = new BinaryExpr(ex, Token::OR, Expression());
            break;
        }
        case Token::AND : {
            Advance();
            ex = new BinaryExpr(ex, Token::AND, Expression());
            break;
        }
        default : break;
    }

    return ex;
}

Expr* Parser::ComparatorExpr() {
    Expr* ex = Addition();

    switch(m_CurrentToken) {
        case (Token::EOL) : {
            break;
        }
        case (Token::OP_DEQ) : {
            Token op = m_CurrentToken;
            Advance();
            ex = new BinaryExpr(ex, op, ComparatorExpr());
            break;
        }
        case (Token::OP_NEQ) : {
            Token op = m_CurrentToken;
            Advance();
            ex = new BinaryExpr(ex, op, ComparatorExpr());
            break;
        }
        case (Token::OP_LT) : {
            Advance();
            ex = new BinaryExpr(ex, Token::OP_LT, ComparatorExpr());
            break;
        }
        case (Token::OP_LTE) : {
             Advance();
             ex = new BinaryExpr(ex, Token::OP_LTE, ComparatorExpr());
             break;
        }
        case (Token::INVALID) : {
            std::stringstream ss;
            ss << "Invalid Token : " << m_Lexer.GetCurrTokText() << '\n';
            ThrowError (SyntaxError :: InvalidToken , ss);
        }
        default : {
            break;
        }
    }
    return ex;
}

Expr* Parser::Addition() {
    Expr* ex = Factor();

    while(Match(Token::OP_ADD) or Match(Token::OP_SUB) ) {
        Token op = m_CurrentToken;
        Advance();
        ex = new BinaryExpr(ex, op, Factor());
    }
    return ex;
}

Expr* Parser::Factor() {
    Expr* ex = Primary();
    
    while(Match(Token::OP_MUL) || Match(Token::OP_DIV)) {
        Token op = m_CurrentToken;
        Advance();
        ex = new BinaryExpr(ex, op, Primary());
    }
    return ex;
}

Expr* Parser::Primary() {
    Expr* ex = nullptr;
    switch(m_CurrentToken) {
        case (Token::INT) : {
            ex = new LiteralExpr(m_Lexer.GetValue());
            Advance();
            break;
        }
        case Token::OP_SUB : {
            Advance ();
            ex = new UnaryExpr (Token::OP_SUB, Primary ());
            break;
        }
        case Token::OP_ADD : {
            Advance ();
            ex = new UnaryExpr (Token::OP_ADD, Primary ());
            break;
        }
        case (Token::STRING) : {
            ex = (new LiteralExpr(m_Lexer.GetValue()));    
            Advance();
            break;
        }
        case (Token::IDENTIFIER) : {
            if(Peek(Token::L_PAREN))
            {
                ex = ParseCallExpr();
                // Advance();
            }
            else if (Peek(Token::DOT))
            {
                ex = ParseAttributeAccessExpr ();
            }
            else
            {
                // std::string s = std::string(m_Lexer.GetValue());
                // Advance();
                // ex = (new IdentifierExpr(s));
                ex = ParseIdentifierExpr();
            }

            break;
        }
        case (Token::L_PAREN) : {
            Advance();
            ex = new GroupExpr(Expression());
            Eat(Token::R_PAREN);
            break;
        }
        case (Token::INVALID) : {
            // throw SyntaxException(SyntaxError::InvalidToken, m_CurrentToken);
            std::stringstream ss;
            ss << "Invalid Token : " << m_Lexer.GetCurrTokText() << '\n';
            ThrowError (SyntaxError :: InvalidToken, ss);
        }
        default : {
            std::stringstream ss;
            ss << "Unexp here";
            ThrowError(SyntaxError::UnexpectedToken, ss);
            break;
        }

    }
    // if (Match (Token::DOT))
    // {
    //     ex = new AttributeAccessExpr (ex, ParseAttributeAccessExpr());
    // }
    return ex;
}

Expr * Parser :: ParseAttributeAccessExpr ()
{
    if (Peek (Token :: L_PAREN ))
    {
        LOG(std::string ("func ") + m_Lexer.GetCurrTokText ());
        return ParseCallExpr ();
    }
    else if (!Peek (Token :: DOT))
    {
        LOG(std::string ("id ") + m_Lexer.GetCurrTokText ());
        return ParseIdentifierExpr ();
    }
    Expr * object = ParseIdentifierExpr (), * attr;

    Eat (Token :: DOT);
    attr = ParseAttributeAccessExpr ();

    return (new AttributeAccessExpr (object, attr));
}

Expr* Parser::ParseIdentifierExpr()
{
    std::string s = m_Lexer.GetValue();
    Eat(Token::IDENTIFIER);
    
    return (new IdentifierExpr(s));
}

Expr* Parser::ParseCallExpr()
{
    std::string func_name(m_Lexer.GetValue());
    Eat(Token::IDENTIFIER);
    Eat(Token::L_PAREN);

    std::vector<Expr*> args;
    while(!Match(Token::R_PAREN))
    {
        args.push_back(Expression());
        if(!Match(Token::R_PAREN))
        {
            Eat(Token::COMMA);
        }
    }
    Eat(Token::R_PAREN);
    return (new CallExpr(func_name, args));
}

Expr * Parser::ParseMethodCallExpr ()
{
    // Expr * idfier_expr = (new IdentifierExpr (m_Lexer.GetValue()));
    // Eat (Token :: IDENTIFIER);
    // Eat (Token :: DOT);
    // return (new MethodCallExpr (idfier_expr, ParseFuncCallExpr ()));
}

Stmt* Parser::Statement()
{
    if (Match(Token::IDENTIFIER))
    {
        if(Peek(Token::EQ))
        {
            return ParseVarDecl();
        }
        else if (Peek(Token::L_PAREN))
        {
            return ParseCallStmt();
        }
        else if (Peek (Token::DOT))
        {
            Expr * access = ParseAttributeAccessExpr ();
            if (Match (Token::EQ))
            {
                Eat (Token::EQ);
                Expr * val = Expression ();
                Eat (Token::SEMICOLON);
                return (new AttributeVarDeclStmt (access, val));
            }
            Eat (Token :: SEMICOLON);
            return (new ExprStmt (access));
            // return (new VarDeclStmt (expr, ParseAttributeAccessExpr ()));
        }
    }
    else if(Match(Token::WHILE))
    {
        return ParseWhileStmt();
    }
    else if (Match(Token::FUNC))
    {
        return ParseFuncDeclStmt();
    }
    else if (Match(Token::L_BRACE))
    {
        return ParseBlockStmt({});
    }
    else if(Match(Token::RETURN))
    {
        return ParseReturnStmt();
    }
    else if(Match(Token::IF))
    {
        return ParseCondStmt();
    }
    else if (Match (Token::STRUCT))
    {
        return  ParseStructDecl ();
    }
    else
    {
        if (Match (Token :: SEMICOLON ))
        {
            Advance ();
            return nullptr;
        }
        return ParseExprStmt();
        // throw SyntaxException(SyntaxError::UnexpectedToken, m_CurrentToken);
        std::stringstream ss;
        ss << "Unexpected Token : '" << m_Lexer.GetCurrTokText() << "'\n";
        ThrowError (SyntaxError :: UnexpectedToken , ss);
    }

}

Stmt* Parser::ParseExprStmt()
{
    Expr * expr = Expression ();
    Eat (Token::SEMICOLON);
    return (new ExprStmt (expr));
}

Stmt* Parser::ParseReturnStmt()
{
    Eat(Token::RETURN);

    Expr * expr = Expression ();
    Eat (Token::SEMICOLON);

    return (new ReturnStmt(expr));
}

Stmt* Parser::ParseCondStmt()
{
    Eat(Token::IF);
    Expr* cond_expr = Expression();
    Stmt* if_block = ParseBlockStmt({Token::FUNC, Token::STRUCT}), *else_block = nullptr;

    if(Match(Token::ELSE))
    {
        Advance();
        else_block = ParseBlockStmt({Token::FUNC, Token::STRUCT});
    }

    return (new CondStmt(cond_expr, if_block, else_block));
}

Stmt* Parser::ParseCallStmt ()
{
    std::string func_name = m_Lexer.GetValue();    
    Eat(Token::IDENTIFIER);
    Eat(Token::L_PAREN);

    std::vector<Expr*> args;
    while(!Match(Token::R_PAREN))
    {
        args.push_back(Expression());
        if(!Match(Token::R_PAREN))
        {
            Eat(Token::COMMA);
        }
    }
    Eat(Token::R_PAREN); Eat (Token::SEMICOLON);

    return (new CallStmt(new CallExpr(func_name, args)));
}

Stmt* Parser::ParseWhileStmt()
{
    Eat(Token::WHILE);
    Eat(Token::L_PAREN);
    Expr* condition = Expression();
    Eat(Token::R_PAREN);

    return (new WhileStmt(condition, ParseBlockStmt({Token::FUNC, Token::STRUCT})));
}

Stmt* Parser::ParseVarDecl()
{
    std::string var_name = std::string(m_Lexer.GetValue());
    Eat(Token::IDENTIFIER);
    Eat(Token::EQ);
    
    Expr * expr = Expression ();
    Eat (Token::SEMICOLON);
    return (new VarDeclStmt(var_name, expr));
}


Stmt* Parser::ParseFuncDeclStmt()
{
    Eat (Token::FUNC);
    Eat (Token::IDENTIFIER);
    std::string func_name = m_Lexer.GetValue();

    
    Eat(Token::L_PAREN);

    std::vector<std::string> args;
    while(!Match(Token::R_PAREN))
    {
        args.push_back(m_Lexer.GetValue());
        Eat(Token::IDENTIFIER);
        if(Match(Token::COMMA))
        {
            Eat(Token::COMMA);
        }
    }

    Eat(Token::R_PAREN);

    Stmt * func_block = ParseBlockStmt ({Token::FUNC, Token::STRUCT});

    return (new FuncDeclStmt(func_name, args, func_block));
}

Stmt* Parser::ParseBlockStmt(std::unordered_set <Token> const& ignores)
{
    Eat(Token::L_BRACE);
    std::vector<Stmt*> block;
    while(!Match(Token::R_BRACE))
    {
        if (ignores.find (m_CurrentToken) != ignores.end())
        {
            std::stringstream ss;
            ss << "Unexpected Token: '" << m_Lexer.GetCurrTokText () << "'\n";
            ThrowError (SyntaxError :: UnexpectedToken , ss);
        }
        block.push_back(Statement());
        // if(!Match (Token::R_BRACE))
        // {
        //     Eat(Token::SEMICOLON);
        // }
    }
    Eat(Token::R_BRACE);
    
    return (new BlockStmt(block));
}


Stmt* Parser::ParseStructDecl ()
{
    Eat (Token::STRUCT);
    Var struct_name = std::string(m_Lexer.GetValue());
    Eat (Token::IDENTIFIER);

    Eat (Token::L_BRACE);
    // std::vector<std::unique_ptr<IdentifierExpr>> attributes;
    // // std::unique_ptr<ConstructorDeclStmt> constructor;
    std::vector <FuncDecl *> methods;
    FuncDecl * ctor_decl = nullptr;
    while (!Match(Token::R_BRACE))
    {
        if (Match (Token::FUNC))
        {
            methods.emplace_back (static_cast<FuncDeclStmt*> (ParseFuncDeclStmt ()) -> m_FuncDecl);
        }
        if (Match (Token::CONSTRUCTOR))
        {
            ctor_decl = (static_cast <FuncDeclStmt *> (ParseConstructorDeclStmt())) -> m_FuncDecl;
            methods.emplace_back (ctor_decl);
        }
    }
    Eat (Token::R_BRACE);
    return (new StructDeclStmt (struct_name, methods, ctor_decl));
    // return (new StructDeclStmt (std::string(value), nullptr, attributes));
}

Stmt* Parser::ParseStructBlockStmt ()
{
    Eat (Token::L_BRACE);
    // std::vector<std::unique_ptr<IdentifierExpr>> attributes;
    // std::unique_ptr<ConstructorDeclStmt> constructor;

    // while (!Match(Token::R_BRACE))
    // {
    //     if (Match(Token::IDENTIFIER))
    //     {
    //         attributes.emplace_back(ParseIdentifierExpr());
    //         if ( Match (Token::COMMA) )
    //         {
    //             Eat(Token::COMMA);
    //         }
    //         if (Match(Token::CONSTRUCTOR))
    //         {
                
    //         }

    //     }
    // }
    // Eat (Token::R_BRACE);
    // return (new StructDeclStmt ())
}

Stmt* Parser::ParseConstructorDeclStmt ()
{
    Eat (Token::CONSTRUCTOR);
    Eat (Token::L_PAREN);

    std::vector<std::string> args;
    while (!Match (Token::R_PAREN))
    {
        args.emplace_back (m_Lexer.GetValue());
        Eat(Token::IDENTIFIER);
        if(Match(Token::COMMA))
        {
            Eat(Token::COMMA);
        }
    }

    Eat(Token::R_PAREN);

    return (new FuncDeclStmt (std::string ("constructor"), args, ParseBlockStmt ({Token::STRUCT, Token::FUNC})));
}

// Stmt * Parser::ParseSelfVarDecl ()
// {
//     std::string var_name = std::string(m_Lexer.GetValue());
//     Eat(Token::IDENTIFIER);
//     Eat(Token::EQ);

//     return (new SelfVarDeclStmt (var_name, Expression ()));
// }

void Parser::Eat(Token expected)
{
    if(m_CurrentToken == expected)
    {
        Advance();
        return;
    }
    std::stringstream ss;
    SyntaxError se;
    if (m_CurrentToken == Token :: INVALID)
    {
        ss << "Invalid Token: '" << m_Lexer.GetCurrTokText () << "'\n";
        se = SyntaxError :: InvalidToken;
    }
    else
    {
        ss << "Expected: '" << expected << "'\n";
        se = SyntaxError :: UnexpectedToken;
    }
    ThrowError (se, ss);
}