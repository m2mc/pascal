#pragma once

#include "types.hpp"

#include <string>
#include <map>
#include <memory>
#include <stack>

class context
{
public:
    virtual type& get(const std::string& name);
    void declare(const std::string& name, type& init_value);
private:
    std::map<std::string, std::shared_ptr<type>> vars;
};

class context_manager
{
public:
    virtual type& get(const std::string& name);

    context& get_global();
    context& get_local();
    void put_local();
    void pop_local();
private:
    context global;
    std::stack<context> local;
};