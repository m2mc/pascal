#pragma once

#include "types.hpp"

#include <string>
#include <map>
#include <memory>

class context
{
public:
    virtual type& get(const std::string& name) = 0;
};

class mutable_context : public context
{
public:
    virtual type& get(const std::string& name);
    virtual void declare(const std::string& name, type& init_value);
private:
    std::map<std::string, std::shared_ptr<type>> vars;
};

class mixed_context : public context
{
public:
    mixed_context(context& global, context& local);
    virtual type& get(const std::string& name);
private:
    context& global;
    context& local;
};