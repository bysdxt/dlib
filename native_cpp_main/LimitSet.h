#pragma once
#include <mutex>
#include <atomic>
#include <utility>
#include <tuple>
using T = ::std::tuple<void*, void*>;	//
//template <class T>
struct LimitSet_default_param {
#pragma region atomic, the smallest unit
    using AtomicType = ::size_t;
    static constexpr auto const AtomicZero = (AtomicType)0;
    static constexpr auto const AtomicFull = (AtomicType)(-1LL);
    static constexpr auto const AtomicOne = (AtomicType)1;
    static constexpr auto const SizeOfAtomic = sizeof(AtomicType);
    static constexpr auto const BitsOfAtomic = 8 * SizeOfAtomic;
    static inline AtomicType findOne(AtomicType value, unsigned& bit_index) {
        if (value) {
            auto mark = AtomicFull;
            auto bits = BitsOfAtomic;
            auto i = 0;
            while (bits > 1) {
                auto low_mark = mark & (mark >> (bits >>= 1));
                if (auto low_value = value & low_mark) { //  low part has one
                    value = low_value;
                    mark = low_mark;
                } else {
                    mark = mark & ~low_mark;
                    value &= mark;
                    i += bits;
                }
            }
            bit_index = bits;
            return mark;
        } else
            return AtomicZero;
    }
#pragma endregion

#pragma region Address, the index of element
    using AddressType = ::size_t;

#pragma endregion

#pragma region for T

#pragma endregion


};

/*template <class T, class param = LimitSet_default_param<T> >*/
class LimitSet {
    using param = LimitSet_default_param;	//	
private:
    struct Node1 {
        ::std::atomic<param::AtomicType> inMark = param::AtomicFull;
        ::std::atomic<param::AtomicType> outMark = param::AtomicZero;
        T slot[param::BitsOfAtomic];
        bool is_can_in() {  //  just fast test
            return param::AtomicZero != inMark.load();
        }
        bool is_can_out() { //  just fast text
            return param::AtomicZero != outMark.load();
        }
        bool save(T&& value, param::AddressType& saved_index) {
            unsigned index;
            auto mark = inMark.load();
            auto one = param::findOne(mark, index);
            while (one) {
                auto new_mark = mark ^ one;
                if (inMark.compare_exchange_strong(mark, new_mark)) {
                    //  good
                    slot[index] = ::std::move(value);
                    ::std::atomic_thread_fence(::std::memory_order::memory_order_seq_cst);
                    do {
                        mark = outMark.load();
                    } while (!outMark.compare_exchange_strong(mark, mark | one));   //  `one` means that slot[index] is empty(before this func called), and outMark must be zero(in index bit), just set it
                    saved_index = index;
                    return true;
                } else
                    one = param::findOne(mark = inMark.load(), index);
            }
            return false;
        }
        bool get(T& value) {
            unsigned index;
            auto mark = outMark.load();
            auto one = param::findOne(mark, index);
            while (one) {
                auto new_mark = mark ^ one;
                if (outMark.compare_exchange_strong(mark, new_mark)) {
                    //  good
                    value = ::std::move(slot[index]);
                    ::std::atomic_thread_fence(::std::memory_order::memory_order_seq_cst);
                    do {
                        mark = inMark.load();
                    } while (!inMark.compare_exchange_strong(mark, mark | one));
                    return true;
                } else
                    one = param::findOne(mark = outMark.load(), index);
            }
            return false;
        }
        bool get(T&value, unsigned index) {
            auto one = param::AtomicOne << index;
            auto mark = outMark.load();
            while (mark & one) {
                if (outMark.compare_exchange_strong(mark, mark ^ one)) {
                    value = ::std::move(slot[index]);
                    ::std::atomic_thread_fence(::std::memory_order::memory_order_seq_cst);
                    do {
                        mark = inMark.load();
                    } while (!inMark.compare_exchange_strong(mark, mark | one));
                    return true;
                } else
                    mark = outMark.load();
            }
            return false;
        }
    };

};