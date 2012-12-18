#include "types.hpp"

#include "expression.hpp"
#include "context.hpp"
#include <stdexcept>

type& value_of(int value)
{
    return *(new int_type(value));
}

type& value_of(bool value)
{
    return *(new bool_type(value));
}

type& value_of(const std::string& value)
{
    return *(new string_type(value));
}

type& type::operator+(type& another)
{
    throw std::logic_error("Operator + is not applicable");
}

type& type::operator-(type& another)
{
    throw std::logic_error("Operator - is not applicable");
}

type& type::operator*(type& another)
{
    throw std::logic_error("Operator * is not applicable");
}

type& type::operator/(type& another)
{
    throw std::logic_error("Operator / is not applicable");
}

type& type::operator==(type& another)
{
    throw std::logic_error("Operator = is not applicable");
}

type& type::operator>(type& another)
{
    throw std::logic_error("Operator > is not applicable");
}

type& type::operator<(type& another)
{
    throw std::logic_error("Operator < is not applicable");
}

type& type::operator~()
{
    throw std::logic_error("Operator ~ is not applicable");
}

type& type::assign(type& another)
{
    throw std::logic_error("Cannot perform assignment");
}

type& type::at(type& index)
{
    throw std::logic_error("Cannot access as an array");
}

int type::to_int()
{
    throw std::logic_error("Not integer type");
}

bool type::to_bool()
{
    throw std::logic_error("Not boolean type");
}

type::operator int()
{
    return to_int();
}

const std::string& type::to_string()
{
    throw std::logic_error("Not string type");
}

type& type::invoke(const std::list<std::shared_ptr<type>>& arg_values)
{
    throw std::logic_error("Cannot invoke this type");
}

type& type::invoke()
{
    return invoke(std::list<std::shared_ptr<type>>());
}

void type::pre_invoke()
{
    throw std::logic_error("Cannot invoke this type");
}

int_type::int_type(int value) : value(value)
{}

int int_type::to_int()
{
    return value;
}

type& int_type::operator+(type& another)
{
    int_type& casted = dynamic_cast<int_type&>(another);
    return *(new int_type(value + casted.value));
}

type& int_type::operator-(type& another)
{
    int_type& casted = dynamic_cast<int_type&>(another);
    return *(new int_type(value - casted.value));
}

type& int_type::operator*(type& another)
{
    int_type& casted = dynamic_cast<int_type&>(another);
    return *(new int_type(value * casted.value));
}

type& int_type::operator/(type& another)
{
    int_type&& casted = dynamic_cast<int_type&&>(another);
    return *(new int_type(value / casted.value));
}

type& int_type::operator==(type& another)
{
    int_type& casted = dynamic_cast<int_type&>(another);
    return *(new bool_type(value == casted.value));
}

type& int_type::operator>(type& another)
{
    int_type& casted = dynamic_cast<int_type&>(another);
    return *(new bool_type(value > casted.value));
}

type& int_type::operator<(type& another)
{
    int_type& casted = dynamic_cast<int_type&>(another);
    return *(new bool_type(value < casted.value));
}

mutable_int_type::mutable_int_type() : int_type(0)
{}

type& mutable_int_type::assign(type& another)
{
    int_type& casted = dynamic_cast<int_type&>(another);
    value = casted.value;
    return *(new void_type());
}

bool_type::bool_type(bool value) : value(value)
{}

bool bool_type::to_bool()
{
    return value;
}

type& bool_type::operator~()
{
    return *(new bool_type(!value));
}

mutable_bool_type::mutable_bool_type() : bool_type(false)
{}

type& mutable_bool_type::assign(type& another)
{
    bool_type& casted = dynamic_cast<bool_type&>(another);
    value = casted.value;
    return *(new void_type()); 
}

string_type::string_type(const std::string& value) :
    value(value)
{}

const std::string& string_type::to_string()
{
    return value;
}

mutable_string_type::mutable_string_type() : string_type("")
{}

type& mutable_string_type::assign(type& another)
{
    string_type& casted = dynamic_cast<string_type&>(another);
    value = casted.value;
    return *(new void_type());
}

array_type::array_type(const std::vector<std::unique_ptr<type>>& value) :
    value(value)
{}

type& array_type::at(type& index)
{
    return *value.at(index.to_int());
}

invokeable_type::invokeable_type(expression_list& arguments, expression& body, context_manager& ctxt) :
    arguments(arguments), body(body), ctxt(ctxt)
{
    // do something nicer
    for (const auto& expr : arguments.get_list())
    {
        expression_list* list
            = dynamic_cast<expression_list*>(expr.get());
        for (const auto& decl : list->get_list())
        {
            var_declare_expression* casted
                = dynamic_cast<var_declare_expression*>(decl.get());
            signature.push_back(casted->get_name());
        }
    }
}

type& invokeable_type::invoke(const std::list<std::shared_ptr<type>>& arg_values)
{
    if (arg_values.size() != signature.size())
        throw std::logic_error("Incorrect number of arguments");
    auto iter = arg_values.begin();
    for (const auto& arg_name : signature)
    {
        ctxt.get(arg_name).assign(**iter);
        ++iter;
    }
    return body.eval();
}

void invokeable_type::pre_invoke()
{
    arguments.eval();
}