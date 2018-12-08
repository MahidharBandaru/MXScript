#ifndef TOKEN_HH
#define TOKEN_HH

#include <map>
#include <string>

enum class Token
{
    L_PAREN, R_PAREN, L_BRACE, R_BRACE, COMMA,
    
    // Arithemetic operators
    OP_SUB, OP_ADD, OP_DIV, OP_MUL, OP_EXP, OP_ADDEQ, OP_SUBEQ,

    // Comparators
    OP_LT, OP_LTE, OP_GT, OP_GTE,
    OP_NEQ, OP_DEQ,
    DEQ, EQ,
    
    // Types
    IDENTIFIER, NUMBER, STRING, INT, FLOAT,
    
    // KeyWords
    FUNC, RETURN, LET, IF, ELSE, IS, OR, AND, WHILE, BREAK, STRUCT, CONSTRUCTOR, SELF,
    
    //MISC
    SEMICOLON, DOT, EOL,

    INVALID
};

static std::map <Token, std::string> tok_table {
    {Token :: L_PAREN , std::string("(")},
    {Token :: R_PAREN, std::string(")")},
    {Token :: SEMICOLON , std::string(";")},
    {Token :: DOT , std::string(".")},
    {Token :: IDENTIFIER , std::string("id")},
};
static std::ostream & operator << (std::ostream & os, Token const& t)
{
    os << tok_table[t];
}


#endif