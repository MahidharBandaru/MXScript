#include "Visitor.hh"
#include "Expr.hh"
#include "Var.hh"
#include "Decl.hh"
#include "Token.hh"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <map>

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder (TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::Value *> NamedValues;

struct IRCodeGen : public ExprVisitor, DeclVisitor
{
    void visit_LiteralExpr (LiteralExpr* literal_expr) override
    {
        // Only int literals.
        // m_Val = llvm::ConstantInt::get(TheContext, llvm::APInt(static_cast<int>(literal_expr->m_Value)));
    }
    void visit_IdentifierExpr (IdentifierExpr* identifier_expr) override
    {
        m_Val = NamedValues[identifier_expr->m_IdentifierName];
    }
    void visit_BinaryExpr (BinaryExpr* binary_expr) override
    {
        llvm::Value* L = Codegen (binary_expr->m_Left);
        llvm::Value* R = Codegen (binary_expr->m_Right);

        if (!L || !R)
        {
            m_Val = nullptr;
            return;
        }

        switch (binary_expr->m_Op)
        {
            case Token::OP_ADD : {
                m_Val = Builder.CreateFAdd(L, R, "addtmp");
                break;
            }
            case Token::OP_SUB : {
                m_Val = Builder.CreateFSub(L, R, "subtmp");
                break;
            }
            case Token::OP_MUL : {
                m_Val = Builder.CreateFMul(L, R, "multmp");
                break;
            }
            default : {

            }
                
        }
    }

    llvm::Value* Codegen(Expr* expr)
    {
        expr->Accept(this);
        return m_Val;
    }

    llvm::Value* m_Val;
















    //-------------------------------DeclCodeGen

    void visit_VarDecl (VarDecl* var_decl) override
    {
        NamedValues[var_decl->m_VarName] = Codegen(var_decl->m_Expr);
    }

};