#include "expression.hpp"

#include <stdexcept>

template <typename Type>
expression<Type>::~expression()
{}

template <typename Type>
const_expression<Type>::const_expression(Type value) : value(value)
{}

template <typename Type>
Type const_expression<Type>::eval()
{
    return value;
}

template <typename Type>
lvalue_expression<Type>::lvalue_expression(Type value) : value(value)
{}

template <typename Type>
Type lvalue_expression<Type>::eval()
{
    return value;
}

template <typename Type>
void lvalue_expression<Type>::set(Type new_value)
{
    value = new_value;
}

template <typename Type>
binary_expression<Type>::binary_expression(expression<Type>& left, char op, expression<Type>& right) :
    left(left), op(op), right(right)
{}

template <typename Type>
Type binary_expression<Type>::eval()
{
    switch (op)
    {
        case '+':
            return left.eval() + right.eval();
        case '*':
            return left.eval() * right.eval();
        default:
            throw std::invalid_argument("Unimplemented operator: " + op);
    }
}

template <typename Type>
assign_expression<Type>::assign_expression(lvalue_expression<Type>& left, expression<Type>& right) :
    left(left), right(right)
{}

template <typename Type>
void assign_expression<Type>::eval()
{
    left.set(right.eval());
}

template <typename Type>
if_expression<Type>::if_expression(expression<bool>& condition, expression<Type>& body) :
    condition(condition), body(body)
{}

template <typename Type>
void if_expression<Type>::eval()
{
    if (condition.eval())
    {
        body.eval();
    }
}