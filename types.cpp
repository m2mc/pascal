#include "types.hpp"

#include <stdexcept>

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

type& type::assign(type& another)
{
    throw std::logic_error("Cannot perform assignment");
}

type& type::invoke()
{
    throw std::logic_error("Cannot invoke this type");
}

int type::to_int()
{
    throw std::logic_error("Not integer type");
}

bool type::to_bool()
{
    throw std::logic_error("Not boolean type");
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

mutable_bool_type::mutable_bool_type() : bool_type(false)
{}

type& mutable_bool_type::assign(type& another)
{
    bool_type& casted = dynamic_cast<bool_type&>(another);
    value = casted.value;
    return *(new void_type()); 
}

invokeable_type::invokeable_type()
{}

type& invokeable_type::invoke()
{}