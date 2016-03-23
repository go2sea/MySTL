#ifndef PRIORITYQUEUE_H
#define PREORITYQUEUE_H

#include "Vector.h"


namespace MySTL {
	template <class T, class Container = MySTL::vector<T>,  class Compare = MySTL::less_struct<T>>
	class priority_queue {
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
	public:
		priority_queue() : container() {}
		explicit priority_queue(const Compare& c) : container(), comp(c) {}
		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& x)
			: container(first, last), comp(x) { 
			make_heap(container.begin(), container.end(), comp); 
		}
		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last)
			: container(first, last) {
			make_heap(container.begin(), container.end(), comp);
		}
		priority_queue(priority_queue& value) {
			container = value.container;
			comp = value.comp;
		}

		bool empty()const { return container.empty(); }
		size_type size()const { return container.size(); }
		const_reference top()const { return container.front(); }
		void push(const value_type& x) {
			container.push_back(x);
			MySTL::push_heap(container.begin(), container.end(), comp);
		}
		void pop() {
			MySTL::pop_heap(container.begin(), container.end(), comp);
			container.pop_back();
		}
		void swap(priority_queue& pq) {
			MySTL::swap(container, pq.container);
			MySTL::swap(comp, pq.comp);
		}

		template <class T, class Container, class Compare>
		friend void swap(priority_queue<T, Container, Compare>& x, priority_queue<T, Container, Compare>& y);

	private:
		Container container;
		Compare comp;
	};
	template <class T, class Container, class Compare>
	void swap(priority_queue<T, Container, Compare>& x, priority_queue<T, Container, Compare>& y){

		x.swap(y);
	}
}

#endif