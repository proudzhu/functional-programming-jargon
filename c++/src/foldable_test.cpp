#include <vector>
#include <cassert>

int main(void)
{
  auto reduce = [](auto f, auto xs, auto x) {
    auto result = x;
    for (auto i : xs) {
        result = f(result, i);
    }
    return result;
  };

  std::vector<int> xs{1, 2, 3, 4, 5};
  auto ret = reduce([](auto x, auto y) { return x + y; }, xs, 0);

  assert(ret == 15);

  return 0;
}
