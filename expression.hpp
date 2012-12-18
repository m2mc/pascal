#pragma once

#include "types.hpp"
#include "context.hpp"

#include <string>
#include <list>
#include <memory>
#include <iostream>
#include <stdexcept>

class expression
{
public:
    virtual ~expression();
    virtual type& eval() = 0;
};

class const_expression : public expression
{
public:
    const_expression(type& value);
    type& eval();
private:
    type& value;
};

class dynamic_expression : public expression
{
public:
    dynamic_expression(const std::string& id, context_manager& ctxt);
    type& eval();
protected:
    std::string id;
    context_manager& ctxt;
};

class binary_expression : public expression
{
public:
    binary_expression(expression& left,
                      char op,
                      expression& right);
    type& eval();
private:
    expression& left;
    char op;
    expression& right;
};

class expression_list : public expression
{
public:
    void push_back(expression& next);
    void push_all(expression_list& next_list);
    const std::list<std::unique_ptr<expression>>& get_list();
    type& eval();
private:
    std::list<std::unique_ptr<expression>> list;
};

class if_expression : public expression
{
public:
    if_expression(expression& condition, expression& body);
    type& eval();
private:
    expression& condition;
    expression& body;
};

class var_declare_expression : public expression
{
public:
    var_declare_expression(const std::string& name,
                           const std::string& type_name,
                           context_manager& ctxt);
    type& eval();
    const std::string& get_name();
private:
    std::string name;
    std::string type_name;
    context_manager& ctxt;
};

class function_invoke_expression : public expression
{
public:
    function_invoke_expression(const std::string& id,
                               expression_list& args,
                               context_manager& ctxt);
    type& eval();
private:
    std::string id;
    expression_list& args;
    context_manager& ctxt;
};