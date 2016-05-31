#include <type_traits>
#include <tuple>

template<class F>
struct function_traits;

// function pointer
template<class R, class... Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)>
{};

template<class R, class... Args>
struct function_traits<R(Args...)>
{
  using return_type = R;

  static constexpr std::size_t arity = sizeof...(Args);

  template <std::size_t N>
  struct argument
  {
    static_assert(N < arity, "error: invalid parameter index.");
    using type = typename std::tuple_element<N,std::tuple<Args...>>::type;
  };
};

float free_function(const std::string& a, int b)
{
  return (float)a.size() / b;
}

int main()
{
  using Traits = function_traits<decltype(free_function)>;

  static_assert(Traits::arity == 2,"");
  static_assert(std::is_same<Traits::return_type,float>::value,"");
  static_assert(std::is_same<Traits::argument<0>::type,const std::string&>::value,"");
  static_assert(std::is_same<Traits::argument<1>::type,int>::value,"");

  return 0;
}

