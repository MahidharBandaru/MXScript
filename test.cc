#include <iostream>
#include <sys/types.h>

struct Position {
    constexpr Position (size_t line_no = 1, size_t col_no = 0)
        : m_LineNo (line_no), m_ColNo (col_no) {}
    constexpr size_t GetLineNo () const noexcept {return m_LineNo;}
    constexpr size_t GetColNo  () const noexcept {return m_ColNo;}

private:
    size_t m_LineNo = 1, m_ColNo = 0;
};

struct Lexer {
    struct Cursor {
        constexpr Cursor (size_t & current)
            : m_Current (current) {}
    private:
        size_t & m_Current;
        Position m_Pos;
        Cursor & operator++ ()
        {
            m_Current++;
            if (m_Src[m_Current] == '\n')
            {
                
            }
            return *this;
        }
    };


    std::string m_Src;
    Cursor m_c;

    friend Cursor;

};

int main ()
{
    return 0;
}