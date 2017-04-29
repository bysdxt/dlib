#pragma once
#include <type_traits>

template <class ... Ts>
struct choose_type {
    static_assert(sizeof...(Ts) > 0, "?");
private:
    template <template <class> class tester>
    struct use {
        template <class T>
        struct check : tester<T> {
            using type = T;
        };
    };
public:
    template <template <class> class tester>
    using by = typename ::std::disjunction<typename use<tester>::template check<Ts>...>::type;
};

template <size_t n>
struct type_size {
    template <class T>
    using tester = ::std::integral_constant<bool, sizeof(T) == n>;
};
