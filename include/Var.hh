#pragma once

#include <variant>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include "Type.hh"

#define LOG(x) std::cout << x << std::endl;

struct Callable;
struct Object;

struct Var {
    Var();
    Var(std::monostate);
    Var(bool);
    Var(int);
    Var(double);
    Var(std::string);
    Var(const char* const begin, const char* const end); // [....)

    Var (Callable * c);
    Var (Object * o) : m_value (o), m_CurrentType (Type::Object) {}

    Var(std::shared_ptr<std::vector<Var>> sv) : m_value(sv) {}
    Var(const Var& other) : m_value(other.m_value), m_CurrentType(other.m_CurrentType) {}
    
    operator bool() const;
    operator int() const;
    operator double() const {}
    operator std::string() const;
    operator Callable*() const {
        return std::get<Callable*>(m_value);
    }
    operator Object * () const
    {
        return std::get<Object *> (m_value);
    }
    bool operator==(const Var&);
    bool operator!= (const Var&);
    bool operator< (const Var&);
    bool operator<=(const Var&);

    Var& operator+=(char c);
    
    Var& operator+(const Var&);
    Var& operator-(const Var&);
    Var& operator*(const Var&);
    Var& operator/(const Var&);


    void Print() const{
        std::visit(PrintVisitor{}, m_value);
    }
    bool IsNone() {
        return m_CurrentType == Type::None;
    }
    bool IsCallable() {
        return m_CurrentType == Type::Callable;
    }
private:
    std::variant<std::monostate, bool, int, double, std::string,
                std::shared_ptr<std::vector<Var> >, Callable*, Object*> m_value;

    struct PrintVisitor {
        void operator()(std::monostate) const{
            std::cout << "nil";
        }
        void operator() (bool b) const {
            std::cout << ( b ? "true" : "false");
        }
        void operator()(int i) const {
            std::cout << i;
        }
        void operator()(double d) const {
            std::cout << d;
        }
        void operator()(std::string s) const {
            // std::cout << "'";
            std::cout << s;
            // std::cout << "'";
        }
        void operator()(std::shared_ptr<std::vector<Var>> vp) {
            std::cout << "[";
            for(auto&e : *vp) {
                std::visit(PrintVisitor{}, e.m_value);
                std::cout << ", ";
            }
            std::cout << "]";
        }
        void operator() (Callable * c)
        {
            std::cout << "<Function at " << c << ">";
        }
        void operator() (Object * o)
        {
            std::cout << "<Instance of ..?..>";
        }
    };

    
    
    Type m_CurrentType;
    
};