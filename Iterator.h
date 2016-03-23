#ifndef ITERATOR_H
#define ITERATOR_H

namespace MySTL {
	//几种tag
	struct input_iterator_tag {};//只读（read only）
	struct output_iterator_tag {};//只写（write only）
	//允许读写，但只能做前向移动
	struct forward_iterator_tag : public input_iterator_tag {};
	//允许读写，可以做双向移动
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	//允许读写，可以任意移动
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};



	////各种不同迭代器的特性
	//template <class T, class Distance>
	//struct input_iterator {
	//	typedef input_iterator_tag	iterator_category;//表示迭代器所属的类型
	//	typedef T					value_type;//表示迭代器所指数据的类型
	//	typedef Distance			difference_type;//表示两个迭代器之间的距离类型
	//	typedef T*					pointer;//表示迭代器所指数据的指针类型
	//	typedef T&					reference; //表示迭代器所指数据的引用类型
	//};
	//template <class T, class Distance>
	//struct output_iterator {
	//	typedef output_iterator_tag	iterator_category;
	//	typedef void				value_type;
	//	typedef void				difference_type;
	//	typedef void				pointer;
	//	typedef void				reference;
	//};
	//template <class T, class Distance>
	//struct forward_iterator {
	//	typedef forward_iterator_tag	iterator_category;
	//	typedef T						value_type;
	//	typedef Distance				difference_type;
	//	typedef T*						pointer;
	//	typedef T&						reference; 
	//};
	//template <class T, class Distance>
	//struct bidirectional_iterator {
	//	typedef bidirectional_iterator_tag	iterator_category;
	//	typedef T							value_type;
	//	typedef Distance					difference_type;
	//	typedef T*							pointer;
	//	typedef T&							reference;
	//};
	//template <class T, class Distance>
	//struct random_access_iterator {
	//	typedef random_access_iterator_tag	iterator_category;
	//	typedef T							value_type;
	//	typedef Distance					difference_type;
	//	typedef T*							pointer;
	//	typedef T&							reference;
	//};

	////what is this???
	//template <class Category, class T, class Distance = ptrdiff_t,
	//class Pointer = T*, class Reference = T&>
	//struct iterator {
	//	typedef Category	iterator_category;
	//	typedef T			value_type;
	//	typedef Distance	difference_type;
	//	typedef Reference	reference;
	//};


	//对所有iterator的泛化
	template <class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};
	//对指针类型的偏特化
	template <class T>
	struct iterator_traits < T* > {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};
	//对常量指针类型的偏特化
	template <class T>
	struct iterator_traits < const T* > {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};


	//获取迭代器类型
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator& It) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();//（）啥意思？--->产生一个暂时对象 侯捷P96
	}
	//获取迭代器所指数据类型。。。的指针？
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
	//获取迭代器差值类型。。。的指针？
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::deference_type*>(0);
	}



}


#endif