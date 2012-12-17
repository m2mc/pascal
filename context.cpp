#include "context.hpp"

#include <stdexcept>

type& mutable_context::get(const std::string& name)
{
    return *vars.at(name);
}

void mutable_context::declare(const std::string& name, type& init_value)
{
    vars.insert(std::make_pair(name, std::unique_ptr<type>(&init_value)));
}

mixed_context::mixed_context(context& global) :
    global(global), local(local)
{}

type& mixed_context::get(const std::string& name)
{
    try
    {
        return local.top()->get(name);
    }
    catch (std::exception e)
    {
        return global.get(name);
    }
}

void mixed_context::put_local(context& next_local)
{
    local.push(std::unique_ptr<context>(&next_local));
}

void mixed_context::pop_local()
{
    local.pop();
}