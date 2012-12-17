#pragma once

#include "types.hpp"

#include <string>
#include <map>
#include <memory>
#include <stack>

class context
{
public:
    virtual type& get(const std::string& name) = 0;
};

class mutable_context : public context
{
public:
    virtual type& get(const std::string& name);
    void declare(const std::string& name, type& init_value);
private:
    std::map<std::string, std::shared_ptr<type>> vars;
};

class mixed_context : public context
{
public:
    mixed_context(context& global);
    virtual type& get(const std::string& name);

    void put_local(context& next_local);
    void pop_local();
private:
    context& global;
    std::stack<std::shared_ptr<context>> local;
};