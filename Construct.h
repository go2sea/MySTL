#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>

#include "TypeTraits.h"//TODO

namespace MySTL {

	template <class T1, class T2>
	inline void construct(T1* ptr, const T2& value) {
		new(ptr)T1(value);//placement new;调用T1::T1(value);
	}


	template <class T>
	inline void destroy(T* ptr) {
		ptr->~T();
	}

	//是POD类型
	template <class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type) {}

	//不是POD类型
	template <class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first != last; first++) {
			destroy(&*first);//注意：first可能是迭代器，不一定是指针，所以直接destroy(first)可能出错
		}
	}

	template <class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		typedef typename _type_traits<T>::is_POD_type is_POD_type;
		//根据萃取的is_POD_type调用不同的_destroy
		_destroy(first, last, is_POD_type);
	}
}


#endif