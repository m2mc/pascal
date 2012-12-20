// template <typename Return, typename ... Params>
// native_invokeable_type<Return, Params ...>::native_invokeable_type(std::function<Return(Params ...)> call) :
//     call(call)
// {}

// template <typename Return, typename ... Params>
// void native_invokeable_type<Return, Params ...>::pre_invoke()
// {}

// template <typename Return, typename ... Params>
// std::shared_ptr<type> native_invokeable_type<Return, Params ...>::invoke(const std::list<std::shared_ptr<type>>& arg_values)
// {
//     if (sizeof...(Params) != arg_values.size())
//         throw std::logic_error("Incorrect number of arguments");
//     return invoke_function(call, arg_values);
// }


// template <typename Return, typename ... Params>
// std::shared_ptr<type> invoke_function(std::function<Return(Params ...)> call, const std::list<std::shared_ptr<type>>& arg_values)
// {
//     throw std::logic_error("Wrong template chosen for native funlction");
// }

// template <typename Return>
// std::shared_ptr<type> invoke_function(std::function<Return()> call, const std::list<std::shared_ptr<type>>& arg_values)
// {
//     return value_of(call());
// }

// template <>
// std::shared_ptr<type> invoke_function(std::function<void()> call, const std::list<std::shared_ptr<type>>& arg_values);

// template <typename Return, typename Param1>
// std::shared_ptr<type> invoke_function(std::function<Return(Param1)> call, const std::list<std::shared_ptr<type>>& arg_values)
// {
//     return value_of(call(*arg_values.front()));
// }

// #include <iostream>
// template <typename Param1>
// std::shared_ptr<type> invoke_function(std::function<void(Param1)> call, const std::list<std::shared_ptr<type>>& arg_values)
// {
//     std::cout << "print ptr: " << arg_values.front() << std::endl;
//     call(*arg_values.front());
//     return std::shared_ptr<type>(new void_type());
// }

// template <typename Return, typename Param1, typename Param2>
// std::shared_ptr<type> invoke_function(std::function<Return(Param1, Param2)> call, const std::list<std::shared_ptr<type>>& arg_values)
// {
//     return value_of(call(*arg_values.front(), *arg_values.back()));
// }

// template <typename Param1, typename Param2>
// std::shared_ptr<type> invoke_function(std::function<void(Param1, Param2)> call, const std::list<std::shared_ptr<type>>& arg_values)
// {
//     call(*arg_values.front(), *arg_values.back());
//     return std::shared_ptr<type>(new void_type());
// }