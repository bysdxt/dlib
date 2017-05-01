#pragma once
#include <type_traits>
namespace dlib {
    template <class ... Ts> struct choose_type;
    namespace __impl_choose_type {
        template <class T>
        struct error_not_find {
            static_assert(sizeof(T) < 0, "not find");
        };
        template <class test, class T, class N, template <class> class tester> struct choose;
        template <class T, class N, template <class> class tester>
        struct choose<::std::true_type, T, N, tester> {
            using Result = T;
        };
        template <class T, class N, template <class> class tester>
        struct choose<::std::false_type, T, N, tester> {
            using Result = typename N::template by<tester>;
        };
        template <class T, template <class> class tester>
        struct choose<::std::false_type, T, choose_type<>, tester> {
            static_assert(sizeof(T) < 0, "not find");
        };
    }
    template <class T, class ... Ts>
    struct choose_type<T, Ts...> {
    public:
        template <template <class> class tester>
        using by = typename __impl_choose_type::template choose<tester<T>, T, choose_type<Ts...>, tester>::Result;
    };
    template <> struct choose_type<> { };
}