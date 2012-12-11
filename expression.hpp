#pragma once

#include "types.hpp"
#include "context.hpp"

#include <string>
#include <list>
#include <memory>
#include <iostream>

class expression
{
public:
    virtual ~expression();
    virtual type&& eval() = 0;
};

class const_expression : public expression
{
public:
    const_expression(type& value);
    type&& eval();
private:
    type& value;
};

class dynamic_expression : public expression
{
public:
    dynamic_expression(const std::string& id, context& ctxt);
    type&& eval();
private:
    std::string id;
    context& ctxt;
};

class binary_expression : public expression
{
public:
    binary_expression(expression& left,
                      char op,
                      expression& right);
    type&& eval();
private:
    expression& left;
    char op;
    expression& right;
};

class expression_list : public expression
{
public:
    void push_back(expression& next);
    type&& eval();
private:
    std::list<std::unique_ptr<expression>> list;
};

class if_expression : public expression
{
public:
    if_expression(expression& condition, expression& body);
    type&& eval();
private:
    expression& condition;
    expression& body;
};