#pragma once

#include "Token.hh"
#include "Var.hh"

#include <string>

struct Position {
    size_t LineNo = 1, ColNo = 0;
};

struct Lexer {
    
    explicit Lexer(std::string & s) noexcept;

    Lexer (Lexer const &) = default;
    Lexer& operator=(Lexer const &) = delete;

    ~Lexer() = default;

    Token Read() noexcept;
    Token Peek() noexcept;
    Var GetValue() const noexcept;
    Position GetPosition () const noexcept;
    std::string GetCurrTokText () const noexcept;
    inline bool AtEnd () const noexcept;

private:
    Var m_value;
    std::string m_Src;

    const size_t m_end;
    size_t m_current, m_Prev;
    Position m_Position;

    void SkipWhiteSpace() noexcept;
    void ReadLineComment () noexcept;
    void ReadBlockComment () noexcept;

    Token ReadString() noexcept;
    Token ReadNumber() noexcept;
    Token ReadIdentifier() noexcept;

    inline void Advance() noexcept;
    bool Peek(char c) noexcept;
    char GetCurrentChar () const noexcept;
};