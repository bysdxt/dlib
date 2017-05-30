#pragma once
#include <type_traits>
#include <limits>
namespace dlib {
    template <class ...> struct Types;
    namespace __impl_Types_Operations {
        template <class, class, class> struct filter;
        template <class, class, class, class, class> struct _filter;
        template <class F, class T, class Ts, class ... ts>
        struct _filter<::std::true_type, Types<ts...>, F, T, Ts> {
            using result = typename filter<Types<ts..., T>, F, Ts>::result;
        };
        template <class F, class T, class Ts, class ts>
        struct _filter<::std::false_type, ts, F, T, Ts> {
            using result = typename filter<ts, F, Ts>::result;
        };
        template <class F, class t>
        struct filter<t, F, Types<>> {
            using result = t;
        };
        template <class F, class T, class ... Ts, class ... ts>
        struct filter<Types<ts...>, F, Types<T, Ts...>> {
            using result = typename _filter<typename F::template execute<T>, Types<ts...>, F, T, Types<Ts...>>::result;
        };
        template <class, class, class> struct map;
        template <class t, class F>
        struct map<t, F, Types<>> {
            using result = t;
        };
        template <class ... ts, class F, class T, class ... Ts>
        struct map<Types<ts...>, F, Types<T, Ts...>> {
            using result = typename map<Types<ts..., typename F::template execute<T>>, F, Types<Ts...>>::result;
        };
    }
    template <> struct Types<> {
        static constexpr size_t count = 0;
        template <class t> using append = Types<t>;
        template <class> using filter = Types<>;
        template <class> using map = Types<>;
    };
    template <class T, class ... Ts>
    struct Types<T, Ts...> {
        static constexpr size_t count = 1 + sizeof...(Ts);
        using first = T;
        using tail = Types<Ts...>;
        template <class t> using append = Types<T, Ts..., t>;
        template <class F>
        using filter = typename __impl_Types_Operations::filter<Types<>, F, Types<T, Ts...>>::result;
        template <class F>
        using map = typename __impl_Types_Operations::map<Types<>, F, Types<T, Ts...>>::result;
    };
    namespace filters {
        template <size_t size>
        struct type_size_is {
            template <class T>
            using execute = ::std::bool_constant<sizeof(T) == size>;
        };
        template <unsigned long long value>
        struct type_max_value_is_enough {
            template <class T>
            using execute = ::std::bool_constant<::std::numeric_limits<T>::max() >= value>;
        };
    }
    namespace maps {
        template <class xy>
        struct transform {
            template <class T>
            struct f {
                template <class t> struct _execute {
                    using result = ::std::false_type;
                };
                template <class ... ts>
                struct _execute<Types<T, ts...>> {
                    using result = ::std::true_type;
                };
                template <class T>
                using execute = typename _execute<T>::result;
            };
            template <class T>
            using execute = typename xy::template filter<f<T>>::first::tail::first;
        };
    }
}