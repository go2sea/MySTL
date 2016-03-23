#ifndef STACK_H
#define STACK_H

#include "List.h"
#include "Utility.h"

namespace MySTL {
	template <class T, class Container = MySTL::list<T>>
	class stack {
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::reference reference;
		typedef typename Container::size_type size_type;
		typedef Container container_type;

		size_type size()const { return container.size(); }
		bool empty()const { return container.empty(); }
		reference top() { return container.back(); }
		void push(const T& value) { container.push_back(value); }
		void pop() { container.pop_back(); }
		void swap(stack& s) { MySTL::swap(container, s.container); }

		bool operator == (const stack& s) { return container == s.container; }
		bool operator != (const stack& s) { return container != s.container; }
	private:
		container_type container;
	};
}//end of namespace MySTL

#endif