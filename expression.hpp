#pragma once

#include <string>
#include <list>
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

template <typename Type>
class lvalue_expression : public expression<Type>
{
public:
    lvalue_expression(Type value);
    Type eval();
    void set(Type new_value);
private:
    Type value;
};

template <typename Type>
class binary_expression : public expression<Type>
{
public:
    binary_expression(expression<Type>& left, char op, expression<Type>& right);
    Type eval();
private:
    expression<Type>& left;
    char op;
    expression<Type>& right;
};

template <typename Type>
class assign_expression : public expression<void>
{
public:
    assign_expression(lvalue_expression<Type>& left, expression<Type>& right);
    void eval();
private:
    lvalue_expression<Type>& left;
    expression<Type>& right;
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