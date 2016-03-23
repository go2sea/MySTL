#ifndef ALLOCATOR_H
#define	ALLOCATOR_H

#include "Alloc.h"

namespace MySTL {
	/*
		�ռ����������Զ���Ϊ��λ����
	*/
	template <class T>
	class Allocator {
	public:
		static T* allocate();
		static T* allocate(size_t n);//����n�����������ڴ�
		static void deallocate(T* ptr);
		static void deallocate(T* ptr, size_t n);

		static void construct(T* ptr);
		static void construct(T* ptr, T& value);
		static void destroy(T* ptr);
		static void destroy(T* begin, T* end);
	};


	template <class T>
	T* Allocator<T>::allocate() {
		return static_cast<T*>(Alloc::allocate(sizeof(T)));
	}

	template <class T>
	T* Allocator<T>::allocate(size_t n) {
		if (n == 0)
			return 0;
		return static_cast<T*>(Alloc::allocate(sizeof(T)*n));
	}

	template <class T>
	void Allocator<T>::deallocate(T* ptr) {
		Alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
	}

	template <class T>
	void Allocator<T>::deallocate(T* ptr, size_t n) {
		Alloc::deallocate(static_cast<void*>(ptr), sizeof(T)*n);
	}

	template <class T>
	void Allocator<T>::construct(T* ptr) {
		//placement new
		new(ptr)T();//���ض���ַ��������
	}

	template <class T>
	void Allocator<T>::construct(T* ptr, T& value) {
		//std::cout << 666 << std::endl;
		new(ptr)T(value);
	}

	template <class T>
	void Allocator<T>::destroy(T* ptr) {
		ptr->~T();
	}

	template <class T>
	void Allocator<T>::destroy(T* begin, T* end) {
		while (begin != end) {
			begin->~T();
			begin++;
			////std::cout << 7;
		}

	}
}

#endif