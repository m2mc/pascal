#include "stdpas.hpp"

#include <iostream>

namespace stdpas
{

// template <typename Base, typename Derived>
// bool instance_of(std::shared_ptr<Base> object)
// {
//     try
//     {
//         std::shared_ptr<Derived> casted = std::dynamic_pointer_cast<Derived>(object);
//         return true;
//     }
//     catch (std::exception e)
//     {
//         return false;
//     }
// }

void declare(const std::string& name, native_func call, context_manager& ctxt)
{
    ctxt.get_global().declare(name, std::shared_ptr<type>(new native_invokeable_type(call)));
}

std::shared_ptr<type> write(native_sign values)
{
    std::cout << values.front()->to_string() << std::endl;
    return std::shared_ptr<type>(new void_type());
}

std::shared_ptr<type> read(native_sign values)
{
    std::shared_ptr<type> value = values.front();
    std::string input;
    std::cin >> input;
    value->from_string(input);
    return std::shared_ptr<type>(new void_type());
}

}