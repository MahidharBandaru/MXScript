#pragma once
#include "Var.hh"

#include <string>
#include <map>


struct Env 
{
    Env() {}
    Env(std::initializer_list<std::pair<std::string, Var>> l)
        : m_Env(l.begin(), l.end()) {}

    auto begin() const noexcept
    {
        return m_Env.begin();
    }
    auto find(std::string const& s) const noexcept
    {
        return m_Env.find(s);
    }
    auto end() const noexcept
    {
        return m_Env.end();
    }
    auto at(std::string const& s) const noexcept
    {
        return m_Env.at(s);
    }
    // Var& operator[](std::string& s)
    // {
    //     return m_Env.at(s);
    // }
    void insert(std::pair<std::string, Var> p) noexcept
    {
        m_Env.insert_or_assign(p.first, p.second);
    }
    size_t size() const noexcept
    {
        return m_Env.size();
    }
    void erase(std::string& s) noexcept
    {
        m_Env.erase(s);
    }
private:
    std::map<std::string, Var> m_Env;
    
};