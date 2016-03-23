#ifndef DEQUE_H
#define DEQUE_H

#include "TypeTraits.h"
#include "Iterator.h"
#include "Allocator.h"
#include "UninitializedFunctions.h"
#include <algorithm>
#include "Utility.h"

namespace MySTL {

	inline size_t __deque_buffer_size(size_t n, size_t T_sz) {
		return n != 0 ? n : (T_sz < 512 ? size_t(512 / T_sz) : size_t(1));
	}

	template <class T, class Ref, class Ptr, size_t BuffSize>//BuffSizΪ�Զ���Ļ�������С
	struct deque_iterator {
		typedef deque_iterator<T, T&, T*, BuffSize> iterator;
		
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T** map_pointer;

		T* cur;		//�õ�����ָ���Ԫ��
		T* first;	//�õ�����ָ���Ԫ�����ڵĻ�������ͷ
		T* last;	//�õ�����ָ���Ԫ�����ڵĻ�������β
		map_pointer node;//ָ��ܿ������� ָ��cur���ڻ������Ľڵ�

		//deque_iterator(T* x, map_pointer y)
		//	: cur(x), first(*y), last(*y + buffer_size()), node(y) {}
		//deque_iterator() : cur(0), first(0), last(0), node(0) {}
		//deque_iterator(const iterator& x)
		//	: cur(x.cur), first(x.first), last(x.last), node(x.node) {}


