#ifndef LIST_H
#define LIST_H

#include "Algorithm.h"
#include "ReverseIterator.h"
#include "UninitializedFunctions.h"
#include "Iterator.h"
#include "Allocator.h"
#include "Functional.h"

namespace MySTL {
	//节点结构体
	template <class T>
	struct list_node {
		list_node<T>* next;//直接定义成list_node<T>*类型而不是void*，后续不用强制转换
		list_node<T>* prev;
		T data;

		bool operator == (const list_node<T>& x) {
			return next = x.next && prev == x.prev && data == x.data;
		}
		bool operator != (const list_node<T>& x) {
			return !(operator == (x));
		}
	};//end of list_node

	//迭代器结构体
	template <class T, class Ref, class Ptr>
	struct list_iterator {
		typedef list_iterator<T, T&, T*> iterator;
		typedef list_iterator<T, Ref, Ptr> self;

		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		//typedef list_node<T>* link_type;
		typedef list_node<T>* list_node_ptr;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		list_node_ptr node;

		//构造函数
		list_iterator(list_node_ptr x) : node(x) {}
		list_iterator() {}
		list_iterator(const iterator& x) : node(x.node) {}//拷贝构造函数

		bool operator== (const iterator& x) const { return node == x.node; }
		bool operator!= (const iterator& x) const { return node != x.node; }
		reference operator*() const { return node->data; }//注意：返回node中的data的引用
		pointer operator-> () const { return &(operator*()); }//注意：返回node中的data的地址
		iterator& operator++ () {	//前置
			node = node->next;
			return *this;
		}
		iterator operator++ (int) {	//后置
			iterator result = *this;
			++*this;
			return result;
		}
		iterator& operator-- () {	//前置
			node = node->prev;
			return *this;
		}
		iterator operator-- (int) {
			iterator result = *this;
			--*this;
			return result;
		}
	};//end of list_iterator



	/**************list**************/
	/**************list**************/
	/**************list**************/
	/**************list**************/
	/**************list**************/
	template <class T>
	class list {
	public:
		typedef list_node<T> list_node_type;
		//typedef list_node_type* link_type;//节点指针，或许叫list_node_ptr更好?
		typedef list_node_type* list_node_ptr;//节点指针，或许叫node_ptr更好
		typedef Allocator<list_node<T>> list_node_allocator;
		typedef list_iterator<T, T&, T*> iterator;
		typedef T value_type;
		typedef size_t size_type;
		typedef T& reference;
		typedef reverse_iterator_t<iterator> reverse_iterator;

	protected:
		list_node_ptr node;//始终维持它指向尾端一个空白节点，node即为end()

	public:
		list() { empty_initialize(); };
		list(size_type size, const T& value);
		template <class InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& x);//拷贝构造函数

		iterator begin()const { return node->next; }
		reverse_iterator rbegin()const { return reverse_iterator(end()); }
		reverse_iterator rend()const { return reverse_iterator(begin()); }
		iterator end()const { return node; }
		bool empty()const { return node->next == node; }
		size_type size()const {
			size_type result = 0;
			for (iterator i = begin(); i != end(); i++)
				result++;
			return result;
		}
		reference front()const { return *(begin()); }
		reference back()const { return *(--end()); }

