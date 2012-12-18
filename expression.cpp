#include "expression.hpp"

expression::~expression()
{}

const_expression::const_expression(type& value) : value(value)
{}

type& const_expression::eval()
{
    return value;
}

dynamic_expression::dynamic_expression(const std::string& id, context_manager& ctxt) :
id(id), ctxt(ctxt)
{}

type& dynamic_expression::eval()
{
    return ctxt.get(id);
}

binary_expression::binary_expression(expression& left,
                                     char op,
                                     expression& right) :
    left(left), op(op), right(right)
{}

type& binary_expression::eval()
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
            return left.eval() / right.eval();
        case '=':
            return left.eval() == right.eval();
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

void expression_list::push_all(expression_list& next_list)
{
    //use splice!
    while (!next_list.list.empty())
    {
        std::unique_ptr<expression> next_expr
            = std::move(next_list.list.front());
        list.push_back(std::move(next_expr));
        next_list.list.pop_front();
    }
}

type& expression_list::eval()
{
    // need fix for double evaluation
    type* ret = nullptr;
    for (auto& next : list)
        ret = &(next->eval());
    if (ret != nullptr)
        return *ret;
    else
        return *(new void_type());
}

const std::list<std::unique_ptr<expression>>& expression_list::get_list()
{
    return list;
}

if_expression::if_expression(expression& condition, expression& body) :
    condition(condition), body(body)
{}

type& if_expression::eval()
{
    if (condition.eval().to_bool())
        return body.eval();
    else
        return *(new void_type());
}

var_declare_expression::var_declare_expression(const std::string& name,
                                               const std::string& type_name,
                                               context_manager& ctxt) :
    name(name), type_name(type_name), ctxt(ctxt)
{}

type& var_declare_expression::eval()
{
    if (type_name == "integer")
        ctxt.get_local().declare(name, *(new mutable_int_type()));
    else if (type_name == "boolean")
        ctxt.get_local().declare(name, *(new mutable_bool_type()));
    else
        throw std::logic_error("Invalid type name: " + type_name);
    return *(new void_type());
}

const std::string& var_declare_expression::get_name()
{
    return name;    
}

function_invoke_expression::function_invoke_expression(const std::string& id,
                                                       expression_list& args,
                                                       context_manager& ctxt) :
    id(id), args(args), ctxt(ctxt)
{}

type& function_invoke_expression::eval()
{
    std::list<std::shared_ptr<type>> arg_values;
    for (const auto& arg : args.get_list())
        arg_values.push_back(std::shared_ptr<type>(&(arg->eval())));
    type& func = ctxt.get(id);
    ctxt.put_local();
    func.pre_invoke();
    type& result = func.invoke(arg_values);
    ctxt.pop_local();
    return result;
}