#include "context.hpp"

#include <stdexcept>
#include <iostream>

std::shared_ptr<type> context::get(const std::string& name)
{
    return vars.at(name);
}

void context::declare(const std::string& name, std::shared_ptr<type> init_value)
{
    vars.insert(std::make_pair(name, init_value));
}

void context::redeclare(const std::string& name, std::shared_ptr<type> init_value)
{
    vars.at(name) = init_value;
}

void context::trace()
{
    for (const auto& var : vars)
        std::cout << var.first << " = " << *var.second << std::endl;
}

void native_write(int value)
{
    std::cout << value << std::endl;
}

context_manager::context_manager()
{
    global.declare("write", std::shared_ptr<type>(new native_invokeable_type<void, int>(native_write)));
}

std::shared_ptr<type> context_manager::get(const std::string& name)
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

void context_manager::trace()
{
    std::stack<context> copy(local);
    while (!copy.empty())
    {
        copy.top().trace();
        copy.pop();
        if (!copy.empty())
            std::cout << "------" << std::endl;
    }
    std::cout << "======" << std::endl;
}