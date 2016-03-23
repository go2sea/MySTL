#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Iterator.h"
#include "Utility.h"
#include "Functional.h"

#include <cstring>


namespace MySTL {
	/************fill系列，用于POD类型的填充************/
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
	//返回两个迭代器之间的距离
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
	//向上调整新push的last-1指向的元素到合适的位置（除新push的元素，其他元素应事先已是一heap）
	//first：总树根  last：总树最后一个叶节点的后一个位置
	template <class RandomAccessIterator, class Compare>
	void up(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		if (first == last - 1)
			return;
		auto parentIndex = (last - first) / 2 - 1;
		//cur指向当前要调整（上移）的节点
		for (auto cur = last - 1; parentIndex >= 0 && cur != first; parentIndex = (cur - first - 1) / 2) {
			auto parent = first + parentIndex;
			if (comp(*parent, *cur))//默认，如果*parent<*cur
				MySTL::swap(*parent, *cur);
			cur = parent;
		}
	}
	//向下调整子树的根节点至合适的位置（该子树的子树应事先调整完毕）
	//first：子树根节点  last：子树最后一个叶节点的后一个位置  head：总根节点
	template <class RandomAccessIterator, class Compare>
	void down(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator head, Compare comp) {
		if (first == last - 1)
			return;
		auto leftChildIndex = (first - head) * 2 + 1;//容器0位置元素不为空时计算左孩子公式
		for (auto cur = first; leftChildIndex < last - head && cur < last - 1; leftChildIndex = (cur - head) * 2 + 1) {
			auto child = head + leftChildIndex;
			if (child + 1 < last && *(child + 1) > *child)//child指向左右孩子较大的那个
				child = child + 1;
			if (comp(*cur, *child))//默认是less_struct，*cur < *child时返回true
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
			//cur：当前子树的根  last：总树最后叶节点后一个位置  first：总树根
			MySTL::down(cur, last, first, comp);
			//cur小于first之后有时无法得出cur>=first的正确性
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
		//注意：原先必须已经是一heap，只调整push进的那一元素即可
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
			if (*(first + index * 2 + 1) > *cur || //左孩子 > 父节点
				(first + index * 2 + 2) <= last && *(first + index * 2 + 2) > *cur) //右孩子 > 父节点
				return false;
			//cur小于first之后有时无法得出cur>=first的正确性
			if (cur == first)
				break;
		}
		return true;
	}
}


#endif