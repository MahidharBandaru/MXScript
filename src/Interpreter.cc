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


Interpreter::Interpreter()
    : m_GEnv {
        {"print", new BuiltInPrint()},
        {"println", new BuiltInPrintLn()},
        {"input", new BuiltInInput()},
        {"int", new BuiltInInt()},
        {"str", new BuiltInStr()},
        {"sqrt", new BuiltInSqrt()}
    } {}

void Interpreter::Evaluate(std::string & s)
{
    Lexer l(s);
    Parser p(l);
    
    try
    {
        auto st = p.Parse();
        for (auto & e : st)
        {
            Execute (e);
        }
        // CHANGE
        for (auto & e : st)
        {
            delete e;
        }
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

void Interpreter::visit_UnaryExpr(UnaryExpr * unary_expr)
{
    Var val = Eval (unary_expr->m_Expr);
    switch (unary_expr->m_Op)
    {
        case Token::OP_ADD : {
            m_ExprVal = val;
            return;
        }
        case Token::OP_SUB : {
            m_ExprVal = -1 * int(val);
        }
    }
}

void Interpreter::visit_IdentifierExpr (IdentifierExpr* identifier_expr)
{
    std::string identifier_name = identifier_expr->m_IdentifierName;
    auto loc = m_Env.find (identifier_name);
    auto glo = m_GEnv.find (identifier_name);
    if (loc == m_Env.end() && glo == m_GEnv.end()) {
        throw SemanticException(SemanticError::UndeclaredIdentifier);
    }
    if(loc == m_Env.end())
    {
        if(glo == m_GEnv.end()) throw SemanticException (SemanticError::UndeclaredIdentifier);
        m_ExprVal = glo->second;
    }
    else
    {
        m_ExprVal = loc->second;
    }
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

void Interpreter::visit_CallExpr(CallExpr* func_call_expr)
{
    std::string func_name = func_call_expr->m_CallName;
    
    if(m_GEnv.find(func_name) == m_GEnv.end())
    {
        throw SemanticException(SemanticError::UndeclaredFunction);
    }

    m_ExprVal = visit_Callable(m_GEnv.at (func_name), func_call_expr->m_Args);
}

Var Interpreter::visit_Callable(Callable * function, std::vector<Expr*>& args)
{
    auto signature = function->m_Decl->GetSignature();

    if( signature.size() != args.size())
    {
        throw SemanticException(SemanticError::UnknownOperation);
    }

    // Env temp = m_Env;
    Env func_env = m_GEnv;
    auto it1 = signature.begin(); auto it2 = args.begin();
    for(; it1 != signature.end(); it1++, it2++)
    {
        std::string sig =*it1;
        Var arg = Eval(*it2);
        func_env.insert(std::make_pair(sig, arg));
    }

    Var res = Var();
    Callable * c = function; //((Callable*)(m_GEnv.at(func_name)));
    Env temp = m_Env;
    m_Env = func_env;
    try
    {
        res =  c->call(this);

    } catch (std::bad_variant_access& e)
    {
        LOG("Object not callable")
        LOG(e.what())
    } catch (TypeException& e)
    {
        LOG("Bad Types");
    }
    m_Env = temp;
    
    return res;
}

void Interpreter::visit_AttributeAccessExpr (AttributeAccessExpr * attr_access_expr)
{
    Var object = Eval (attr_access_expr -> m_Object);
    try {
        auto func = static_cast<CallExpr *> (attr_access_expr->m_Attribute);
        m_ExprVal = ((Object *)object) -> call (this, func);
    } catch (std::out_of_range & e)
    {
        try {
            m_ExprVal = ((Object *)object)->call (this, static_cast<IdentifierExpr*> (attr_access_expr->m_Attribute));
        } catch (std::exception & e)
        {
            LOG(e.what());
        }
    }
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
        // v.Print();
    }
    // delete expr_stmt;
    // std::cout << std::endl;
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
    // delete while_stmt->m_Block;
}

void Interpreter::visit_ReturnStmt (ReturnStmt* return_stmt)
{
    Var res;
    if(return_stmt->m_RetExpr)  res = Eval (return_stmt->m_RetExpr);
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
}

void Interpreter::visit_VarDeclStmt(VarDeclStmt* var_decl_stmt) {
    var_decl_stmt->m_VarDecl->Declare(this);
}

void Interpreter::visit_FuncDeclStmt(FuncDeclStmt* func_decl_stmt)
{
    func_decl_stmt->m_FuncDecl->Declare(this);
}

void Interpreter::visit_StructDeclStmt(StructDeclStmt* struct_decl_stmt)
{
    struct_decl_stmt->m_StructDecl->Declare(this);
}

void Interpreter::visit_CallStmt (CallStmt* call_stmt)
{
    Eval(call_stmt->m_CallExpr); //->Eval(this);
    // delete func_call_stmt->m_FuncCallExpr;
}

void Interpreter::visit_AttributeVarDeclStmt (AttributeVarDeclStmt * attr_decl)
{
    attr_decl->m_SelfVarDecl->Declare (this);
}






/*
--------------------------------------------------------------------------------------------------------------------------------------------------------
                                                                    DECL

--------------------------------------------------------------------------------------------------------------------------------------------------------
*/

void Interpreter::visit_VarDecl(VarDecl* var_decl)
{
    Var val = Eval(var_decl->m_Expr);
    if(val.IsCallable())
    {
        m_GEnv.insert ({
            var_decl->m_VarName,
            val
        });
    }
    else
    {
        m_Env.insert ({
            var_decl->m_VarName,
            val
        });
    }

}

void Interpreter::visit_FuncDecl(FuncDecl* func_decl)
{
    m_GEnv.insert({
        func_decl->m_FuncName,
        new Function(func_decl)
    });
}


void Interpreter::visit_StructDecl (StructDecl * struct_decl)
{
    m_GEnv.insert ({struct_decl->m_StructName,
                   new Struct (struct_decl)});
}

void Interpreter::visit_AttributeVarDecl (AttributeVarDecl * attr_var_decl)
{
    auto access = static_cast<AttributeAccessExpr *> (attr_var_decl -> m_Access);
    Var ob = Eval (access->m_Object);
    auto idfier_expr = static_cast<IdentifierExpr *> (access->m_Attribute);
    std::string attr= idfier_expr -> m_IdentifierName;
    
    Var val = Eval (attr_var_decl -> m_Expr);
    try
    {
        ((Object *) ob) -> set (this, attr, val);
    }
    catch (std::exception & e)
    {
        LOG("NOT AN OBJECT");
    }
}