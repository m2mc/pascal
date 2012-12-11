#include "expression.hpp"

void expression_list::eval()
{
    for (auto& next : list)
    {
        next->eval();
    }
}