#ifndef UNINITIALIZED_FUNCTIONS_H
#define UNINITIALIZED_FUNCTIONS_H

#include "Algorithm.h"
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"


namespace MySTL {

	/********************************/
	/********************************/
	//T是POD类型
	template <class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _true_type) {
		return fill_n(first, n, value);//简单的内存拷贝，在Algorithm.h中定义
	}
	//T不是POD类型
	template <class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _false_type) {

		int i = 0;
		for (; i < n; i++)
			//注意：萃取机制和迭代器 需保证此处的first就是T*类型
			construct((T*)(first + i), value);
		return first + i;

	}
	//根据萃取到的isPODType调用不同的_uninitialized_fill_n_aux
	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		return _uninitialized_fill_n_aux(first, n, value, isPODType());
	}






	/********************************/
	/********************************/
	//迭代器的value_type是POD类型

	//根据萃取到的isPODType调用不同的_uninitialized_fill_aux
	template <class ForwardIterator, class T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_uninitialized_fill_aux(first, last, value, isPODType());
	}
	//T是POD类型
	template <class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type) {
		fill(first, last, value);//简单的内存拷贝，在Algorithm.h中定义
	}
	//T不是POD类型
	template <class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type) {
		for (; first != last; first++)
			//注意：萃取机制和迭代器 需保证此处的first就是T*类型
			construct((T*)first, value);
	}
	









	/********************************/
	/********************************/
	//迭代器的value_type是POD类型
	template <class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type) {
		//注意：萃取机制和迭代器 需保证此处的first last result就是T*类型
		memcpy(result, first, (last - first)*sizeof(*first));
		return result + (last - first);
	}
	//迭代器的value_type不是POD类型
	template <class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type) {
		//注意：萃取机制和迭代器 需保证此处的first last result就是T*类型
		int i = 0;
		for (; i < last - first; i++)
			construct((result + i), *(first + i));
		return result + i;
	}
	//根据对 "迭代器的value_type" 萃取得到的isPODType调用不同的_uninitialized_copy_aux
	template <class InputIterator, class ForwardIterator>
	//InputIerator：只读    ForwardIterator：允许写入型算法
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		//嵌套萃取
		typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _uninitialized_copy_aux(first, last, result, isPODType());
	}



}




#endif


