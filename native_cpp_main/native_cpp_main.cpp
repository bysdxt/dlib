// native_cpp_main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "choose_type.h"
template <bool test, size_t size, class T, template<size_t> class Tp>
struct __choose_type_by_size_step;
template <size_t size, class T, template<size_t> class Tp>
struct __choose_type_by_size_step<true, size, T, Tp> {
    using Result = T;
};
template <size_t size, class T, template<size_t> class Tp>
struct __choose_type_by_size_step<false, size, T, Tp> {
    using Result = typename Tp<size>;
};
template <class ... Ts>
struct __choose_type_by_size;
template <class T, class ... Ts>
struct __choose_type_by_size<T, Ts...> {
    template <size_t size>
    using Result =
        typename __choose_type_by_size_step<(sizeof(T) == size), size, T, __choose_type_by_size<Ts...>::template Result>::Result;
};
template<>
struct __choose_type_by_size<> {
    template <size_t>
    using Result = void;
};
template <size_t size, class ... Ts>
using choose_type_by_size = typename __choose_type_by_size<Ts...>::template Result<size>;

template <size_t size>
using UInt = choose_type_by_size<size, unsigned __int8, unsigned __int16, unsigned __int32, unsigned __int64>;
template <size_t size> 
using Int = choose_type_by_size<size, signed __int8, signed __int16, signed __int32, signed __int64>;
template <size_t size>
using Float = choose_type_by_size<size, float, double, long double>;

template <size_t size>
using SInt = choose_type<signed __int8, signed __int16, signed __int32, signed __int64, void>::by<type_size<size>::template tester>;
int main()
{
    UInt<2> a = 0;
    Int<4> b = 0;
    Float<8> c = 0;
    SInt<4> d = 0;
    return 0;
}

