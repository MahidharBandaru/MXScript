#pragma once

#include <map>
#include "Token.hh"

std::map<std::string, Token> const keywords = 
{
    std::make_pair(std::string("func"), Token::FUNC),
    std::make_pair(std::string("return"), Token::RETURN),
    std::make_pair(std::string("let"), Token::LET),
    std::make_pair(std::string("if"), Token::IF),
    std::make_pair(std::string("else"), Token::ELSE),
    std::make_pair(std::string("and"), Token::AND),
    std::make_pair(std::string("or"), Token::OR),
    std::make_pair(std::string("while"), Token::WHILE),
    std::make_pair(std::string("break"), Token::BREAK),
    std::make_pair(std::string("is"), Token::OP_DEQ),
};