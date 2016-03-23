#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Iterator.h"
#include "Utility.h"
#include "Functional.h"

#include <cstring>


namespace MySTL {
	/************fillϵ�У�����POD���͵����************/
	template <class OutputIterator, class size_type, class T>
	OutputIterator fill_n(OutputIterator first, size_type n, const T& value) {
		for (; n > 0; n--, first++)
			*first = value;
		return first;
	}

	template <class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; first++)
			*first = value;
	}


	/*********************distance**********************/
	//��������������֮��ľ���
	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type _distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type dist = 0;
		while (first++ != last)
			dist++;
		return dist;
	}
	template <class RandomIterator>
	typename iterator_traits<RandomIterator>::difference_type _distance(RandomIterator first, RandomIterator last, random_access_iterator_tag) {
		auto dist = last - first;
		return dist;
	}
	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type distance(Iterator first, Iterator last) {
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		return _distance(first, last, iterator_category());
	}

	/*********************heap**********************/
	//���ϵ�����push��last-1ָ���Ԫ�ص����ʵ�λ�ã�����push��Ԫ�أ�����Ԫ��Ӧ��������һheap��
	//first��������  last���������һ��Ҷ�ڵ�ĺ�һ��λ��
	template <class RandomAccessIterator, class Compare>
	void up(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		if (first == last - 1)
			return;
		auto parentIndex = (last - first) / 2 - 1;
		//curָ��ǰҪ���������ƣ��Ľڵ�
		for (auto cur = last - 1; parentIndex >= 0 && cur != first; parentIndex = (cur - first - 1) / 2) {
			auto parent = first + parentIndex;
			if (comp(*parent, *cur))//Ĭ�ϣ����*parent<*cur
				MySTL::swap(*parent, *cur);
			cur = parent;
		}
	}
	//���µ��������ĸ��ڵ������ʵ�λ�ã�������������Ӧ���ȵ�����ϣ�
	//first���������ڵ�  last���������һ��Ҷ�ڵ�ĺ�һ��λ��  head���ܸ��ڵ�
	template <class RandomAccessIterator, class Compare>
	void down(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator head, Compare comp) {
		if (first == last - 1)
			return;
		auto leftChildIndex = (first - head) * 2 + 1;//����0λ��Ԫ�ز�Ϊ��ʱ�������ӹ�ʽ
		for (auto cur = first; leftChildIndex < last - head && cur < last - 1; leftChildIndex = (cur - head) * 2 + 1) {
			auto child = head + leftChildIndex;
			if (child + 1 < last && *(child + 1) > *child)//childָ�����Һ��ӽϴ���Ǹ�
				child = child + 1;
			if (comp(*cur, *child))//Ĭ����less_struct��*cur < *childʱ����true
				MySTL::swap(*cur, *child);
			cur = child;
		}
	}
	template <class RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
		MySTL::make_heap(first, last, typename MySTL::less_struct<MySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template <class RandomAccessIterator, class Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		for (auto cur = first + (last - first) / 2 - 1; cur >= first; cur--) {
			//cur����ǰ�����ĸ�  last���������Ҷ�ڵ��һ��λ��  first��������
			MySTL::down(cur, last, first, comp);
			//curС��first֮����ʱ�޷��ó�cur>=first����ȷ��
			if (cur == first)
				return;
		}


	}
	template <class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		MySTL::push_heap(first, last, MySTL::less_struct<typename MySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template <class RandomAccessIterator, class Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		MySTL::up(first, last, comp);
		//ע�⣺ԭ�ȱ����Ѿ���һheap��ֻ����push������һԪ�ؼ���
	}
	template <class RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
		MySTL::pop_heap(first, last, MySTL::less_struct<MySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template <class RandomAccessIterator, class Compare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		MySTL::swap(*first, *(last - 1));
		if (last - first > 2)
			MySTL::down(first, last - 1, first, comp);
	}
	template <class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
		MySTL::sort_heap(first, last, MySTL::less_struct<MySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template <class RandomAccessIterator, class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		for (auto cur = last; cur != first; cur--)
			MySTL::pop_heap(first, cur, comp);
	}
	template <class RandomAccessIterator>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
		return MySTL::is_heap(first, last, MySTL::less_struct<MySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template <class RandomAccessIterator, class Compare>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		auto index = (last - first) / 2 - 1;
		for (auto cur = first + index; cur >= first; cur--, index--) {
			if (*(first + index * 2 + 1) > *cur || //���� > ���ڵ�
				(first + index * 2 + 2) <= last && *(first + index * 2 + 2) > *cur) //�Һ��� > ���ڵ�
				return false;
			//curС��first֮����ʱ�޷��ó�cur>=first����ȷ��
			if (cur == first)
				break;
		}
		return true;
	}
}


#endif