#ifndef SLIST_H
#define SLIST_H

#include "Allocator.h"
#include "Construct.h"
#include "TypeTraits.h"
#include "Iterator.h"
#include "Utility.h"


#include <algorithm>

namespace MySTL {
	//单向链表节点基本结构
	struct slist_node_base {
		slist_node_base* next;
	};
	//单向链表节点结构
	template <class T>
	struct slist_node : public slist_node_base {
		T data;
	};


	/*********************************************************************************/
	/********************************全局函数******************************************/
	/*********************************************************************************/

	//全局函数：将new_node插入到prev_node之后
	inline slist_node_base* __slist_make_link(slist_node_base* prev_node, slist_node_base* new_node) {
		new_node->next = prev_node->next;
		prev_node->next = new_node;
		return new_node;
	}
	//全局函数：获取链表长度（node为第一个有值节点）
	inline size_t __slist_size(slist_node_base* node) {
		size_t result = 0;
		for (; node != 0; node = node->next)
			result++;
		return result;
	}
	//全局函数：获取前一个节点（若node不在链表中，返回空）
	inline slist_node_base* __slist_previous(slist_node_base* head, const slist_node_base* node) {
		while (head && head->next != node)
			head = head->next;
		return head;
	}



	//单向链表迭代器基本结构
	struct slist_iterator_base {
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef forward_iterator_tag iterator_category;

		//注意：是基本结构（base）在迭代器中必要的位置要强制转换为slist_node*
		slist_node_base* node;

		//构造函数
		slist_iterator_base(slist_node_base* x) : node(x) {}

		//前进一个节点
		void incr() { node = node->next; }

		bool operator==(const slist_iterator_base& x)const {
			return node == x.node;
		}
		bool operator!=(const slist_iterator_base& x)const {
			return node != x.node;
		}
	};
	//单向链表迭代器结构
	template <class T, class Ref, class Ptr>
	struct slist_iterator : public slist_iterator_base {

		typedef slist_iterator<T, T&, T*> iterator;
		typedef slist_iterator<T, Ref, Ptr> self;//一般用于自增自减之类的运算

		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef slist_node<T> list_node;

		//构造函数
		slist_iterator(list_node* x) : slist_iterator_base(x) {}
		slist_iterator() : slist_iterator_base(0) {}
		slist_iterator(const iterator& x) : slist_iterator_base(x.node) {}

		//注意：强制转换
		reference operator*()const { return ((list_node*)node)->data; }
		pointer operator->()const { return &(operator*()); }

		//前置
		self& operator++() {
			incr();
			return *this;
		}
		//后置
		self operator++(int) {
			self temp = *this;
			incr();
			return temp;
		}

		//没有实现operator--：这是一个forward_iterator
	};





	template <class T>
	class slist {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef slist_iterator<T, T&, T*> iterator;

	private:
		typedef slist_node<T> list_node;
		typedef slist_node_base list_node_base;
		typedef slist_iterator_base iterator_base;
		typedef Allocator<list_node> list_node_allocator;

	private:
		list_node_base head;//注意，不是指针

	public:
		slist() { head.next = 0; }
		~slist() { clear(); };

		iterator begin() { return iterator((list_node*)head.next); }
		iterator end() { return iterator(0); }
		size_type size() { return __slist_size(head.next); }
		bool empty() { return head.next == 0; }
		//两个slist交换，只需交换head
		void swap(slist& L) { MySTL::swap(head, L.head); }
		reference front() { return ((list_node*)head.next)->data; }
		void push_front(const value_type& value) { __slist_make_link(&head, create_node(value)); }
		void pop_front() {
			list_node* node = (list_node*)head.next;
			head.next = node->next;
			destroy_node(node);
		}
		void clear();
		//删除当前，返回下一个
		iterator erase(iterator pos) {
			return iterator((list_node*)erase_after(__slist_previous(&head, pos.node)));
		}
		void reverse() {
			if (head.next)
				head.next = slist_reverse(head.next);
		}

		/*********insert系列（之前插入）**************/
		//在pos_prev之前（pos_prev所指节点后移）插入值为value的节点
		iterator insert(iterator pos_prev, const value_type& value) {
			return iterator(_insert_after(__slist_previous(&head, pos_prev.node), value));
		}
		template <class InputIterator>
		void insert(iterator pos_prev, InputIterator first, InputIterator last) {
			_insert_after_aux(__slist_previous(&head, pos_prev.node), first, last, std::is_integral<InputIterator>());
		}
		void insert(iterator pos_prev, size_type n, const value_type& value) {
			_insert_after_aux(__slist_previous(&head, pos_prev.node, n, value), std::is_integral<size_type>());
		}


