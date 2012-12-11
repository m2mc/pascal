#include "expression.hpp"

#include <stdexcept>

expression::~expression()
{}

const_expression::const_expression(type& value) : value(value)
{}

type&& const_expression::eval()
{
    return std::move(value);
}

dynamic_expression::dynamic_expression(const std::string& id, context& ctxt) :
id(id), ctxt(ctxt)
{}

type&& dynamic_expression::eval()
{
    return std::move(*(new void_type()));
    // fill
}

binary_expression::binary_expression(expression& left,
                                     char op,
                                     expression& right) :
    left(left), op(op), right(right)
{}

type&& binary_expression::eval()
{
    switch (op)
    {
        case '+':
            return std::move(left.eval() + right.eval());
        case '-':
            return std::move(left.eval() - right.eval());
        case '*':
            return std::move(left.eval() * right.eval());
        case '/':
            return std::move(left.eval() / right.eval());
        case '=':
            return std::move(left.eval() == right.eval());
        case 'a':
            return left.eval().assign(right.eval());
        default:
            throw std::invalid_argument("Unimplemented operator: " + std::string(&op, 1));
    }
}

void expression_list::push_back(expression& next)
{
    list.push_back(std::unique_ptr<expression>(&next));
}

type&& expression_list::eval()
{
    // need fix for double evaluation
    for (auto& next : list)
        next->eval();
    if (!list.empty())
        return list.back()->eval();
    else
        return std::move(*(new void_type()));
}

if_expression::if_expression(expression& condition, expression& body) :
    condition(condition), body(body)
{}

type&& if_expression::eval()
{
    if (condition.eval().to_bool())
        return body.eval();
    else
        return std::move(*(new void_type()));
}