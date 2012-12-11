#pragma once

#include "types.hpp"

#include <string>
#include <map>

class context
{
public:
    void put(const std::string& name, const std::string& type_name);
    type&& get(const std::string& name);
private:
    std::map<std::string, type&&> vars;
};