		void push_back(const T& x) { insert(end(), x); }
		void push_front(const T& x) { insert(begin(), x); }
		void pop_front() { erase(begin()); }
		void pop_back() { erase(--end()); }
		iterator insert(iterator position, const T& value);
		void insert(iterator position, size_type size, const T& value);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);		//删除迭代器position所指节点
		iterator erase(iterator first, iterator last);
		void swap(list& x);
		void clear();//清除所有节点
		void remove(const T& value);//清除data为value的所有节点
		void unique();//移除data相同的连续元素，注意，是连续元素
		void splice(iterator position, list& x);//将x接合于position之前，x必须不同于*this
		void splice(iterator position, list&, iterator i);//i接合于position之前，i和position可指向同一list
		void splice(iterator position, list&, iterator first, iterator last);//[first,last)接position前，区间和position可指向同一list，但position不能在区间之内
		void merge(list& x);//将x合并到*this，两个list的内容必须先经过非递减排序
		template <class Compare>
		void merge(list& x, Compare comp);
		void sort();//一种奇怪的MergeSort
		template <class Compare>
		void sort(Compare comp);
		void reverse();//用的自己的方法，可能出错哟~
		template <class Predicate>
		void remove_if(Predicate pred);

		//操作符重载
		bool operator == (const list& x)const;
		bool operator != (const list& x)const { return !(*this == x); }


	protected:
		//产生（配置并构造）一个节点，带有元素值
		list_node_ptr create_node(const T& value) {
			list_node_ptr result = list_node_allocator::allocate();
			construct(&result->data, value);
			return result;
		}
		//销毁（析构并释放）一个节点
		void destroy_node(list_node_ptr p) {
			destroy(&p->data);
			list_node_allocator::deallocate(p);
		}
		void empty_initialize() {
			node = list_node_allocator::allocate();
			node->next = node;
			node->prev = node;
		}
		//将[first,last)内所有元素移到position之前
		void  transfer(iterator position, iterator first, iterator last);
		//区分两种构造函数
		void ctor_aux(size_type size, const T& value, std::true_type);
		template <class InputIterator>
		void ctor_aux(InputIterator first, InputIterator last, std::false_type);
		//区分两种insert
		void insert_aux(iterator position, size_type size, const T& value, std::true_type);
		template <class InputIterator>
		void insert_aux(iterator positon, InputIterator first, InputIterator last, std::false_type);


	};//end of list


	//区分两种构造函数
	template <class T>
	void list<T>::ctor_aux(size_type size, const T& value, std::true_type) {
		empty_initialize();
		for (size_type i = 0; i < size; i++)
			push_back(value);
	}
	template <class T>
	template <class InputIterator>
	void list<T>::ctor_aux(InputIterator first, InputIterator last, std::false_type) {
		empty_initialize();
		while (first != last) {
			push_back(*first);
			first++;
		}
	}
	template <class T>
	list<T>::list(size_type size, const T& value) {
		ctor_aux(size, value, std::is_integral<size_type>());
	}
	template <class T>
	template <class InputIterator>
	list<T>::list(InputIterator first, InputIterator last) {
		ctor_aux(first, last, std::is_integral<InputIterator>());
	}

	//拷贝构造函数
	template <class T>
	list<T>::list(const list& x) {
		ctor_aux(x.begin(), x.end(), std::false_type());
	}

	template <class T>
	typename list<T>::iterator list<T>::insert(iterator position, const T& value) {
		list_node_ptr temp = create_node(value);
		temp->next = position.node;
		temp->prev = position.node->prev;
		position.node->prev->next = temp;
		position.node->prev = temp;
		return temp;//调用list_iterator的构造函数list_iterator(list_node_ptr x) : node(x) {}
	}

	template <class T>
	typename list<T>::iterator list<T>::erase(iterator position) {
		list_node_ptr prev_node = position.node->prev;
		list_node_ptr next_node = position.node->next;
		next_node->prev = prev_node;
		prev_node->next = next_node;
		destroy_node(position.node);
		return next_node;//调用list_iterator的构造函数list_iterator(list_node_ptr x) : node(x) {}
	}

	template <class T>
	typename list<T>::iterator list<T>::erase(iterator first, iterator last) {
		iterator result;
		while (first != last) {
			iterator temp = first++;//先保存
			result = erase(temp);
		}
		return result;
	}

	//区分两种insert
	template <class T>
	void list<T>::insert_aux(iterator position, size_type size, const T& value, std::true_type) {
		while (size--)
			insert(position, value);
	}
	template <class T>
	template <class InputIterator>
	void list<T>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		while (first != last) {
			insert(position, *first);
			first++;
		}
	}
	template <class T>
	void list<T>::insert(iterator position, size_type size, const T& value) {
		insert_aux(position, size, value, std::is_integral<size_type>());
	}
	template <class T>
	template <class InputIterator>
	void list<T>::insert(iterator position, InputIterator first, InputIterator last) {
		insert_aux(position, first, last, std::is_integral<InputIterator>());
	}

	template <class T>
	void list<T>::swap(list& x){
		MySTL::swap(node, x.node);
		//下面：node是换了，node相邻的指向node的没换啊，干脆直接换node
		//MySTL::swap(node->prev, x.node->prev);
		//MySTL::swap(node->next, x.node->next);
	}

	template <class T>
	void list<T>::sort() {
		sort(MySTL::less_struct<T>());
	}

	template <class T>
	template <class Compare>
	void list<T>::sort(Compare comp) {
		if (size() <= 1)
			return;
		list<T> carry;
		list<T> counter[64];
		int fill = 0;
		while (!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry, comp);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			//最高位的fill位置，不是空就是满（i位置最多放2的i次方个数，2的i+1次方就需往后归并）
			if (i == fill)//都已经归并到现在的fill
				fill++;//下一个即将被装满的位置
		}
		for (int i = 1; i < fill; i++)
			counter[i].merge(counter[i - 1], comp);
		swap(counter[fill - 1]);
	}

	template <class T>
	void list<T>::clear() {
		list_node_ptr cur = node->next;
		while (cur != node) {
			list_node_ptr temp = cur->next;
			destroy_node(cur);
			cur = temp;
		}
		node->next = node;
		node->prev = node;
	}

	template <class T>
	void list<T>::remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator temp = first++;
			if (*temp == value)
				erase(temp);
		}
	}

	template <class T>
	void list<T>::unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last)
			return;
		iterator temp = first;
		while (++temp != last) {
			if (*temp == *first) {
				erase(temp);
				temp = first;
			}
			else
				first = temp;
		}
	}

	template <class T>
	void list<T>::transfer(iterator position, iterator first, iterator last) {
		if (position == last || position == first || first == last)
			return;
		last.node->prev->next = position.node;
		first.node->prev->next = last.node;
		list_node_ptr temp = last.node->prev;
		last.node->prev = first.node->prev;
		position.node->prev->next = first.node;
		first.node->prev = position.node->prev;
		position.node->prev = temp;
	}

	template <class T>
	void list<T>::splice(iterator position, list& x) {
		transfer(position, x.begin(), x.end());//first last position是否相同在transfer中处理
	}

	template <class T>
	void list<T>::splice(iterator position, list&, iterator i) {
		iterator j = i++;
		transfer(position, j, i);//first last position是否相同在transfer中处理
	}

	template <class T>
	void list<T>::splice(iterator position, list&, iterator first, iterator last) {
		transfer(position, first, last);//first last position是否相同在transfer中处理
	}

	template <class T>
	void list<T>::merge(list& x) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		while (first1 != last1 && first2 != last2) {
			if (*first1 <= *first2) {
				first1++;
				continue;
			}
			iterator temp = first2;
			while (*first2 < *first1 && first2 != last2)
				first2++;
			transfer(first1, temp, first2);
		}
		if (first2 != last2)
			transfer(first1, first2, last2);
	}

	template <class T>
	template <class Compare>
	//stl默认升序（传入less函数）
	void list<T>::merge(list& x, Compare comp/* = std::less<T>*/) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		while (first1 != last1 && first2 != last2) {
			if (!comp(*first2, *first1)) {
				first1++;
				continue;
			}
			iterator temp = first2;
			while (comp(*first2, *first1) && first2 != last2)
				first2++;
			transfer(first1, temp, first2);
		}
		if (first2 != last2)
			transfer(first1, first2, last2);
	}

	template <class T>
	void list<T>::reverse() {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			list_node_ptr temp = first.node->prev;
			first.node->prev = first.node->next;
			first.node->next = temp;
			first--;//因为已经调转方向
		}
		//last
		list_node_ptr temp = last.node->prev;
		last.node->prev = last.node->next;
		last.node->next = temp;
	}

	template <class T>
	template <class Predicate>
	void list<T>::remove_if(Predicate pred) {
		iterator it = begin();
		while (it != end()) {
			if (pred(*it))
				it = erase(it);
			else
				it++;
		}
	}
	
	template <class T>
	bool list<T>::operator == (const list& x)const {
		if (size() != x.size())
			return false;
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		while (first1 != last1) {
			if (*first1 != *first2)
				return false;
			first1++;
			first2++;
		}
		return true;
	}


};//end of namespace MySTL
#endif