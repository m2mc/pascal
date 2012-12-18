#include "context.hpp"

#include <stdexcept>

type& context::get(const std::string& name)
{
    return *vars.at(name);
}

void context::declare(const std::string& name, type& init_value)
{
    vars.insert(std::make_pair(name, std::unique_ptr<type>(&init_value)));
}

type& context_manager::get(const std::string& name)
{
    try
    {
        return local.top().get(name);
    }
    catch (std::exception e)
    {
        return global.get(name);
    }
}

context& context_manager::get_global()
{
    return global;
}

context& context_manager::get_local()
{
    return local.top();
}

void context_manager::put_local()
{
    local.push(context());
}

void context_manager::pop_local()
{
    local.pop();
}