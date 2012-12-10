#pragma once

#include "expression.hpp"

#include <string>
#include <map>

class context
{
public:
    void put_variable(const std::string& name, const std::string& type);

    template <typename Type>
    lvalue_expression<Type>& get_variable(const std::string& name);
    
private:
    template <typename Type>
    void put_variable(const std::string& name);

    std::map<std::string, lvalue_expression<int>> int_vars;
};

#include "context.inl"