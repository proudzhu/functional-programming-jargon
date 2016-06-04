/* this file is copy from #archlinux-cn
 * the original author is conky
 */
#include <iostream>
#include <functional>
#include <algorithm>
#include <type_traits>
#include <cassert>

/*
 * auto f = curry(foo);   void foo(int, int, int) {}
 * f(1)(2)(3);
 * f(1,2)(3);
 * f(1,2,3);
 * f(1) return a lambda taking 2 parameters.
 * f(1,2) return a lambda taking 1 parameter.
 * f(1,2,3) return a value.
 */

namespace lambda {
namespace detail {
template <typename R, typename TupleType, size_t... I>
auto helper(std::index_sequence<I...>) {
    using t = std::function<R(std::tuple_element_t<I, TupleType>...)>;
    return t();
}

template <size_t N>
struct drop {
    template <typename First, typename... Args>
    struct elems {
        using type = typename drop<N - 1>::template elems<Args...>::type;
    };
};

template <>
struct drop<0> {
    template <typename... Args>
    struct elems {
        using type = std::tuple<Args...>;
    };
};

template <typename>
class CurryObject;

template <typename R, typename... Args>
class CurryObject<R(Args...)> : public CurryObject<std::function<R(Args...)>> {
public:
    CurryObject(R f(Args...))
        : CurryObject<std::function<R(Args...)>>(std::move(f)) {}
};

template <typename R, typename... Args>
class CurryObject<R (*)(Args...)> : public CurryObject<R(Args...)> {
public:
    CurryObject(R (*pf)(Args...)) : CurryObject<R(Args...)>(pf) {}
};

template <typename R, typename... Args>
class CurryObject<std::function<R(Args...)>> {
public:
    CurryObject(std::function<R(Args...)> f) : fn(std::move(f)) {}

    // full apply
    template <typename... Params,
              std::enable_if_t<sizeof...(Params) == sizeof...(Args), int> = 0>
    auto operator()(Params&&... parms) {
        return fn(std::forward<Params>(parms)...);
    }

    // partial apply
    template <typename... Params,
              std::enable_if_t<sizeof...(Params) < sizeof...(Args), int> = 0>
    auto operator()(Params&&... parms) {
        using tuple_type =
            typename drop<sizeof...(Params)>::template elems<Args...>::type;
        using func_type = decltype(helper<R, tuple_type>(
            std::make_index_sequence<sizeof...(Args) - sizeof...(Params)>{}));

        func_type f = [&](auto... xs) {
            return fn(std::forward<Params>(parms)..., xs...);
        };
        return CurryObject<func_type>(f);
    }

private:
    std::function<R(Args...)> fn;
};

template <typename T>
struct memfun_type {
    using type = void;
};

template <typename R, typename C, typename... Args>
struct memfun_type<R (C::*)(Args...) const> {
    using type = std::function<R(Args...)>;
};
}

template <typename F, std::enable_if_t<!std::is_class<F>::value, int> = 0>
auto curry(F f) {
    return detail::CurryObject<F>(f);
}

template <typename F, std::enable_if_t<std::is_class<F>::value, int> = 0>
auto curry(F f) {
    using t = typename detail::memfun_type<decltype(&F::operator())>::type;
    return detail::CurryObject<t>(f);
}
}

int foo(int a, int b, int c) {
    return a + b + c;
}

struct bar {
    void operator()() {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }
};

int main() {
    using namespace lambda;

    auto f = curry(foo);
    auto r1 = f(1, 2, 4);
    auto r2 = f(1, 4);
    auto r3 = f(1)(8);

    auto ftor = [](int x, int y, int z) { return x * y + z; };
    auto cftor = curry(ftor);

    assert(r1 == f(1, 2, 4));
    assert(r2(42) == f(1, 4, 42));
    assert(r3(42) == f(1, 8, 42));
    assert(cftor(1, 2, 3) == cftor(1)(2)(3));

    /*
    std::cout << r1 << std::endl      // 7
              << r2(10) << std::endl  // 15
              << r3(64) << std::endl  // 73
              << cftor(1)(2)(3) << std::endl // 5
              << cftor(1, 3)(4) << std::endl // 7
              << cftor(2, 5, 7) << std::endl // 17
              ;
    */
}
