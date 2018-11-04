#pragma once

#include <string>
#include <vector>

#include "Token.hh"
#include "Var.hh"

struct Lexer {
    
    explicit Lexer(std::string & s) noexcept;

    Lexer (Lexer const &) = default;
    Lexer& operator=(Lexer const &) = delete;

    ~Lexer() = default;

    Token Read() noexcept;
    Token Peek() noexcept;
    Var GetValue() const noexcept;
    std::pair <size_t, size_t> GetCursor () const noexcept;
    std::string GetCurrTokText () const noexcept;

private:
    Var m_value;
    std::string m_Src, m_CurrTok;

    const size_t m_end;
    size_t m_current, m_LineNo, m_Pos;

    void SkipWhiteSpace() noexcept;
    void ReadLineComment () noexcept;
    void ReadBlockComment () noexcept;

    Token ReadString() noexcept;
    Token ReadNumber() noexcept;
    Token ReadIdentifier() noexcept;

    inline void Advance() noexcept;
    inline bool AtEnd () noexcept;
    bool Peek(char c) noexcept;
};