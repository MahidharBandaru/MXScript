#pragma once

#include <string>
#include <fstream>
#include <iostream>

struct FileMap
{
    explicit FileMap (std::string const& file_name)
    {
        std::ifstream ifs (file_name);
        
        if (ifs)
        {
            ifs.seekg (0, ifs.end);
            size_t size = ifs.tellg();
            ifs.seekg (0, ifs.beg);

            char * buffer = new char [size+1];
            buffer [size] = '\0';
            ifs.read (buffer, size);

            m_begin = buffer;
        }
    }
    FileMap (FileMap && f)
    {

    }
    
    char Get () const
    {
        return * (m_begin + m_pos);
    }
    
    size_t m_pos;
    char const * m_begin, * m_end;
};