#include "context.hpp"

#include <stdexcept>

void context::put(const std::string& name, const std::string& type_name)
{
    if (type_name == "integer")
        vars.insert(std::make_pair(name, std::unique_ptr<type>(new mutable_int_type())));
    else if (type_name == "boolean")
        vars.insert(std::make_pair(name, std::unique_ptr<type>(new mutable_bool_type())));
    else
        throw std::logic_error("Invalid type name: " + type_name);
}

type& context::get(const std::string& name)
{
    return *vars.at(name);
}
