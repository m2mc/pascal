#pragma once

#include <string>
#include <list>
#include <memory>
#include <iostream>

template <typename Type>
class expression
{
public:
    virtual ~expression();
    virtual Type eval() = 0;
};

template <typename Type>
class const_expression : public expression<Type>
{
public:
    const_expression(Type value);
    Type eval();
private:
    Type value;
};

class lvalue_expression
{
public:
    lvalue_expression(std::string name);

    template <typename Type>
    void set(Type new_value);

    int eval_int();
    bool eval_bool();

    void set(lvalue_expression& right);
private:

};

template <typename Type>
class static_expression : public expression<Type>
{
public:
    static_expression(lvalue_expression& dynamic);
    Type eval();
};

template <typename Type, typename Argtype>
class binary_expression : public expression<Type>
{
public:
    binary_expression(expression<Argtype>& left, char op, expression<Argtype>& right);
    Type eval();
private:
    expression<Argtype>& left;
    char op;
    expression<Argtype>& right;
};

template <typename Type>
class assign_expression : public expression<void>
{
public:
    assign_expression(lvalue_expression& left, expression<Type>& right);
    void eval();
private:
    lvalue_expression& left;
    expression<Type>& right;
};

template <typename Type>
class void_wrap : public expression<void>
{
public:
    void_wrap(expression<Type>& wrapped);
    void eval();
private:
    expression<Type>& wrapped;
};

class expression_list : public expression<void>
{
public:
    template <typename Type>
    void push_back(expression<Type>& next);
    void eval();
private:
    std::list<std::unique_ptr<expression<void>>> list;
};

template <typename Type>
class if_expression : public expression<void>
{
public:
    if_expression(expression<bool>& condition, expression<Type>& body);
    void eval();
private:
    expression<bool>& condition;
    expression<Type>& body;
};

#include "expression.inl"