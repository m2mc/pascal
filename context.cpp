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

mixed_context::mixed_context(context& global, context& local) :
    global(global), local(local)
{}

type& mixed_context::get(const std::string& name)
{
    try
    {
        return local.get(name);
    }
    catch (std::exception e)
    {
        return global.get(name);
    }
}