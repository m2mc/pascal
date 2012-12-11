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

template <typename Type, typename Argtype>
binary_expression<Type, Argtype>::binary_expression(expression<Argtype>& left, char op, expression<Argtype>& right) :
    left(left), op(op), right(right)
{}

template <typename Type, typename Argtype>
Type binary_expression<Type, Argtype>::eval()
{
    switch (op)
    {
        case '+':
            return left.eval() + right.eval();
        case '-':
            return left.eval() - right.eval();
        case '*':
            return left.eval() * right.eval();
        case '/':
        {
            Type lvalue = left.eval();
            Type rvalue = right.eval();
            if (rvalue == 0)
            {
                throw std::domain_error("Division by zero");
            }
            return lvalue / rvalue;
        }
        default:
            throw std::invalid_argument("Unimplemented operator: " + std::string(&op, 1));
    }
}

template <typename Type>
assign_expression<Type>::assign_expression(lvalue_expression& left, expression<Type>& right) :
    left(left), right(right)
{}

template <typename Type>
void assign_expression<Type>::eval()
{
    left.set<Type>(right.eval());
}

template <typename Type>
void_wrap<Type>::void_wrap(expression<Type>& wrapped) : wrapped(wrapped)
{}

template <typename Type>
void void_wrap<Type>::eval()
{
    wrapped.eval();
}

template <typename Type>
void expression_list::push_back(expression<Type>& next)
{
    list.push_back(std::unique_ptr<expression<void>>(new void_wrap<Type>(next)));
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