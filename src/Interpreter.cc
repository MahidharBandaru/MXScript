#include "Interpreter.hh"

#include "Var.hh"
#include "Callable.hh"
#include "Lexer.hh"
#include "Parser.hh"
#include "Expr.hh"
#include "Stmt.hh"
#include "SyntaxException.hh"
#include "SemanticException.hh"
#include "TypeException.hh"
#include "Return.hh"
#include "Library.hh"

#define LOG(x) std::cout << x << std::endl;

Interpreter::Interpreter()
    : m_Env {
        {"print", new BuiltInPrint()},
        {"println", new BuiltInPrintLn()},
        {"input", new BuiltInInput()},
    } {}

void Interpreter::Evaluate(std::string const& s)
{
    Lexer l(s);
    Parser p(l);
    
    try
    {
        Stmt* st = p.Parse();
        Execute(st);
        delete st;
    }
    catch (const SyntaxException& e) 
    {
        // std::cout << "SyntaxExceptION " << e.what() << std::endl;
        const char * c = e.what();
        std::cout << c << std::endl;
        std::exit(EXIT_FAILURE);
    }
    catch (const SemanticException& e)
    {
        std::cout << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    catch(const TypeException& e)
    {
        std::cout << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
}

Var Interpreter::visit_LiteralExpr(LiteralExpr* literal_expr)
{
    return literal_expr->m_Value;
}

Var Interpreter::visit_IdentifierExpr (IdentifierExpr* identifier_expr)
{
    std::string identifier_name = identifier_expr->m_IdentifierName;
    if (m_Env.find(identifier_name) == m_Env.end()) {
        throw SemanticException(SemanticError::UndeclaredVariable);
    }
    return m_Env.at(identifier_name);
}

Var Interpreter::visit_BinaryExpr(BinaryExpr* binary_expr)
{

    // HANDLE VAR EXCEPTIONS!!

    Var left = Eval(binary_expr->m_Left);
    Var right = Eval(binary_expr->m_Right);

    Token op = binary_expr->m_Op;
    switch(op) {
        case (Token::OP_ADD) : {
            return left+right;
        }
        case (Token::OP_SUB) : {
            return left-right;
        }
        case (Token::OP_MUL) : {
            return left*right;
        }
        case (Token::OP_DIV) : {
            return left/right;
        }
        case (Token::OP_DEQ) : {
            return left == right;
        }
        case (Token::OP_NEQ) : {
            return left != right;
        }
        case (Token::OP_LT) : {
            return left < right;
        }
        case (Token::OP_LTE) : {
            return left <= right;
        }
        case (Token::OP_GT) : {
            return !(left <= right);
        }
        case (Token::OP_GTE) : {
            return !(left < right); //left >= right;
        }
        default : {
            throw SemanticException(SemanticError::UnknownOperation);
        }
    }
}

Var Interpreter::visit_GroupExpr(GroupExpr* group_expr)
{

    return Eval(group_expr->m_Expr);
}

Var Interpreter::visit_FuncCallExpr(FuncCallExpr* func_call_expr)
{
    std::string func_name = func_call_expr->m_FuncName;

    if(m_Env.find(func_name) == m_Env.end())
    {
        throw SemanticException(SemanticError::UndeclaredVariable);
    }
    // if(!m_Env.at(func_name).IsCallable())
    // {
    //     std::cout << "Object not callable" << std::endl;
    //     return Var();
    // }

    auto function = (Callable*)m_Env.at(func_name);
    auto& signature = function->m_FuncDecl->m_Signature;
    auto& args = func_call_expr->m_Args;


    if( signature.size() != args.size())
    {
        throw SemanticException(SemanticError::UnknownOperation);
    }

    Env temp = m_Env;
    auto it1 = signature.begin();
    auto it2 = args.begin();
    for(it1, it2; it1 != signature.end(); it1++, it2++)
    {

        std::string sig =*it1;
        Var arg = (*it2)->Eval(this);
        
        m_Env.insert(std::make_pair(sig, arg));
    }
    Var res;
    try
    {
        res =  ((Callable*)(m_Env.at(func_name)))->call(this);

    } catch(std::exception e)
    {
        std::cout << "Object not callable" << std::endl;
        res = Var();
    }
    m_Env = temp;
    return res;
}

Var Interpreter::Eval(Expr* expr) {
    return expr->Eval(this);
}




/*
--------------------------------------------------------------------------------------------------------------------------------------------------------
                                                                    STMT

--------------------------------------------------------------------------------------------------------------------------------------------------------
*/






void Interpreter::Execute(Stmt* s) {
    s->Execute(this);
}

void Interpreter::visit_ExprStmt(ExprStmt* expr_stmt) {
    Var v = expr_stmt->m_Expr->Eval(this);
    if(v.IsCallable()) {
        // IGNORE
    }
    else if(!v.IsNone()) {
        v.Print();
    }
    std::cout << std::endl;
}

void Interpreter::visit_BlockStmt(BlockStmt* block_stmt)
{
    // Any declaration must be removed.
    Env temp(m_Env);
    for(auto& e: block_stmt->m_Block)
    {
        e->Execute(this);
    }
    for(auto& e : m_Env)
    {
        std::string s = e.first;
        if(temp.find(s) == temp.end())
        {
            m_Env.erase(s);
        }
    }
    // m_Env = temp;
    
}

void Interpreter::visit_WhileStmt (WhileStmt* while_stmt)
{
    while(bool(while_stmt->m_Condition->Eval(this)))
    {
        while_stmt->m_Block->Execute(this);
    }
}

void Interpreter::visit_ReturnStmt (ReturnStmt* return_stmt)
{
    Var res = return_stmt->m_RetExpr->Eval(this);
    throw ReturnException(res);
    
}

void Interpreter::visit_CondStmt (CondStmt* cond_stmt)
{
    bool cond = bool(cond_stmt->m_CondExpr->Eval(this));
    if(cond)
    {
        cond_stmt->m_IfBlock->Execute(this);
    }
    else if (cond_stmt->m_ElseBlock != nullptr)
    {
        cond_stmt->m_ElseBlock->Execute(this);
    }
}

void Interpreter::visit_VarDeclStmt(VarDeclStmt* var_decl_stmt) {
    var_decl_stmt->m_VarDecl->Declare(this);
}

void Interpreter::visit_FuncDeclStmt(FuncDeclStmt* func_decl_stmt)
{
    func_decl_stmt->m_FuncDecl->Declare(this);
}

void Interpreter::visit_FuncCallStmt (FuncCallStmt* func_call_stmt)
{
    func_call_stmt->m_FuncCallExpr->Eval(this);
}








/*
--------------------------------------------------------------------------------------------------------------------------------------------------------
                                                                    DECL

--------------------------------------------------------------------------------------------------------------------------------------------------------
*/

void Interpreter::visit_VarDecl(VarDecl* var_decl)
{
    m_Env.insert ({
        var_decl->m_VarName,
        var_decl->m_Expr->Eval(this)
    });

}

void Interpreter::visit_FuncDecl(FuncDecl* func_decl)
{
    m_Env.insert({
        func_decl->m_FuncName,
        new Function(func_decl)
    });
}