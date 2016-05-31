#include <type_traits>
#include <string>
#include "arity.hpp"

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

