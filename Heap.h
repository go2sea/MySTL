#ifndef HEAP_H
#define HEAP_H

#include "Vector.h"

namespace MySTL {
	template <class T>
	class heap {
		typedef typename Container::value_type value_type;
		typedef typename Container::reference reference;
		typedef typename Container::size_type size_type;
		typedef MySTL::vector<T> container_type;
	public:
		template <class RandomAccessIterator>
		void make_heap(RandomAccessIterator first, RandomAccessIterator last);
		template <class RandomAccessIterator>
		void push_heap(RandomAccessIterator first, RandomAccessIterator last);
		template <class RandomAccessIterator>
		void pop_heap(RandomAccessIterator first, RandomAccessIterator last);
	private:
		container_type container;
	};

	template <class T>
	template <class RandomAccessIterator>
	void heap<T>::make_heap(RandomAccessIterator first, RandomAccessIterator last) {

	}
}


#endif


