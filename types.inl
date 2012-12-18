template <typename Return, typename ... Params>
native_invokeable_type<Return, Params ...>::native_invokeable_type(std::function<Return(Params ...)> call) :
    call(call)
{}

template <typename Return, typename ... Params>
void native_invokeable_type<Return, Params ...>::pre_invoke()
{}

template <typename Return, typename ... Params>
type& native_invokeable_type<Return, Params ...>::invoke(const std::list<std::shared_ptr<type>>& arg_values)
{
    return value_of(call(*arg_values.front()));
}