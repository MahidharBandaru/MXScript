#pragma once

enum class Token
{
    L_PAREN, R_PAREN, L_BRACE, R_BRACE, COMMA,
    
    // Arithemetic operators
    OP_SUB, OP_ADD, OP_DIV, OP_MUL, OP_EXP,

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