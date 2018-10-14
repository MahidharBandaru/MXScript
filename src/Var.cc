#include "Var.hh"

#include "TypeException.hh"
#include <iostream>

#define LOG(x) std::cout << x << std::endl;

// typedef std::variant<std::monostate, bool, int, double, std::string, std::shared_ptr<> holder;
using std::get;
using std::holds_alternative;

Var::Var()
    : m_CurrentType (Type::None) {}

Var::Var(bool b)
    : m_CurrentType(Type::Bool), m_value(b) {}

Var::Var(int i)
    : m_CurrentType(Type::Int), m_value(i) {}

Var::Var(double d)
    : m_CurrentType(Type::Double), m_value(d) {}

Var::Var(std::string s)
    : m_CurrentType(Type::String), m_value(s) {}

// Var(std::shared_ptr<std::vector<Var> > pv) : m_value(pv) {}

Var::operator bool() const{
    switch (m_CurrentType) {
        case Type::Bool : {
            return get<bool> (m_value);
        }
        case Type::Int : {
            return get<int> (m_value) == 0 ? false : true;
        }
        case Type::Double : {
            return get<double> (m_value) == 0 ? false : true;
        }
        case Type::String : {
            return get<std::string> (m_value).size() == 0 ? false : true;
        }
    }
}

Var::operator int() const {
    switch (m_CurrentType) {
        case Type::Bool : {
            return std::get<bool>(m_value) == true  ;
        }
        case Type::Int : {
            return get<int> (m_value) == 0 ? false : true;
        }
        case Type::Double : {
            return get<double> (m_value) == 0 ? false : true;
        }
        case Type::String : {
            return get<std::string> (m_value).size() == 0 ? false : true;
        }
    }
}
Var::operator std::string() const {
    switch (m_CurrentType) {
        case Type::Bool : {
            return std::get<bool>(m_value) == true ? std::string("true") : std::string("false");
        }
        case Type::Int : {
            return (std::to_string(std::get<int>(m_value)));
        }
        case Type::Double : {
            return (std::to_string(std::get<double>(m_value)));
        }
        case Type::String : {
            return std::get<std::string>(m_value);
        }
    }
}


bool Var::operator==(const Var& other) {
    return (m_value == other.m_value);
}
bool Var::operator!=(const Var& other) {
    return !(*this == other);
}
bool Var::operator<(const Var& other)
{
    return m_value < other.m_value;
}
bool Var::operator<=(const Var& other)
{
    return m_value <= other.m_value;
}
Var& Var::operator+=(char c) {
    if(m_CurrentType != Type::String) {
        //throw TypeError;
    } else {
        get<std::string>(m_value) += c;
        return *this;
    }
}
Var& Var::operator+(const Var& other)
{
    if(m_CurrentType != other.m_CurrentType) {
        throw TypeException(m_CurrentType, other.m_CurrentType);
        throw std::runtime_error("Types not fit");
    }
    switch(m_CurrentType) {
        case Type::Int : {
            std::get<int>(m_value) += std::get<int>(other.m_value);
            break;
        }
        case Type::Double : {
            std::get<double>(m_value) += std::get<double>(other.m_value);
            break;
        }
        case Type::String : {
            std::get<std::string>(m_value) += std::get<std::string>(other.m_value);
            break;
        }
        default: break;
    }
    return *this;
}

Var& Var::operator-(const Var& other) {
    if(m_CurrentType != other.m_CurrentType) {
        throw TypeException(m_CurrentType, other.m_CurrentType);
        throw std::runtime_error("Types not fit");
    }
    switch(m_CurrentType) {
        case Type::Int : {
            std::get<int>(m_value) -= std::get<int>(other.m_value);
            break;
        }
        case Type::Double : {
            std::get<double>(m_value) -= std::get<double>(other.m_value);
            break;
        }
        default: throw std::runtime_error("- not supported on current types");
    }
    return *this;
}

Var& Var::operator*(const Var& other) {
    if(m_CurrentType != other.m_CurrentType) {
        throw TypeException(m_CurrentType, other.m_CurrentType);
        throw std::runtime_error("Types not fit");
    }
    switch(m_CurrentType) {
        case Type::Int : {
            std::get<int>(m_value) *= std::get<int>(other.m_value);
            break;
        }
        case Type::Double : {
            std::get<double>(m_value) *= std::get<double>(other.m_value);
            break;
        }
        default: throw std::runtime_error("* not supported on current types");
    }
    return *this;
}

Var& Var::operator/(const Var& other) {
    // CATCH DIVISION BY ZERO!
    if(m_CurrentType != other.m_CurrentType) {
        throw TypeException(m_CurrentType, other.m_CurrentType);
        throw std::runtime_error("Types not fit");
    }
    switch(m_CurrentType) {
        case Type::Int : {
            std::get<int>(m_value) /= std::get<int>(other.m_value);
            break;
        }
        case Type::Double : {
            std::get<double>(m_value) /= std::get<double>(other.m_value);
            break;
        }
        default: throw std::runtime_error("/ not supported on current types");
    }
    return *this;
}