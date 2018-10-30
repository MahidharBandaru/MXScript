#include "Decl.hh"
#include "Stmt.hh"


FuncDecl::FuncDecl(std::string func_name, std::vector<std::string> signature, Stmt* block)
        : m_FuncName(std::move(func_name)), m_Signature(std::move(signature)), m_Block(block) {}

void FuncDecl::Declare (DeclVisitor* dv)
{
    dv->visit_FuncDecl(this);
}

std::vector<std::string> FuncDecl::GetSignature () const
{
    return m_Signature;
}
FuncDecl::~FuncDecl ()
{
    delete m_Block;
}


// StructDecl

StructDecl::StructDecl (std::string struct_name, std::vector<FuncDecl *> & method_decl_stmts, FuncDecl* ctor_decl)
        : m_StructName (std::move (struct_name)), m_MethodDecls (std::move (method_decl_stmts)),
            m_CtorDecl (ctor_decl) {}

void StructDecl::Declare (DeclVisitor* dv)
{
    dv->visit_StructDecl (this);
}
StructDecl::~StructDecl ()
{
    for (auto & e : m_MethodDecls)
    {
        delete e;
    }
    if (m_CtorDecl)
        delete m_CtorDecl;
}
std::vector<std::string> StructDecl::GetSignature () const
{
    if(!m_CtorDecl) return  {};
    return m_CtorDecl->GetSignature();
}