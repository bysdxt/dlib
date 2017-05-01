#pragma once
#include <type_traits>
#include <limits>

namespace dlib {
    namespace type_checker {
        template <size_t size> struct type_size {
            template <class T> using is_equal = ::std::bool_constant<(sizeof(T) == size)>;
        };
        template <size_t align> struct type_align {
            template <class T> using is_equal = ::std::bool_constant<(alignof(T) == align)>;
        };
        template <unsigned long long size> struct type_max_value {
            template <class T> using is_enough = ::std::bool_constant<(::std::numeric_limits<T>::max() >= size)>;
        };
        template <long long min_value_need, unsigned long long max_value_need> struct type_max_value {
            template <class T> using is_enough = ::std::bool_constant<((::std::numeric_limits<T>::min() <= min_value_need) && (::std::numeric_limits<T>::max() >= max_value_need))>;
        };
    }
}
