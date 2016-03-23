#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"
#include "Utility.h"

namespace MySTL {
	template <class T, class Container = MySTL::list<T>>
	class queue {
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::reference reference;
		typedef typename Container::size_type size_type;
		typedef Container container_type;

		queue() {};
		//¿½±´¹¹Ôìº¯Êý
		queue(const queue& q) { container = q.container; }

		bool empty()const { return container.empty(); }
		size_type size()const { return container.size(); }
		reference front()const { return container.front(); }
		reference back()const { return container.back(); }
		void pop() { container.pop_front(); }
		void push(const T& value) { container.push_back(value); }
		void swap(queue& q) { MySTL::swap(container, q.container); }
		bool operator == (queue& q) { return container == q.container; }
		bool operator != (queue& q) { return container != q.container; }
	private:
		Container container;
	};
}//end of namespace MySTL


#endif