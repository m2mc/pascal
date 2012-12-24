#include "stdpas.hpp"

#include <iostream>

namespace stdpas
{

void declare(const std::string& name, native_func call, context_manager& ctxt)
{
    ctxt.get_global().declare(name, std::shared_ptr<type>(new native_invokeable_type(call)));
}

std::shared_ptr<type> write(native_sign values)
{
    for (const auto& value : values)
    {
        std::cout << value->to_string() << " ";
    }
    std::cout << std::endl;
    return std::shared_ptr<type>(new void_type());
}

std::shared_ptr<type> read(native_sign values)
{
    for (const auto& value : values)
    {
        std::string input;
        std::cin >> input;
        value->from_string(input);
    }
    return std::shared_ptr<type>(new void_type());
}

}