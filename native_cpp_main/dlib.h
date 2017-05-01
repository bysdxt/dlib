#pragma once
#include "choose_type.h"
#include "type_checker.h"
namespace dlib {
    template <size_t size> using SInt =
        choose_type<signed __int8, signed __int16, signed __int32, signed __int64>::by
            <type_checker::type_size<size>::template is_equal>;
    template <size_t size> using UInt =
        choose_type<unsigned __int8, unsigned __int16, unsigned __int32, unsigned __int64>::by
            <type_checker::type_size<size>::template is_equal>;
    template <size_t size> using Float =
        choose_type<float, double, long double>::by
            <type_checker::type_size<size>::template is_equal>;
}