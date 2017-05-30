// native_cpp_main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "types.h"
template <class ... Ts>
static constexpr int f(::dlib::Types<Ts...> *) {
    return sizeof...(Ts);
}
template <class Ts>
static constexpr int f(::dlib::Types<Ts> *) {
    return -1;
}
static constexpr auto v = f((::dlib::Types<int>*)nullptr);
;int main()
{
    return 0;
}

