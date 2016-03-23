#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>

#include "TypeTraits.h"//TODO

namespace MySTL {

	template <class T1, class T2>
	inline void construct(T1* ptr, const T2& value) {
		new(ptr)T1(value);//placement new;����T1::T1(value);
	}


	template <class T>
	inline void destroy(T* ptr) {
		ptr->~T();
	}

	//��POD����
	template <class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type) {}

	//����POD����
	template <class ForwardIterator>
	inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first != last; first++) {
			destroy(&*first);//ע�⣺first�����ǵ���������һ����ָ�룬����ֱ��destroy(first)���ܳ���
		}
	}

	template <class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		typedef typename _type_traits<T>::is_POD_type is_POD_type;
		//������ȡ��is_POD_type���ò�ͬ��_destroy
		_destroy(first, last, is_POD_type);
	}
}


#endif