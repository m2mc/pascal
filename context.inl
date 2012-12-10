#include <stdexcept>

template <typename Type>
void context::put_variable(const std::string& name)
{
    throw std::invalid_argument("Unimplemented type of: " + name);
}

template<>
void context::put_variable<int>(const std::string& name)
{
    int_vars.insert(std::make_pair(name, lvalue_expression<int>(0)));
}

void context::put_variable(const std::string& name, const std::string& type)
{
    if (type == "integer")
    {
        put_variable<int>(name);
    }
    else
    {
        throw std::invalid_argument("Unknown type: " + type);
    }
}

template <typename Type>
lvalue_expression<Type>& context::get_variable(const std::string& name)
{
    throw std::invalid_argument("Unimplemented type of: " + name);
}

template <>
lvalue_expression<int>& context::get_variable(const std::string& name)
{
    return int_vars.at(name);
} 