		//��������ȡ�������С
		static size_t buffer_size() {
			return __deque_buffer_size(BuffSize, sizeof(T));//˫�»��߱�ʾinline������
		}
		//ע�⣺set_node��û������cur
		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		void swap(deque_iterator<T, T&, T*, BuffSize>& x) {
			MySTL::swap(cur, x.cur);
			MySTL::swap(first, x.first);
			MySTL::swap(last, x.last);
			MySTL::swap(node, x.node);
		}
		reference operator*() { return *cur; }
		pointer operator->() { return &(operator*()); }
		difference_type operator-(const deque_iterator& x)const {
			return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
		}
		//ǰ��++
		deque_iterator& operator++() {
			if (++cur == last) {
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		//����++
		deque_iterator operator++(int) {
			deque_iterator result = *this;
			++*this;
			return result;
		}
		//ǰ��--
		deque_iterator& operator--() {
			if (cur == first) {
				set_node(node - 1);
				cur = last;
			}
			cur--;
			return *this;
		}
		//����--
		deque_iterator operator--(int) {
			deque_iterator result = *this;
			--*this;
			return result;
		}
		//�����ȡ����������ֱ����Ծn������
		deque_iterator& operator+=(difference_type n) {
			difference_type offset = n + (cur - first);//����ڵ�ǰ��������first��ƫ����
			//����绺����
			if (offset >= 0 && offset < buffer_size())
				cur += n;
			//�绺����
			else {
				difference_type node_offset = offset>0 ? offset / difference_type(buffer_size())
					: -difference_type((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset*difference_type(buffer_size()));
			}
			return *this;
		}
		deque_iterator operator+(difference_type n) {
			deque_iterator temp = *this;
			return temp += n;
		}
		deque_iterator operator-=(difference_type n) {
			return *this += -n;
		}
		deque_iterator operator-(difference_type n) {
			deque_iterator temp = *this;
			return temp -= n;
		}

		reference operator[] (difference_type n) { return *(*this + n); }

		bool operator==(const deque_iterator& x)const { return cur == x.cur; }
		bool operator!=(const deque_iterator& x)const { return !(*this == x); }
		bool operator<(const deque_iterator& x)const { return node == x.node ? cur < x.cur : node < x.node; }
		bool operator>(const deque_iterator& x)const { return !(*this == x || *this < x); }
		bool operator<=(const deque_iterator& x)const { return !(*this > x); }
		bool operator>=(const deque_iterator& x)const { return !(*this < x); }
		
	};


	 template <class T, class deque_alloc = Allocator<T>, size_t BuffSize = 0>
	 class deque {
	 public:
		 typedef T value_type;
		 typedef T* pointer;
		 typedef size_t size_type;
		 typedef deque_iterator<T, T&, T*, BuffSize> iterator;
		 typedef ptrdiff_t difference_type;

		 //����ר���Ŀռ�������
		 typedef Allocator<T> data_allocator;
		 typedef Allocator<pointer> map_allocator;

	 protected:
		 typedef T** map_pointer;
		 typedef T& reference;
	 protected:
		 iterator start;
		 iterator finish;
		 map_pointer map;
		 size_type map_size;//map���ж���ָ�루�ж��ٻ�������
		 //ע�⣺�������һ���������������������������ģ�last��ָ�����һ��Ԫ�صĺ�һ�ڵ㣬�������������ģ��������ͼʾ��

	 public:
		 deque() :start(), finish(), map(0), map_size(0) { create_map_and_nodes(0); };
		 deque(int n, const value_type& value) :start(), finish(), map(0), map_size(0) { fill_initialize(n, value); }

		 template <class InputIterator>
		 deque(InputIterator begin, InputIterator end);



		 iterator begin()const { return start; }
		 iterator end()const { return finish; }
		 reference operator[](size_type n) { return start[difference_type(n)]; }
		 reference front() { return *start; }
		 reference back() { return *(finish - 1); }
		 size_type size()const { return finish - start; }
		 size_type buffer_size() { return __deque_buffer_size(BuffSize, sizeof(T)); }
		 size_type max_size() { return size_type(-1); }//�޷��ŵ�-1��ʾ���޷����������ܱ�ʾ�����ֵ
		 bool empty()const { return start == finish; }
		 static size_type initial_map_size() { return 8; }

		 void push_back(const value_type& value);
		 void push_front(const value_type& value);
		 void pop_back();
		 void pop_front();
		 void clear();
		 iterator erase(iterator pos);
		 iterator erase(iterator begin, iterator end);
		 iterator insert(iterator pos, const value_type& value);
		 void swap(deque<T, deque_alloc>& x) {
			 start.swap(x.start);
			 finish.swap(x.finish);
			 MySTL::swap(map, x.map);
			 MySTL::swap(map_size, x.map_size);
		 }



		 template <class T, class deque_alloc>
		 friend bool operator==(const deque<T, deque_alloc>& lhs, const deque<T, deque_alloc>& rhs);
		 template <class T, class deque_alloc>
		 friend bool operator!=(const deque<T, deque_alloc>& lhs, const deque<T, deque_alloc>& rhs);

	 protected:
		 void fill_initialize(int n, const value_type& value);
		 void create_map_and_nodes(size_type num_elements);
		 //����һ��������
		 pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
		 void deallocate_node(pointer p) { data_allocator::deallocate(p, buffer_size()); }
		 void push_back_aux(const value_type& value);
		 void push_front_aux(const value_type& value);
		 void pop_back_aux();
		 void pop_front_aux();
		 iterator insert_aux(iterator pos, const value_type& value);
		 void reserve_map_at_back(size_type nodes_to_add = 1) {
			 if (nodes_to_add > map_size - (finish.node - map + 1))
				 reallocate_map(nodes_to_add, false);//false:����front��push
		 }
		 void reserve_map_at_front(size_type nodes_to_add = 1) {
			 if (nodes_to_add > start.node - map)
				 reallocate_map(nodes_to_add, true);//true:��front��push
		 }
		 void reallocate_map(size_type nodes_to_add, bool add_at_front);

	 };

	 template <class T, class deque_alloc, size_t BuffSize>
	 template <class InputIterator>
	 deque<T, deque_alloc, BuffSize>::deque(InputIterator begin, InputIterator end) {
		 create_map_and_nodes(0);
		 difference_type mid_offset = (end - begin) / 2;
		 for (InputIterator cur = begin + mid_offset; cur != begin - 1; cur--)
			 push_front(*cur);
		 for (InputIterator cur = begin + mid_offset + 1; cur != end; cur++)
			 push_back(*cur);
	 }


	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::fill_initialize(int n, const value_type& value) {
		 create_map_and_nodes(n);
		 map_pointer cur;
		 for (cur = start.node; cur < finish.node; cur++)
			 MySTL::uninitialized_fill(*cur, *cur + buffer_size(), value);
		 //���һ��������������ͬ�����ÿռ䲻�����ó�ֵ
		 MySTL::uninitialized_fill(finish.first, finish.cur, value);//curָ�����һ��Ԫ�ص��߼��ϵĺ�һλ�ã���һ�����ÿռ䣩
	 }


	 

	 //����Ԫ������create map and nodes
	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::create_map_and_nodes(size_type num_elements) {
		 //��Ҫ�ڵ���
		 //����պ������������һ���ڵ㣨��Ϊ������ʱ��finish��curҪָ����һ��nodes��first��
		 size_type num_nodes = num_elements / buffer_size() + 1;
		 //һ��mapҪ�������ڵ㡣������8�������������ڵ���+2
		 map_size = std::max(initial_map_size(), num_nodes + 2);
		 //����һ������map_size���ڵ��map
		 map = map_allocator::allocate(map_size);
		 //ʹ[nstart, nfinish]����λ������
		 map_pointer nstart = map + (map_size - num_nodes) / 2;
		 map_pointer nfinish = nstart + num_nodes - 1;
		 //Ϊmap�е�ÿ�����ýڵ㣨λ��[nstart, nfinish]�����û�����
		 map_pointer cur;
		 for (cur = nstart; cur <= nfinish; cur++)
			 *cur = allocate_node();
		 start.set_node(nstart);
		 finish.set_node(nfinish);
		 start.cur = start.first;
		 finish.cur = finish.first + num_elements % buffer_size();
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::push_back(const value_type& value) {
		 //�������������ϵı�������ʱ
		 if (finish.cur != finish.last - 1) {
			 construct(finish.cur, value);
			 finish.cur++;
		 }
		 //ֻ��һ����������ʱ
		 else
			 push_back_aux(value);
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::push_back_aux(const value_type& value) {		 
		 reserve_map_at_back();	//������ĳ���������뻻һ��map

		 *(finish.node + 1) = allocate_node();//����һ���µĽڵ�
		 construct(finish.cur, value);
		 finish.set_node(finish.node + 1);
		 finish.cur = finish.first;//set_nodeû�ж�cur�������ã����set_node����Ͷ�cur�����óɶԳ���
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::push_front(const value_type& value) {
		 //ͷ������ͷ�����б��ÿռ�
		 if (start.cur != start.first) {
			 construct(start.cur - 1, value);
			 start.cur--;
		 }
		 //ͷ������û�б��ÿռ�
		 else
			 push_front_aux(value);
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::push_front_aux(const value_type& value) {
		 reserve_map_at_front();	//������ĳ���������뻻һ��map

		 *(start.node - 1) = allocate_node();//����һ���µĻ�����
		 start.set_node(start.node - 1);
		 start.cur = start.last - 1;//ע�⣬�Ǵ�β����ʼ������
		 construct(start.cur, value);
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::pop_back() {
		 if (finish.cur != finish.first) {
			 finish.cur--;
			 destroy(finish.cur);
		 }
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::pop_back_aux() {
		 deallocate_node(finish.first);
		 finish.set_node(finish.node - 1);
		 finish.cur = finish.last - 1;
		 destroy(finish.cur);
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::pop_front() {
		 if (start.cur != start.last - 1) {
			 destroy(start.cur);
			 start.cur++;
		 }
		 else
			 pop_front_aux();
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::pop_front_aux() {
		 destroy(start.cur);
		 deallocate_node(start.first);
		 start.set_node(start.node + 1);
		 start.cur = start.first;
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
		 size_type old_num_nodes = finish.node - start.node + 1;
		 size_type new_num_nodes = old_num_nodes + nodes_to_add;

		 map_pointer new_nstart;

		 //added by myself����¼cur��first�����ƫ��
		 difference_type offset_finish_cur = finish.cur - finish.first;
		 difference_type offset_start_cur = start.cur - start.first;

		 if (map_size > new_num_nodes * 2) {
			 //��λ������
			 new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			 if (new_nstart < start.node)
				 std::copy(start.node, finish.node + 1, new_nstart);
			 else
				 std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
		 }
		 else {
			 size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
			 map_pointer new_map = map_allocator::allocate(new_map_size);
			 new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			 std::copy(start.node, finish.node + 1, new_nstart);
			 //�ͷ�ԭmap
			 map_allocator::deallocate(map, map_size);

			 map = new_map;
			 map_size = new_map_size;
		 }

		 start.set_node(new_nstart);
		 finish.set_node(new_nstart + old_num_nodes - 1);

		 //added by myself���ָ�cur�����λ��
		 start.cur = start.first + offset_start_cur;
		 finish.cur = finish.first + offset_finish_cur;
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 void deque<T, deque_alloc, BuffSize>::clear() {
		 //ͷβ֮���ÿһ�������Ļ�����
		 for (map_pointer node = start.node + 1; node < finish.node; node++) {
			 destroy(*node, *node + buffer_size());
			 data_allocator::deallocate(*node, buffer_size());
		 }
		 //ͷβ��ͬ���ͷ�β
		 if (start.node != finish.node) {
			 destroy(start.cur, start.last);
			 destroy(finish.first, finish.cur);
			 data_allocator::deallocate(finish.first, buffer_size());
		 }
		 //ֻ��һ��������
		 else
			 destroy(start.cur, start.last);

		 finish = start;

		 //������Ĳ��ù�curŶ~~~~���������ˣ�curָ��������ν
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 typename deque<T, deque_alloc, BuffSize>::iterator deque<T, deque_alloc, BuffSize>::erase(iterator pos) {
		 //ע�⣺����������copy�ĸ����л�����pop��
		 iterator next = pos;
		 next++;
		 difference_type index = next - start;
		 //ԭ�򣺾������ٵ��ƶ�Ԫ��
		 //���ǰ��Ԫ�ؽ���
		 if (index < (size() >> 1)) {
			 std::copy_backward(start, pos, next);
			 pop_front();//��һ��Ԫ������
		 }
		 //�����Ԫ�ؽ���
		 else {
			 std::copy(next, finish, pos);
			 pop_back();//���һ��Ԫ������
		 }
		 return start + index;
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 typename deque<T, deque_alloc, BuffSize>::iterator deque<T, deque_alloc, BuffSize>::erase(iterator begin, iterator end) {
		 //�����������䣬ֱ��clear()
		 if (begin == start && end == finish) {
			 clear();
			 return finish;
		 }

		 difference_type n = end - begin;//������䳤��
		 difference_type elements_before = begin - start;
		 //ǰ��Ԫ�ؽ���
		 if (elements_before < (size() - n) / 2) {
			 std::copy_backward(start, begin, end);
			 iterator new_start = start + n;
			 destroy(start, new_start);
			 //�ͷ�
			 for (map_pointer node = start.node; node < new_start.node; node++)
				 data_allocator::deallocate(*node, buffer_size());
			 start = new_start;
		 }
		 //��Ԫ�ؽ���
		 else {
			 std::copy(end, finish, begin);
			 iterator new_finish = finish - n;
			 destroy(new_finish, finish);
			 for (map_pointer node = new_finish.node + 1; node <= finish.node; node++)
				 data_allocator::deallocate(*node, buffer_size());
			 finish = new_finish;
		 }
		 return start + elements_before;
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 typename deque<T, deque_alloc, BuffSize>::iterator deque<T, deque_alloc, BuffSize>::insert(iterator pos, const value_type& value) {
		 if (pos == start) {
			 push_front(value);
			 return start;
		 }
		 else if (pos == finish) {
			 push_back(value);
			 iterator result = finish;
			 return --result;
		 }
		 else
			 return insert_aux(pos, value);
	 }

	 template <class T, class deque_alloc, size_t BuffSize>
	 typename deque<T, deque_alloc, BuffSize>::iterator deque<T, deque_alloc, BuffSize>::insert_aux(iterator pos, const value_type& value) {
		 difference_type elements_before = pos - start;
		 //ǰ��Ԫ�ؽ���
		 if (elements_before < size() / 2) {
			 push_front(front());//��Ϊstart.cur�����ڻ�������Ե����push�������µĻ�������
			 iterator front1 = start + 1;
			 iterator front2 = start + 2;
			 iterator pos = start + elements_before;//����pos--����Ϊpush��ʱ�����reserve��reallocate����map���߼���pos��ǰ�ƶ���һ������ΪҪ��ǰ��Ԫ�ذ����
			 iterator pos1 = pos + 1;
			 std::copy(front2, pos1, front1);
		 }
		 //��Ԫ�ؽ���
		 else {
			 push_back(back());
			 iterator back1 = finish - 1;
			 iterator back2 = back1 - 1;
			 pos = start + elements_before;//��Ϊpush��ʱ�����reserve��reallocate����map���������¶�λһ��pos
			 std::copy_backward(pos, back2, back1);
		 }
		 *pos = value;
		 return pos;
	 }

	 template <class T, class deque_alloc>
	 bool operator==(const deque<T, deque_alloc>& lhs, const deque<T, deque_alloc>& rhs) {
		 if (lhs.size() != rhs.size())
			 return false;
		 auto lit = lhs.begin(), rit = rhs.begin();
		 for (; lit != lhs.end(); lit++, rit++)
			 if (*lit != *rit)
				 return false;
		 return true;
	 }
	 template <class T, class deque_alloc>
	 bool operator!=(const deque<T, deque_alloc>& lhs, const deque<T, deque_alloc>& rhs) {
		 return !(lhs == rhs);
	 }

}

#endif