#include "Lexer.hh"
#include "Keywords.hh"

#define LOG(x)  std::cout << x << std::endl;

Lexer::Lexer (std::string& s) noexcept
    :  m_Src(std::move(s)), m_end(m_Src.size() + 4)
{
    m_Src.insert (m_Src.begin(), {'{', '\n'});
    m_Src.insert (m_Src.end(), {'\n', '}'});
    m_current = 0, m_LineNo = 0, m_Pos = 0;
}

void Lexer::ReadLineComment () noexcept
{
    while (m_current < m_end && m_Src.at(m_current) != '\n')    {Advance();}
}
void Lexer::ReadBlockComment () noexcept
{

}

std::pair <size_t, size_t> Lexer::GetCursor () const noexcept
{
    return  {m_LineNo, m_Pos};
}

std::string Lexer::GetCurrTokText () const noexcept
{
    return m_CurrTok;
}

Token Lexer::Peek() noexcept
{
    size_t curr_pos = m_current, pos = m_Pos, line_no = m_LineNo;
    Var curr_val = m_value;

    Token next = Read();
    
    m_current = curr_pos, m_Pos = pos, m_LineNo = line_no;
    m_value = curr_val;
    
    return next;
}

Token Lexer::Read() noexcept
{
    m_CurrTok = "";
    SkipWhiteSpace();
    if (m_current >= m_end)
    {
        return  Token::EOL;
    }

    char c = m_Src[m_current];
    if (c == '#')
    {
        ReadLineComment ();
        m_Pos = 0;
    }

    if (c == '\n')
    {
        Advance ();
        m_LineNo++;
        m_Pos = 0;
        return Read ();
    }
    if (AtEnd ())
    {
        return  Token::EOL;
    }

    c = m_Src[m_current];
    switch (c)
    {
        case '+' : {
            Advance(); m_CurrTok = "+";
            return Token::OP_ADD;
        }
        case '-' : {
            Advance(); m_CurrTok = "-";
            return Token::OP_SUB;
        }
        case '*' : {
            Advance(); m_CurrTok = "*";
            return Token::OP_MUL;
        }
        case '/' : {
            Advance(); m_CurrTok = "/";
            return Token::OP_DIV;
        }
        case '=' : {
            if(Peek('=')) {
                Advance();
                Advance(); m_CurrTok = "==";
                return Token::OP_DEQ;
            }
            Advance(); m_CurrTok = "=";
            return Token::EQ;
        }
        case '<' : {
            if(Peek('=')) {
                Advance(); Advance();  m_CurrTok = "<=";
                return Token::OP_LTE;
            }
            Advance(); m_CurrTok = "<";
            return Token::OP_LT;
        }
        case '!' : {
            if(Peek('=')) {
                Advance(); Advance(); m_CurrTok = "!=";
                return Token::OP_NEQ;
            }
            Advance(); m_CurrTok = "!";
            return Token::INVALID;
        }
        case '>' : {
            if(Peek('=')) {
                Advance(); Advance(); m_CurrTok = ">=";
                return Token::OP_GTE;
            }
            Advance(); m_CurrTok = ">";
            return Token::OP_GT;
        }
        case '(': {
            Advance(); m_CurrTok = "(";
            return Token::L_PAREN;
        }
        case ')': {
            Advance(); m_CurrTok = ")";
            return Token::R_PAREN;
        }
        case '{': {
            Advance(); m_CurrTok = "{";
            return Token::L_BRACE;
        }
        case '\'': {
            return ReadString();
        }
        case '}': {
            Advance(); m_CurrTok = "}";
            return Token::R_BRACE;
        }
        case ',': {
            Advance(); m_CurrTok = ",";
            return Token::COMMA;
        }
        case ';' : {
            Advance(); m_CurrTok = ";";
            return Token::SEMICOLON;
        }
        case '.' : {
            Advance(); m_CurrTok = ".";
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

inline void Lexer::Advance() noexcept {m_current++, m_Pos++;}

void Lexer::SkipWhiteSpace() noexcept
{
    while(!AtEnd () &&  m_Src[m_current] == ' ') {Advance();}
}

inline bool Lexer::AtEnd () noexcept {return (m_current >= m_end);}

bool Lexer::Peek(const char c) noexcept
{
    return AtEnd() ? false : (c == m_Src.at(m_current+1));
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
        Advance();
    }
    m_value = Var(i);
    m_CurrTok = std::string(m_value);
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
    m_CurrTok = s;
    auto it = keywords.find(s);
    if(it == keywords.end()) {
        m_value = Var(s);
        return Token::IDENTIFIER;
    }

    return it->second;
}

Var Lexer::GetValue() const noexcept {return m_value;}