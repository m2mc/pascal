#include "context.hpp"

#include <stdexcept>

void context::put(const std::string& name, const std::string& type_name)
{
    if (name == "integer")
        vars.insert(std::make_pair(name, *(new mutable_int_type())));
    else if (name == "boolean")
        vars.insert(std::make_pair(name, *(new mutable_bool_type())));
    else
        throw new std::logic_error("Invalid type name");
}

type&& context::get(const std::string& name)
{
    return std::move(vars.at(name));
}


