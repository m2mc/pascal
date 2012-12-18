#pragma once

#include <list>
#include <memory>

class expression;
class expression_list;
class context_manager;

class type
{
public:
    virtual type& operator+(type& another);
    virtual type& operator-(type& another);
    virtual type& operator*(type& another);
    virtual type& operator/(type& another);
    virtual type& operator==(type& another);
    virtual type& assign(type& another);

    virtual int to_int();
    virtual bool to_bool();

    virtual type& invoke(const std::list<std::shared_ptr<type>>& arg_values);
    virtual type& invoke();
    virtual void pre_invoke();
};

class int_type : public type
{
public:
    int_type(int value);
    int to_int();
    type& operator+(type& another);
    type& operator-(type& another);
    type& operator*(type& another);
    type& operator/(type& another);
    type& operator==(type& another);
// protected:
    int value;
};

class mutable_int_type : public int_type
{
public:
    mutable_int_type();
    type& assign(type& another);
};

class bool_type : public type
{
public:
    bool_type(bool value);
    bool to_bool();
// protected:
    bool value;
};

class mutable_bool_type : public bool_type
{
public:
    mutable_bool_type();
    type& assign(type& another);
};

class void_type : public type
{
};

class invokeable_type : public type
{
public:
    invokeable_type(expression_list& arguments, expression& body, context_manager& ctxt);
    type& invoke(const std::list<std::shared_ptr<type>>& arg_values);
    void pre_invoke();
private:
    expression& arguments;
    expression& body;
    std::list<std::string> signature;
    context_manager& ctxt;
};