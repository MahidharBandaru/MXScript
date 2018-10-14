#include "Lexer.hh"
#include "Keywords.hh"

#define LOG(x)  std::cout << x << std::endl;

Lexer::Lexer(const std::string& s) noexcept
    : m_Src(s), m_end(s.size())
{
    m_current = 0;
}

Token Lexer::Peek() noexcept
{
    size_t curr_pos = m_current;
    Var curr_val = m_value;

    Token next = Read();
    
    m_current = curr_pos;
    m_value = curr_val;
    
    return next;
}

Token Lexer::Read() noexcept
{
    SkipWhiteSpace();
    if (m_current >= m_end)
    {
        return  Token::EOL;
    }

    const char c = m_Src[m_current];
    switch (c)
    {
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
        case '\'': {
            return ReadString();
        }
        case '}': {
            Advance();
            return Token::R_BRACE;
        }
        case ',': {
            Advance();
            return Token::COMMA;
        }
        default: {
            if(isalpha(c)) {
                return ReadIdentifier();
            }
            else if(isdigit(c)) {
                return ReadNumber();
            }
            return Token::INVALID;
        }
        
    }

}

inline void Lexer::Advance() noexcept {m_current++;}

void Lexer::SkipWhiteSpace() noexcept
{
    while(m_current < m_end &&  m_Src[m_current] == ' ') {m_current++;}
}

bool Lexer::Peek(const char c) noexcept
{
    return (m_current == m_end) ? false : (c == m_Src.at(m_current+1));
}

Token Lexer::ReadString() noexcept {
    Advance();
    m_value = Var(std::string(""));
    while(m_Src[m_current] != '\'') {
        m_value += m_Src[m_current];
        m_current++;
    }
    Advance();
    return Token::STRING;
}

Token Lexer::ReadNumber() noexcept
{
    int i = 0;
    while(m_current < m_end && isdigit(m_Src[m_current])) {
        i = i*10 +  (m_Src[m_current] - '0');
        m_current++;
    }
    m_value = Var(i);
    return Token::INT;
}

Token Lexer::ReadIdentifier() noexcept
{
    char c = m_Src[m_current];
    std::string s;

    while( (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        ||  c == '_') {
            s += c;
            m_current++;
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