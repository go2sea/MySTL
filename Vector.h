#pragma warning(disable:4996) 
#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <iostream>

#include "Allocator.h"
#include "Algorithm.h"
#include "Iterator.h"
#include "UninitializedFunctions.h"
#include "Utility.h"
#include "ReverseIterator.h"


namespace MySTL {
	template <class T, class vector_alloc = Allocator<T>>
	class vector {
	public:
		//vector的嵌套类别定义
		typedef T value_type;
		typedef T* pointer;
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef reverse_iterator_t<T*> reverse_iterator;
		typedef reverse_iterator_t<const T*> const_reverse_iterator;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef vector_alloc data_allocator;

	private:
		iterator start;			//目前使用空间的头
		iterator finish;		//目前使用空间的尾
		iterator end_of_storage;//目前可用空间的尾

	public:
		//一系列构造函数
		vector() : start(0), finish(0), end_of_storage(0) {};
		vector(size_type n, const T& value);//n个value做初值
		//vector(int n, const T& value);
		//vector(long n, const T& value);
		vector(const vector& v);//拷贝构造函数
		template <class InputIterator>
		vector(InputIterator first, InputIterator last);
		explicit vector(size_type n);//explicit防止隐式转换
		~vector();

		iterator begin();
		const_iterator begin()const;
		const_iterator cbegin()const;
		iterator end();
		const_iterator end()const;
		const_iterator cend()const;
		reverse_iterator rbegin();
		const_reverse_iterator crbegin()const;
		reverse_iterator rend();
		const_reverse_iterator crend()const;
		size_type size()const;//大小
		size_type capacity()const;//总容量，区分size()
		bool empty()const;
		reference operator[](size_t n);//取元素，返回引用
		reference front()const;
		reference back()const;
		pointer data();
		void swap(vector& v);
		void push_back(const T& value);
		void pop_back();
		//三种insert
		void insert(iterator position, const T& value);
		void insert(iterator position, size_type n, const T& value);
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);

		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void resize(size_type new_size, const T& value);//少了就删，多了就用value补
		void reserve(size_type new_capacity);
		void clear();

		vector& operator = (const vector& v);
		//vector& operator = (vector&& v);
		bool operator == (const vector& v)const;
		bool operator !=(const vector& v)const;

		template <class T, class vector_alloc>
		friend void swap(vector<T, vector_alloc>& x, vector<T, vector_alloc>& y);

		

