#ifndef UNINITIALIZED_FUNCTIONS_H
#define UNINITIALIZED_FUNCTIONS_H

#include "Algorithm.h"
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"


namespace MySTL {

	/********************************/
	/********************************/
	//T��POD����
	template <class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _true_type) {
		return fill_n(first, n, value);//�򵥵��ڴ濽������Algorithm.h�ж���
	}
	//T����POD����
	template <class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _false_type) {

		int i = 0;
		for (; i < n; i++)
			//ע�⣺��ȡ���ƺ͵����� �豣֤�˴���first����T*����
			construct((T*)(first + i), value);
		return first + i;

	}
	//������ȡ����isPODType���ò�ͬ��_uninitialized_fill_n_aux
	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		return _uninitialized_fill_n_aux(first, n, value, isPODType());
	}






	/********************************/
	/********************************/
	//��������value_type��POD����

	//������ȡ����isPODType���ò�ͬ��_uninitialized_fill_aux
	template <class ForwardIterator, class T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_uninitialized_fill_aux(first, last, value, isPODType());
	}
	//T��POD����
	template <class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type) {
		fill(first, last, value);//�򵥵��ڴ濽������Algorithm.h�ж���
	}
	//T����POD����
	template <class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type) {
		for (; first != last; first++)
			//ע�⣺��ȡ���ƺ͵����� �豣֤�˴���first����T*����
			construct((T*)first, value);
	}
	









	/********************************/
	/********************************/
	//��������value_type��POD����
	template <class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type) {
		//ע�⣺��ȡ���ƺ͵����� �豣֤�˴���first last result����T*����
		memcpy(result, first, (last - first)*sizeof(*first));
		return result + (last - first);
	}
	//��������value_type����POD����
	template <class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type) {
		//ע�⣺��ȡ���ƺ͵����� �豣֤�˴���first last result����T*����
		int i = 0;
		for (; i < last - first; i++)
			construct((result + i), *(first + i));
		return result + i;
	}
	//���ݶ� "��������value_type" ��ȡ�õ���isPODType���ò�ͬ��_uninitialized_copy_aux
	template <class InputIterator, class ForwardIterator>
	//InputIerator��ֻ��    ForwardIterator������д�����㷨
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		//Ƕ����ȡ
		typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _uninitialized_copy_aux(first, last, result, isPODType());
	}



}




#endif


