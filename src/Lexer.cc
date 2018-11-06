#include "Lexer.hh"
#include "Keywords.hh"

#define LOG(x)  std::cout << x << std::endl;

Lexer::Lexer (std::string& s) noexcept
    :  m_Src(std::move(s)), m_end(m_Src.size())
{
    m_current = 0, m_Prev = 0;
    m_Position = {1, 0};
}

void Lexer::ReadLineComment () noexcept
{
    while (!AtEnd () && GetCurrentChar () != '\n')    {Advance();}
}
void Lexer::ReadBlockComment () noexcept
{
    // TBI
}

Position Lexer::GetPosition () const noexcept
{
    return m_Position;
}

std::string Lexer::GetCurrTokText () const noexcept
{
    return m_Src.substr (m_Prev, m_current-m_Prev);
}

Token Lexer::Peek() noexcept
{
    size_t curr_pos = m_current, prev_pos = m_Prev;
    auto pos = m_Position;
    Var curr_val = m_value;

    Token next = Read();
    
    m_current = curr_pos;
    m_value = curr_val;
    m_Position = pos;
    m_Prev = prev_pos;

    return next;
}

Token Lexer::Read() noexcept
{
    
    SkipWhiteSpace();

    auto c = GetCurrentChar ();
    if (c == '#')
    {
        ReadLineComment ();
    }
    c = GetCurrentChar ();
    m_Prev = m_current;
    switch (c)
    {
        case '\0' : {
            return  Token :: EOL;
        }
        case '\n' : {
            Advance ();
            m_Position.ColNo = 0;
            m_Position.LineNo++;
            return  Read();
        }
        case '+' : {
            Advance();
            return Token::OP_ADD;
        }
        case '-' : {
            Advance();
            return Token::OP_SUB;
        }
        case '*' : {
            Advance();
            return Token::OP_MUL;
        }
        case '/' : {
            Advance();
            return Token::OP_DIV;
        }
        case '=' : {
            if(Peek('=')) {
                Advance();
                Advance();
                return Token::OP_DEQ;
            }
            Advance();
            return Token::EQ;
        }
        case '<' : {
            if(Peek('=')) {
                Advance(); Advance();
                return Token::OP_LTE;
            }
            Advance();
            return Token::OP_LT;
        }
        case '!' : {
            if(Peek('=')) {
                Advance(); Advance();
                return Token::OP_NEQ;
            }
            Advance();
            return Token::INVALID;
        }
        case '>' : {
            if(Peek('=')) {
                Advance(); Advance();
                return Token::OP_GTE;
            }
            Advance();
            return Token::OP_GT;
        }
        case '(': {
            Advance();
            return Token::L_PAREN;
        }
        case ')': {
            Advance();
            return Token::R_PAREN;
        }
        case '{': {
            Advance();
            return Token::L_BRACE;
        }
        case '}': {
            Advance();
            return Token::R_BRACE;
        }
        case '\'': {
            return ReadString();
        }
        case ',': {
            Advance();
            return Token::COMMA;
        }
        case ';' : {
            Advance();
            return Token::SEMICOLON;
        }
        case '.' : {
            Advance();
            return Token::DOT;
        }
        default: {
            if(isalpha(c)) {
                return ReadIdentifier();
            }
            else if(isdigit(c)) {
                return ReadNumber();
            }
            Advance ();
            return Token::INVALID;
        }
        
    }

}

inline void Lexer::Advance() noexcept {
    m_current++;
    m_Position.ColNo++;
}

void Lexer::SkipWhiteSpace() noexcept
{
    while(!AtEnd () && GetCurrentChar () == ' ') {Advance();}
}

inline bool Lexer::AtEnd () const noexcept {return (m_current >= m_end);}

bool Lexer::Peek(const char c) noexcept
{
    return AtEnd() ? false : (c == m_Src.at(m_current+1));
}

Token Lexer::ReadString() noexcept {
    Advance();
    m_value = Var(std::string(""));
    auto c = GetCurrentChar ();
    while(c != '\'') {
        m_value += c;
        Advance ();
        c = GetCurrentChar ();
    }
    Advance();
    return Token::STRING;
}

Token Lexer::ReadNumber() noexcept
{
    int i = 0;
    while(m_current < m_end && isdigit(m_Src[m_current])) {
        i = i*10 +  (m_Src[m_current] - '0');
        Advance();
    }
    m_value = Var(i);
    return Token::INT;
}

Token Lexer::ReadIdentifier() noexcept
{
    char c = GetCurrentChar ();
    std::string s;

    while( isalnum (c)
        ||  c == '_') {
            s += c;
            Advance ();
            c = m_Src[m_current];
        }
    auto it = keywords.find(s);
    if(it == keywords.end()) {
        m_value = Var(s);
        return Token::IDENTIFIER;
    }

    return it->second;
}

Var Lexer::GetValue() const noexcept {return m_value;}

char Lexer::GetCurrentChar () const noexcept
{
    if (AtEnd ())   return '\0';
    return m_Src[m_current];
}