		/*********insert_after系列（之后插入）********/
		//在pos_prev之后插入值为value的节点
		iterator insert_after(iterator pos, const value_type& value) {
			return _insert_after(pos.node, value);
		}
		void insert_after(iterator pos, size_type n, const value_type& value) {
			_insert_after_aux(pos.node, n, value, std::is_integral<size_type>());
		}
		template <class InputIterator>
		void insert_after(iterator pos, InputIterator first, InputIterator last) {
			_insert_after_aux(pos.node, first, last, std::is_integral<InputIterator>());
		}





		//完成赋值操作，并将原先的链表销毁并释放
		slist<T>& operator=(slist<T>& L);

	private:
		list_node* create_node(const value_type& value);
		static void destroy_node(list_node* node) {
			destroy(&node->data);
			list_node_allocator::deallocate(node);
		}
		//擦除pos后面的节点，必须保证node->next不为空，注意返回值为next_next
		list_node_base* erase_after(list_node_base* node);
		//擦除（before_list, last_node）区间的节点，开区间，返回last_node
		list_node_base* erase_after(list_node_base* before_first, list_node_base* last_node);
		//传入的node应为head->next，且保证node不为空
		list_node_base* slist_reverse(list_node_base* node);
		
		/****各种insert*****/
		/****各种insert*****/

		//在pos之后插入值为value的节点（注意返回值类型）
		list_node* _insert_after(list_node_base* node_prev, const value_type& value) {
			return (list_node*)__slist_make_link(node_prev, create_node(value));
		}
		//pos之后插入[first, last)前闭后开
		template <class InputIterator>
		void _insert_after_aux(list_node_base* pos, InputIterator first, InputIterator last, std::false_type) {
			while (first != last) {
				auto temp = *first;
				pos = __slist_make_link(pos, create_node(temp));
				first++;
			}
		}
		void _insert_after_aux(list_node_base* pos, size_type n, const value_type& value, std::true_type) {
			for (size_type i = 0; i < n; i++)
				pos = __slist_make_link(pos, create_node(value));
		}
		
	};

	template <class T>
	void slist<T>::clear() {
		list_node* temp1 = (list_node*)head.next;
		while (temp1) {
			list_node* temp2 = (list_node*)temp1->next;
			destroy_node(temp1);
			temp1 = temp2;
		}
		head.next = 0;
	}

	//传入的node应为head->next，且保证node不为空
	template <class T>
	typename slist<T>::list_node_base* slist<T>::slist_reverse(list_node_base* node) {
		list_node_base* result = node;
		node = node->next;
		result->next = 0;
		while (node) {
			list_node_base* next = node->next;
			node->next = result;
			result = node;
			node = next;
		}
		return result;
	}

	template <class T>
	typename slist<T>::list_node* slist<T>::create_node(const value_type& value) {
		list_node* node = list_node_allocator::allocate();
		construct(&(node->data), value);
		node->next = 0;
		return node;
	}

	//必须保证node->next不为空，注意返回值为next_next
	template <class T>
	typename slist<T>::list_node_base* slist<T>::erase_after(list_node_base* node) {
		list_node* next = (list_node*)node->next;
		list_node_base* next_next = next->next;
		node->next = next_next;
		destroy_node(next);
		return next_next;
	}

	//擦除（before_list, last_node）区间的节点，开区间，返回last_node
	template <class T>
	typename slist<T>::list_node_base* slist<T>::erase_after(list_node_base* before_first, list_node_base* last_node) {
		list_node_base* cur = before_first->next;
		while (cur != last_node) {
			list_node_base* next = cur->next;
			destroy_node((list_node*)cur);
			cur = next;
		}
		before_first->next = last_node;
		return last_node;
	}


	//完成赋值操作，并将原先的链表销毁并释放
	template <class T>
	slist<T>& slist<T>::operator=(slist<T>& L) {
		if (this != &L) {
			list_node_base* p1 = &head;
			list_node* n1 = (list_node*)head.next;
			list_node* n2 = (list_node*)L.head.next;
			while (n1 && n2) {
				n1->data = n2->data;
				p1 = n1;//记录n1的前驱，用于后面可能的释放
				n1 = (list_node*)n1->next;
				n2 = (list_node*)n2->next;
			}
			if (n2 == 0)
				erase_after(p1, 0);
			else
				insert_after(iterator((list_node*)p1), iterator(n2), L.end());

		}
		return *this;
	}
}


#endif