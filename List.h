#ifndef LIST_H
#define LIST_H

#include "Algorithm.h"
#include "ReverseIterator.h"
#include "UninitializedFunctions.h"
#include "Iterator.h"
#include "Allocator.h"
#include "Functional.h"

namespace MySTL {
	//�ڵ�ṹ��
	template <class T>
	struct list_node {
		list_node<T>* next;//ֱ�Ӷ����list_node<T>*���Ͷ�����void*����������ǿ��ת��
		list_node<T>* prev;
		T data;

		bool operator == (const list_node<T>& x) {
			return next = x.next && prev == x.prev && data == x.data;
		}
		bool operator != (const list_node<T>& x) {
			return !(operator == (x));
		}
	};//end of list_node

	//�������ṹ��
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

		//���캯��
		list_iterator(list_node_ptr x) : node(x) {}
		list_iterator() {}
		list_iterator(const iterator& x) : node(x.node) {}//�������캯��

		bool operator== (const iterator& x) const { return node == x.node; }
		bool operator!= (const iterator& x) const { return node != x.node; }
		reference operator*() const { return node->data; }//ע�⣺����node�е�data������
		pointer operator-> () const { return &(operator*()); }//ע�⣺����node�е�data�ĵ�ַ
		iterator& operator++ () {	//ǰ��
			node = node->next;
			return *this;
		}
		iterator operator++ (int) {	//����
			iterator result = *this;
			++*this;
			return result;
		}
		iterator& operator-- () {	//ǰ��
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
		//typedef list_node_type* link_type;//�ڵ�ָ�룬�����list_node_ptr����?
		typedef list_node_type* list_node_ptr;//�ڵ�ָ�룬�����node_ptr����
		typedef Allocator<list_node<T>> list_node_allocator;
		typedef list_iterator<T, T&, T*> iterator;
		typedef T value_type;
		typedef size_t size_type;
		typedef T& reference;
		typedef reverse_iterator_t<iterator> reverse_iterator;

	protected:
		list_node_ptr node;//ʼ��ά����ָ��β��һ���հ׽ڵ㣬node��Ϊend()

	public:
		list() { empty_initialize(); };
		list(size_type size, const T& value);
		template <class InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& x);//�������캯��

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
		iterator erase(iterator position);		//ɾ��������position��ָ�ڵ�
		iterator erase(iterator first, iterator last);
		void swap(list& x);
		void clear();//������нڵ�
		void remove(const T& value);//���dataΪvalue�����нڵ�
		void unique();//�Ƴ�data��ͬ������Ԫ�أ�ע�⣬������Ԫ��
		void splice(iterator position, list& x);//��x�Ӻ���position֮ǰ��x���벻ͬ��*this
		void splice(iterator position, list&, iterator i);//i�Ӻ���position֮ǰ��i��position��ָ��ͬһlist
		void splice(iterator position, list&, iterator first, iterator last);//[first,last)��positionǰ�������position��ָ��ͬһlist����position����������֮��
		void merge(list& x);//��x�ϲ���*this������list�����ݱ����Ⱦ����ǵݼ�����
		template <class Compare>
		void merge(list& x, Compare comp);
		void sort();//һ����ֵ�MergeSort
		template <class Compare>
		void sort(Compare comp);
		void reverse();//�õ��Լ��ķ��������ܳ���Ӵ~
		template <class Predicate>
		void remove_if(Predicate pred);

		//����������
		bool operator == (const list& x)const;
		bool operator != (const list& x)const { return !(*this == x); }


	protected:
		//���������ò����죩һ���ڵ㣬����Ԫ��ֵ
		list_node_ptr create_node(const T& value) {
			list_node_ptr result = list_node_allocator::allocate();
			construct(&result->data, value);
			return result;
		}
		//���٣��������ͷţ�һ���ڵ�
		void destroy_node(list_node_ptr p) {
			destroy(&p->data);
			list_node_allocator::deallocate(p);
		}
		void empty_initialize() {
			node = list_node_allocator::allocate();
			node->next = node;
			node->prev = node;
		}
		//��[first,last)������Ԫ���Ƶ�position֮ǰ
		void  transfer(iterator position, iterator first, iterator last);
		//�������ֹ��캯��
		void ctor_aux(size_type size, const T& value, std::true_type);
		template <class InputIterator>
		void ctor_aux(InputIterator first, InputIterator last, std::false_type);
		//��������insert
		void insert_aux(iterator position, size_type size, const T& value, std::true_type);
		template <class InputIterator>
		void insert_aux(iterator positon, InputIterator first, InputIterator last, std::false_type);


	};//end of list


	//�������ֹ��캯��
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

	//�������캯��
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
		return temp;//����list_iterator�Ĺ��캯��list_iterator(list_node_ptr x) : node(x) {}
	}

	template <class T>
	typename list<T>::iterator list<T>::erase(iterator position) {
		list_node_ptr prev_node = position.node->prev;
		list_node_ptr next_node = position.node->next;
		next_node->prev = prev_node;
		prev_node->next = next_node;
		destroy_node(position.node);
		return next_node;//����list_iterator�Ĺ��캯��list_iterator(list_node_ptr x) : node(x) {}
	}

	template <class T>
	typename list<T>::iterator list<T>::erase(iterator first, iterator last) {
		iterator result;
		while (first != last) {
			iterator temp = first++;//�ȱ���
			result = erase(temp);
		}
		return result;
	}

	//��������insert
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
		//���棺node�ǻ��ˣ�node���ڵ�ָ��node��û�������ɴ�ֱ�ӻ�node
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
			//���λ��fillλ�ã����ǿվ�������iλ������2��i�η�������2��i+1�η���������鲢��
			if (i == fill)//���Ѿ��鲢�����ڵ�fill
				fill++;//��һ��������װ����λ��
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
		transfer(position, x.begin(), x.end());//first last position�Ƿ���ͬ��transfer�д���
	}

	template <class T>
	void list<T>::splice(iterator position, list&, iterator i) {
		iterator j = i++;
		transfer(position, j, i);//first last position�Ƿ���ͬ��transfer�д���
	}

	template <class T>
	void list<T>::splice(iterator position, list&, iterator first, iterator last) {
		transfer(position, first, last);//first last position�Ƿ���ͬ��transfer�д���
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
	//stlĬ�����򣨴���less������
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
			first--;//��Ϊ�Ѿ���ת����
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