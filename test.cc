#include <iostream>
#include <memory>

using namespace std;


struct Expr
{
    virtual void bark () = 0;
};

struct BinaryExpr : public Expr
{
    BinaryExpr (std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : l(std::move(l)), r(std::move(r)) {}

    virtual void bark () override
    {
        cout << "BinaryExpr" << endl;
    }
    ~BinaryExpr () {
        cout << "Deleting BinaryExpr" << endl;
    }
    std::unique_ptr<Expr> l, r;
};

struct LiteralExpr : public Expr
{
    virtual void bark () override
    {
        cout << "LiteralExpr" << endl;
    }
    ~LiteralExpr ()
    {
        cout << "Deleting Litrealexpr" << endl;
    }
};

void bark (std::unique_ptr<Expr> const& e)
{
    e->bark();
}

std::unique_ptr<Expr> get ()
{
    return std::make_unique<BinaryExpr>(make_unique<LiteralExpr>(), make_unique<LiteralExpr>());
}

int main()
{
    // std::unique_ptr<Expr> expr = std::make_unique<BinaryExpr> ();
    auto t = get();
    bark(get());
    return 0;
}