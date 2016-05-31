#include <cassert>
#include <functional>

int sum2(int a, int b)
{
  return a + b;
}

int main(void)
{
  using namespace std::placeholders;

  auto sum1 = std::bind(sum2, 40, _1);
  int ret = sum1(2);

  assert(ret == 42);

  return 0;
}
