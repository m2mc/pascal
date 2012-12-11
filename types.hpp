#pragma once

class type
{
public:
    virtual type&& operator+(type&& another);
    virtual type&& operator-(type&& another);
    virtual type&& operator*(type&& another);
    virtual type&& operator/(type&& another);
    virtual type&& operator==(type&& another);
    virtual type&& assign(type&& another);

    virtual int to_int();
    virtual bool to_bool();
};

class int_type : public type
{
public:
    int_type(int value);
    int to_int();
    type&& operator+(type&& another);
    type&& operator-(type&& another);
    type&& operator*(type&& another);
    type&& operator/(type&& another);
    type&& operator==(type&& another);
// protected:
    int value;
};

class mutable_int_type : public int_type
{
public:
    mutable_int_type();
    type&& assign(type&& another);
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
    type&& assign(type&& another);
};

class void_type : public type
{
};