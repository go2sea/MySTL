#ifndef SLIST_H
#define SLIST_H

#include "Allocator.h"
#include "Construct.h"
#include "TypeTraits.h"
#include "Iterator.h"
#include "Utility.h"


#include <algorithm>

namespace MySTL {
	//��������ڵ�����ṹ
	struct slist_node_base {
		slist_node_base* next;
	};
	//��������ڵ�ṹ
	template <class T>
	struct slist_node : public slist_node_base {
		T data;
	};


	/*********************************************************************************/
	/********************************ȫ�ֺ���******************************************/
	/*********************************************************************************/

	//ȫ�ֺ�������new_node���뵽prev_node֮��
	inline slist_node_base* __slist_make_link(slist_node_base* prev_node, slist_node_base* new_node) {
		new_node->next = prev_node->next;
		prev_node->next = new_node;
		return new_node;
	}
	//ȫ�ֺ�������ȡ�����ȣ�nodeΪ��һ����ֵ�ڵ㣩
	inline size_t __slist_size(slist_node_base* node) {
		size_t result = 0;
		for (; node != 0; node = node->next)
			result++;
		return result;
	}
	//ȫ�ֺ�������ȡǰһ���ڵ㣨��node���������У����ؿգ�
	inline slist_node_base* __slist_previous(slist_node_base* head, const slist_node_base* node) {
		while (head && head->next != node)
			head = head->next;
		return head;
	}



	//������������������ṹ
	struct slist_iterator_base {
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef forward_iterator_tag iterator_category;

		//ע�⣺�ǻ����ṹ��base���ڵ������б�Ҫ��λ��Ҫǿ��ת��Ϊslist_node*
		slist_node_base* node;

		//���캯��
		slist_iterator_base(slist_node_base* x) : node(x) {}

		//ǰ��һ���ڵ�
		void incr() { node = node->next; }

		bool operator==(const slist_iterator_base& x)const {
			return node == x.node;
		}
		bool operator!=(const slist_iterator_base& x)const {
			return node != x.node;
		}
	};
	//��������������ṹ
	template <class T, class Ref, class Ptr>
	struct slist_iterator : public slist_iterator_base {

		typedef slist_iterator<T, T&, T*> iterator;
		typedef slist_iterator<T, Ref, Ptr> self;//һ�����������Լ�֮�������

		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef slist_node<T> list_node;

		//���캯��
		slist_iterator(list_node* x) : slist_iterator_base(x) {}
		slist_iterator() : slist_iterator_base(0) {}
		slist_iterator(const iterator& x) : slist_iterator_base(x.node) {}

		//ע�⣺ǿ��ת��
		reference operator*()const { return ((list_node*)node)->data; }
		pointer operator->()const { return &(operator*()); }

		//ǰ��
		self& operator++() {
			incr();
			return *this;
		}
		//����
		self operator++(int) {
			self temp = *this;
			incr();
			return temp;
		}

		//û��ʵ��operator--������һ��forward_iterator
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
		list_node_base head;//ע�⣬����ָ��

	public:
		slist() { head.next = 0; }
		~slist() { clear(); };

		iterator begin() { return iterator((list_node*)head.next); }
		iterator end() { return iterator(0); }
		size_type size() { return __slist_size(head.next); }
		bool empty() { return head.next == 0; }
		//����slist������ֻ�轻��head
		void swap(slist& L) { MySTL::swap(head, L.head); }
		reference front() { return ((list_node*)head.next)->data; }
		void push_front(const value_type& value) { __slist_make_link(&head, create_node(value)); }
		void pop_front() {
			list_node* node = (list_node*)head.next;
			head.next = node->next;
			destroy_node(node);
		}
		void clear();
		//ɾ����ǰ��������һ��
		iterator erase(iterator pos) {
			return iterator((list_node*)erase_after(__slist_previous(&head, pos.node)));
		}
		void reverse() {
			if (head.next)
				head.next = slist_reverse(head.next);
		}

		/*********insertϵ�У�֮ǰ���룩**************/
		//��pos_prev֮ǰ��pos_prev��ָ�ڵ���ƣ�����ֵΪvalue�Ľڵ�
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


		/*********insert_afterϵ�У�֮����룩********/
		//��pos_prev֮�����ֵΪvalue�Ľڵ�
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





		//��ɸ�ֵ����������ԭ�ȵ��������ٲ��ͷ�
		slist<T>& operator=(slist<T>& L);

	private:
		list_node* create_node(const value_type& value);
		static void destroy_node(list_node* node) {
			destroy(&node->data);
			list_node_allocator::deallocate(node);
		}
		//����pos����Ľڵ㣬���뱣֤node->next��Ϊ�գ�ע�ⷵ��ֵΪnext_next
		list_node_base* erase_after(list_node_base* node);
		//������before_list, last_node������Ľڵ㣬�����䣬����last_node
		list_node_base* erase_after(list_node_base* before_first, list_node_base* last_node);
		//�����nodeӦΪhead->next���ұ�֤node��Ϊ��
		list_node_base* slist_reverse(list_node_base* node);
		
		/****����insert*****/
		/****����insert*****/

		//��pos֮�����ֵΪvalue�Ľڵ㣨ע�ⷵ��ֵ���ͣ�
		list_node* _insert_after(list_node_base* node_prev, const value_type& value) {
			return (list_node*)__slist_make_link(node_prev, create_node(value));
		}
		//pos֮�����[first, last)ǰ�պ�
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

	//�����nodeӦΪhead->next���ұ�֤node��Ϊ��
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

	//���뱣֤node->next��Ϊ�գ�ע�ⷵ��ֵΪnext_next
	template <class T>
	typename slist<T>::list_node_base* slist<T>::erase_after(list_node_base* node) {
		list_node* next = (list_node*)node->next;
		list_node_base* next_next = next->next;
		node->next = next_next;
		destroy_node(next);
		return next_next;
	}

	//������before_list, last_node������Ľڵ㣬�����䣬����last_node
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


	//��ɸ�ֵ����������ԭ�ȵ��������ٲ��ͷ�
	template <class T>
	slist<T>& slist<T>::operator=(slist<T>& L) {
		if (this != &L) {
			list_node_base* p1 = &head;
			list_node* n1 = (list_node*)head.next;
			list_node* n2 = (list_node*)L.head.next;
			while (n1 && n2) {
				n1->data = n2->data;
				p1 = n1;//��¼n1��ǰ�������ں�����ܵ��ͷ�
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