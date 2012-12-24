#pragma once

#include <memory>
#include "types.hpp"
#include "context.hpp"

namespace stdpas
{

void declare(const std::string& name, native_func call, context_manager& ctxt);

std::shared_ptr<type> write(native_sign values);
std::shared_ptr<type> read(native_sign values);

}