	private://为何该方法不是private？
		//填充初始化
		void fillInitialize(size_type n, const T& value);//配置并填充并初始化
		iterator allocateAndFill(size_type n, const T& value);//配置并填充
		//拷贝初始化
		void copyInitialize(const vector& v);
		template <class InputOperator>//可能是别的类型的迭代器指向的对象拷贝到vector中，so要用模板而不能用iterator first
		void allocateAndCopy(InputOperator first, InputOperator last);
		//destroy并deallocate
		void destroyAndDeallocateAll();
		//插入
		void insert_aux(iterator position, const T& value);
		//处理迭代器和数字间区别（用于区分两个构造函数的调用:insert(iterator position, InputIterator first, InputIterator last) && insert(iterator position, Integer n, const value_type& value);
		template <class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template <class Integer>
		void insert_aux(iterator position, Integer n, const value_type& value, std::true_type);
		//处理迭代器和数字间区别（用于区分两个构造函数的调用:vector(size_type n, const T& value) && vector(InputIterator first, InputIterator last);
		template <class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);
		template <class Integer>
		void vector_aux(Integer n, const value_type& value, std::true_type);
	};



	/***********vector(size_type n, const T& value)**************/
	template <class T, class vector_alloc>
	vector<T, vector_alloc>::vector(size_type n, const T& value) {
		fillInitialize(n, value);
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::fillInitialize(size_type n, const T& value) {
		start = allocateAndFill(n, value);
		end_of_storage = finish = start + n;
	}
	template<class T, class vector_alloc>
	typename vector<T, vector_alloc>::iterator vector<T, vector_alloc>::allocateAndFill(size_type n, const T& value) {
		iterator result = data_allocator::allocate(n);
		MySTL::uninitialized_fill_n(result, n, value);//填充，在UninitializedFunction.h中定义
		return result;
	}
	/***********vector(const vector& v)**********************/
	template <class T, class vector_alloc>
	vector<T, vector_alloc>::vector(const vector& v) {
		copyInitialize(v);
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::copyInitialize(const vector& v) {
		allocateAndCopy(v.start, v.finish);
	}
	template <class T, class vector_alloc>
	template <class InputIterator>//注意：两行template不能写在一块
	void vector<T, vector_alloc>::allocateAndCopy(InputIterator first, InputIterator last) {
		start = data_allocator::allocate(last- first);
		end_of_storage = finish = MySTL::uninitialized_copy(first, last, start);
	}






	/***********vector(InputIterator first, InputIterator last)**********************/
	template <class T, class vector_alloc>
	template <class InputIterator>
	vector<T, vector_alloc>::vector(InputIterator first, InputIterator last) {
		//allocateAndCopy(first, last);
		vector_aux(first, last, std::is_integral<InputIterator>::type());
	}
	template <class T, class vector_alloc>
	template <class InputIterator>
	void vector<T, vector_alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type) {
		allocateAndCopy(first, last);
	}
	template <class T, class vector_alloc>
	template <class Integer>
	void vector<T, vector_alloc>::vector_aux(Integer n, const value_type& value, std::true_type) {
		fillInitialize(n, value);
	}
	/***************vector(const size_type n)***************/
	template <class T, class vector_alloc>
	vector<T, vector_alloc>::vector(const size_type n) {
		fillInitialize(n, value_type());
	}
	/***************~vector()*******************************/
	template <class T, class vector_alloc>
	vector<T, vector_alloc>::~vector() {
		destroyAndDeallocateAll();
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::destroyAndDeallocateAll() {
		if (capacity() != 0) {
			data_allocator::destroy(start, finish);
			data_allocator::deallocate(start, capacity());
		}
	}





	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::iterator vector<T, vector_alloc>::begin() {
		return start;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::const_iterator vector<T, vector_alloc>::begin()const {
		return start;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::const_iterator vector<T, vector_alloc>::cbegin()const {
		return start;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::iterator vector<T, vector_alloc>::end() {
		return finish;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::const_iterator vector<T, vector_alloc>::end()const {
		return finish;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::const_iterator vector<T, vector_alloc>::cend()const {
		return finish;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::reverse_iterator vector<T, vector_alloc>::rbegin() {
		return reverse_iterator(finish);
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::const_reverse_iterator vector<T, vector_alloc>::crbegin()const {
		return const_reverse_iterator(finish);
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::reverse_iterator vector<T, vector_alloc>::rend() {
		return reverse_iterator(start);
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::const_reverse_iterator vector<T, vector_alloc>::crend()const {
		return const_reverse_iterator(start);
	}



	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::size_type vector<T, vector_alloc>::size()const {
		return vector<T, vector_alloc>::size_type(finish - start);
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::size_type vector<T, vector_alloc>::capacity()const {
		return vector<T, vector_alloc>::size_type(end_of_storage - start);
	}
	template <class T, class vector_alloc>
	bool vector<T, vector_alloc>::empty()const {
		return start == finish;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::reference vector<T, vector_alloc>::operator[](size_type n) {
		return *(start + n);
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::reference vector<T, vector_alloc>::front()const {
		return *start;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::reference vector<T, vector_alloc>::back()const {
		return *(finish - 1);
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::pointer vector<T, vector_alloc>::data() {
		return start;
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::swap(vector& v) {
		MySTL::swap(start, v.start);
		MySTL::swap(finish, v.finish);
		MySTL::swap(end_of_storage, v.end_of_storage);
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::push_back(const T& value) {
		if (finish != end_of_storage) {//还有空间
			construct(finish, value);
			finish++;
			return;
		}
		insert_aux(end(), value);//member function
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::insert_aux(iterator position, const T& value) {
		//还有备用空间
		if (finish != end_of_storage) {
			construct(finish, *(finish - 1));
			finish++;
			T value_copy = value;
			//注意：有重叠，所以不能用uninitialized_copy前向拷贝，会覆盖，应用后向拷贝函数copy_backward
			std::copy_backward(position, finish - 2, finish - 1);
			*position = value_copy;
			return;
		}
		//已无备用空间
		const size_type old_size = size();
		const size_type len = old_size == 0 ? 1 : 2 * old_size;//原0则配置1，否则扩大到两倍

		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = MySTL::uninitialized_copy(start, position, new_start);
		construct(new_finish, value);
		new_finish = MySTL::uninitialized_copy(position, finish, new_finish + 1);

		destroyAndDeallocateAll();//析构并释放原vector

		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::pop_back() {
		finish--;
		data_allocator::destroy(finish);
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::insert(iterator position, const T& value) {
		//insert_aux(position, value);
		insert(position, 1, value);
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::insert(iterator position, size_type n, const T& value) {
		insert_aux(position, n, value, std::is_integral<size_type>::type());
	}
	template <class T, class vector_alloc>
	template<class InputIterator>
	void vector<T, vector_alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}
	template <class T, class vector_alloc>
	template <class Integer>
	void vector<T, vector_alloc>::insert_aux(iterator position, Integer n, const T& value, std::true_type) {
		if (n <= 0)
			return;

		//备用空间大于等于新增元素个数
		if (end_of_storage - finish >= n) {
			//下面根据“要移动的元素个数”与“插入点之后的元素个数”的大小来决定前向移动还是后向移动（防止覆盖）
			Integer elems_after = finish - position;
			if (n < elems_after) {	//后向移动
				/*
					注意！！！！！！
					有覆盖的地方，要先析构被覆盖区域上的对象！！！！
					覆盖的部分拿出来用std的方法比较靠谱！！！！！
				*/
				MySTL::uninitialized_copy(finish - n, finish, finish);
				std::copy_backward(position, finish - n, finish);//分两次
				std::fill_n(position, n, value);//覆盖部分，用std的靠谱，自己的不会析构原有对象
			}
			else {					//前向移动
				/*
					注意！！！！！！
					有覆盖的地方，要先析构被覆盖区域上的对象！！！！
					覆盖的部分拿出来用std的方法比较靠谱！！！！！
				*/
				//finish += n;
				//uninitialized_copy(position, old_finish, finish - elems_after);
				//uninitialized_fill_n(position, n, value);

				iterator temp = MySTL::uninitialized_fill_n(finish, n - elems_after, value);
				MySTL::uninitialized_copy(position, finish, temp);
				std::fill_n(position, n, value);//覆盖部分，用std的靠谱，自己的不会析构原有对象
			}
			finish += n;
		}
		//备用空间小于新增元素个数
		else {
			Integer old_size = size();
			Integer new_size = old_size += std::max(old_size, n);//旧长度的两倍 或 旧长度+新增元素个数
			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			new_finish = MySTL::uninitialized_copy(start, position, new_start);
			new_finish = MySTL::uninitialized_fill_n(new_finish, n, value);
			new_finish = MySTL::uninitialized_copy(position, finish, new_finish);
			destroyAndDeallocateAll();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + new_size;
		}
	}

	template <class T, class vector_alloc>
	template <class InputIterator>
	void vector<T, vector_alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		size_type n = distance(first, last);//要插入的元素个数，注意要与下面的old_size类型保持一致
		if (n <= 0)
			return;

		//备用空间大于等于新增元素个数
		if (end_of_storage - finish >= n) {
			//下面根据“要移动的元素个数”与“插入点之后的元素个数”的大小来决定前向移动还是后向移动（防止覆盖）
			size_type elems_after = finish - position;
			if (n < elems_after) {	//后向移动
				/*
					注意！！！！！！
					有覆盖的地方，要先析构被覆盖区域上的对象！！！！
					覆盖的部分拿出来用std的方法比较靠谱！！！！！
				*/
				uninitialized_copy(finish - n, finish, finish);
				std::copy_backward(position, finish - n, finish);
				std::copy(first, last, position);//覆盖部分，用std的靠谱，自己的不会析构原有对象
			}
			else {					//前向移动
				/*
					注意！！！！！！
					有覆盖的地方，要先析构被覆盖区域上的对象！！！！
					覆盖的部分拿出来用std的方法比较靠谱！！！！！
				*/
				//uninitialized_copy(position, old_finish, finish - elems_after);
				//uninitialized_copy(first, last, position);

				iterator temp = uninitialized_copy(first + (finish - position), last, finish);
				uninitialized_copy(position, finish, temp);
				std::copy(first, first + (finish - position), position);//覆盖部分，用std的靠谱，自己的不会析构原有对象
			}
			finish += n;
		}
		//备用空间小于新增元素个数
		else {
			size_type old_size = size();
			size_type new_size = old_size = old_size + std::max(old_size, n);//旧长度的两倍 或 旧长度+新增元素个数
			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			new_finish = uninitialized_copy(start, position, new_start);
			new_finish = uninitialized_copy(first, last, new_finish);
			new_finish = uninitialized_copy(position, finish, new_finish);
			destroyAndDeallocateAll();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + new_size;
		}
	}



	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::iterator vector<T, vector_alloc>::erase(iterator position) {
		std::copy(position + 1, finish, position);
		finish--;
		data_allocator::destroy(finish);
		return position;
	}
	template <class T, class vector_alloc>
	typename vector<T, vector_alloc>::iterator vector<T, vector_alloc>::erase(iterator first, iterator last) {
		iterator temp = std::copy(last, finish, first);
		data_allocator::destroy(temp, finish);
		finish = finish - (last - first);
		return first;
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::resize(size_type new_size, const T& value) {
		if (new_size < size())
			erase(start + new_size, end());
		else
			insert(finish, new_size - size(), value);
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::reserve(size_type new_capacity) {
		if (capacity() >= new_capacity)
			return;
		iterator new_start = data_allocator::allocate(new_capacity);
		iterator new_finish = uninitialized_copy(start, finish, new_start);
		destroyAndDeallocateAll();
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + new_capacity;
	}
	template <class T, class vector_alloc>
	void vector<T, vector_alloc>::clear() {
		erase(start, finish);
	}

	template<class T, class vector_alloc>
	vector<T, vector_alloc>& vector<T, vector_alloc>::operator = (const vector& v){
		if (this != &v)
			allocateAndCopy(v.start, v.finish);
		return *this;
	}

	//template<class T, class vector_alloc>
	//vector<T, vector_alloc>& vector<T, vector_alloc>::operator = (vector&& v){
	//	if (this != &v){
	//		destroyAndDeallocateAll();
	//		start = v.start;
	//		finish = v.finish;
	//		end_of_storage = v.end_of_storage;
	//		v.start = v.finish = v.end_of_storage = 0;
	//	}
	//	return *this;
	//}

	template <class T, class vector_alloc>
	bool vector<T, vector_alloc>::operator == (const vector& v)const {
		if (size() != v.size())
			return false;
		iterator temp_start = start;
		iterator temp_v_start = v.start;
		for (; temp_start != finish && temp_v_start != v.end(); temp_start++, temp_v_start++)
			if (*temp_start != *temp_v_start)
				return false;
		return true;
	}
	template <class T, class vector_alloc>
	bool vector<T, vector_alloc>::operator != (const vector& v)const {
		return !(*this == v);
	}


	template <class T, class vector_alloc>
	void swap(vector<T, vector_alloc>& x, vector<T, vector_alloc>& y) {
		x.swap(y);
	}


}

#endif