#include <vector>
#include <cassert>

int main(void)
{
  auto filter = [](auto pred, auto xs) {
    decltype(xs) result;
    for (auto x : xs) {
      if (pred(x)) {
        result.push_back(x);
      }
    }
    return result;
  };

  std::vector<int> xs{1, 2, 3, 4, 5};
  std::vector<int> ans{1, 3, 5};
  std::vector<int> ret = filter([](auto x){ return x % 2; }, xs);

  assert(ret == ans);

  return 0;
}
