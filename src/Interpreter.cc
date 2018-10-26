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
#include "Object.hh"

#define LOG(x) std::cout << x << std::endl;

StructDecl * structtest = new StructDecl (std::string("Struct")
                                        );


Interpreter::Interpreter()
    : m_Env {
        {"print", new BuiltInPrint()},
        {"println", new BuiltInPrintLn()},
        {"input", new BuiltInInput()},
        {"int", new BuiltInInt()},
        {"str", new BuiltInStr()},
        // {"Struct", new Struct (structtest)}
    } {
        Struct * test = new Struct (structtest);
        (test -> m_Methods) . insert ({std::string("bark"), new BuiltInPrintLn()});
        m_Env.insert ({"Struct", test});

    }

void Interpreter::Evaluate(std::string const& s)
{
    Lexer l(s);
    Parser p(l);
    
    try
    {
        Stmt* st = p.Parse();
        Execute(st);
        // delete st;
    }
    catch (const SyntaxException& e) 
    {
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

void Interpreter::visit_LiteralExpr(LiteralExpr* literal_expr)
{
    m_ExprVal = literal_expr->m_Value;
}

void Interpreter::visit_IdentifierExpr (IdentifierExpr* identifier_expr)
{
    std::string identifier_name = identifier_expr->m_IdentifierName;
    if (m_Env.find(identifier_name) == m_Env.end()) {
        throw SemanticException(SemanticError::UndeclaredVariable);
    }
    m_ExprVal = m_Env.at(identifier_name);
}

void Interpreter::visit_BinaryExpr(BinaryExpr* binary_expr)
{

    // HANDLE VAR EXCEPTIONS!!

    Var left = Eval(binary_expr->m_Left);
    Var right = Eval(binary_expr->m_Right);

    Token op = binary_expr->m_Op;
    switch(op) {
        case (Token::OP_ADD) : {
            m_ExprVal = left+right;
            break;
        }
        case (Token::OP_SUB) : {
            m_ExprVal = left-right;
            break;
        }
        case (Token::OP_MUL) : {
            m_ExprVal = left*right;
            break;
        }
        case (Token::OP_DIV) : {
            m_ExprVal = left/right;
            break;
        }
        case (Token::OP_DEQ) : {
            m_ExprVal = left == right;
            break;
        }
        case Token::OR : {
            m_ExprVal = bool(left) || bool(right);
            break;
        }
        case Token::AND : {
            m_ExprVal = bool(left) && bool(right);
            break;
        }
        case (Token::OP_NEQ) : {
            m_ExprVal = left != right;
            break;
        }
        case (Token::OP_LT) : {
            m_ExprVal = left < right;
            break;
        }
        case (Token::OP_LTE) : {
            m_ExprVal = left <= right;
            break;
        }
        case (Token::OP_GT) : {
            m_ExprVal = !(left <= right);
            break;
        }
        case (Token::OP_GTE) : {
            m_ExprVal = !(left < right); //left >= right;
            break;
        }
        default : {
            throw SemanticException(SemanticError::UnknownOperation);
        }
    }
}

void Interpreter::visit_GroupExpr(GroupExpr* group_expr)
{
    m_ExprVal = Eval(group_expr->m_Expr);
}

void Interpreter::visit_FuncCallExpr(FuncCallExpr* func_call_expr)
{
    std::string func_name = func_call_expr->m_FuncName;
    
    if(m_Env.find(func_name) == m_Env.end())
    {
        throw SemanticException(SemanticError::UndeclaredFunction);
    }
    if(!m_Env.at(func_name).IsCallable())
    {
        std::cout << "Error: '" << func_name << "' " << "Object not callable" << std::endl;
        m_ExprVal = Var();
        return;
    }

    auto function = (Callable*)m_Env.at(func_name);
    auto signature = function->m_Decl->GetSignature();
    
    auto& args = func_call_expr->m_Args;


    if( signature.size() != args.size())
    {
        throw SemanticException(SemanticError::UnknownOperation);
    }

    Env temp = m_Env;

    auto it1 = signature.begin(); auto it2 = args.begin();
    for(; it1 != signature.end(); it1++, it2++)
    {
        std::string sig =*it1;
        Var arg = Eval(*it2);
        
        m_Env.insert(std::make_pair(sig, arg));
    }

    Var res = Var();
    try
    {
        res =  ((Callable*)(m_Env.at(func_name)))->call(this);

    } catch (std::bad_variant_access& e)
    {
        LOG("Object not callable")
        LOG(e.what())
    } catch (TypeException& e)
    {
        LOG("Bad Types");
    }
    m_Env = temp;
    // return res;
    m_ExprVal = res;
}

void Interpreter::visit_MethodCallExpr (MethodCallExpr * method_call)
{
    Var object = Eval (method_call -> m_Object);
    // Env temp = m_Env;
    // m_Env += ((Object * )object)->m_Env;
    // m_ExprVal = Eval (method_call->m_Method);
    // m_Env = temp;
    ((Object *)object)->call (this, (FuncCallExpr *)method_call->m_Method);
}

Var Interpreter::Eval(Expr* expr) {
    expr->Accept(this);
    return m_ExprVal;
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
    // Var v = expr_stmt->m_Expr->Eval(this);
    Var v = Eval(expr_stmt->m_Expr);
    if(v.IsCallable()) {
        // IGNORE
    }
    else if(!v.IsNone()) {
        v.Print();
    }
    // delete expr_stmt;
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
    while(bool(Eval(while_stmt->m_Condition)))
    {
        while_stmt->m_Block->Execute(this);
    }
    // delete while_stmt->m_Condition;
}

void Interpreter::visit_ReturnStmt (ReturnStmt* return_stmt)
{
    Var res = Eval(return_stmt->m_RetExpr);
    // delete return_stmt->m_RetExpr;
    throw ReturnException(res);
    
}

void Interpreter::visit_CondStmt (CondStmt* cond_stmt)
{
    bool cond = bool(Eval(cond_stmt->m_CondExpr));
    if(cond)
    {
        cond_stmt->m_IfBlock->Execute(this);
    }
    else if (cond_stmt->m_ElseBlock != nullptr)
    {
        cond_stmt->m_ElseBlock->Execute(this);
    }
    delete cond_stmt->m_CondExpr;
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
    Eval(func_call_stmt->m_FuncCallExpr); //->Eval(this);
    delete func_call_stmt->m_FuncCallExpr;
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
        Eval(var_decl->m_Expr)
    });
    delete var_decl->m_Expr;

}

void Interpreter::visit_FuncDecl(FuncDecl* func_decl)
{
    m_Env.insert({
        func_decl->m_FuncName,
        new Function(func_decl)
    });
}


void Interpreter::visit_StructDecl (StructDecl * struct_decl)
{
    m_Env.insert ({struct_decl->m_StructName,
                   new Struct (struct_decl)});
}