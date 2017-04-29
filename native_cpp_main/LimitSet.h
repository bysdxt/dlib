#pragma once
#include <mutex>
#include <atomic>

using T = int;	//
//template <class T>
struct LimitSet_default_param {
	using AtomicType = size_t;
	
};

/*template <class T, class param = LimitSet_default_param<T> >*/
class LimitSet {
	using param = LimitSet_default_param;	//	
private:
	/*
	×î»ù´¡µÄ
	*/
	struct Node1 {

	};
};