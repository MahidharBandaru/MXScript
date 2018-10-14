#pragma once

#include <string>
#include <vector>

#include "Token.hh"
#include "Var.hh"

struct Lexer {
    
    explicit Lexer(std::string const& s) noexcept;

    Lexer (Lexer const &) = default;
    Lexer& operator=(Lexer const &) = delete;

    ~Lexer() = default;

    Token Read() noexcept;
    Token Peek() noexcept;
    Var GetValue() const noexcept;

private:
    Var m_value;
    std::string m_Src;

    const size_t m_end;
    size_t m_current;

    void SkipWhiteSpace() noexcept;
    Token ReadString() noexcept;
    Token ReadNumber() noexcept;
    Token ReadIdentifier() noexcept;

    inline void Advance() noexcept;
    bool Peek(char c) noexcept;
};