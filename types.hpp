#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

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
    virtual type& operator>(type& another);
    virtual type& operator<(type& another);
    virtual type& operator~();
    virtual type& assign(type& another);
    virtual type& at(type& index);

    virtual int to_int();
    virtual bool to_bool();

    virtual operator int();

    virtual const std::string& to_string();

    virtual type& invoke(const std::list<std::shared_ptr<type>>& arg_values);
    virtual type& invoke();
    virtual void pre_invoke();
};


type& value_of(int value);
type& value_of(bool value);
type& value_of(const std::string& value);

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
    type& operator>(type& another);
    type& operator<(type& another);
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

    type& operator~();
// protected:
    bool value;
};

class mutable_bool_type : public bool_type
{
public:
    mutable_bool_type();
    type& assign(type& another);
};

class string_type : public type
{
public:
    string_type(const std::string& value);
    const std::string& to_string();
// protected:
    std::string value;
};

class mutable_string_type : public string_type
{
public:
    mutable_string_type();
    type& assign(type& another);
};

class void_type : public type
{
};

class array_type : public type
{
public:
    array_type(const std::vector<std::unique_ptr<type>>& value);
    type& at(type& index);
private:
    const std::vector<std::unique_ptr<type>>& value;
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

template <typename Return, typename ... Params>
class native_invokeable_type : public type
{
public:
    native_invokeable_type(std::function<Return(Params ...)> call);
    void pre_invoke();
    type& invoke(const std::list<std::shared_ptr<type>>& arg_values);
protected:
    std::function<Return(Params ...)> call;
};

#include "types.inl"