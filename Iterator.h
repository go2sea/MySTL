#ifndef ITERATOR_H
#define ITERATOR_H

namespace MySTL {
	//����tag
	struct input_iterator_tag {};//ֻ����read only��
	struct output_iterator_tag {};//ֻд��write only��
	//�����д����ֻ����ǰ���ƶ�
	struct forward_iterator_tag : public input_iterator_tag {};
	//�����д��������˫���ƶ�
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	//�����д�����������ƶ�
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};



	////���ֲ�ͬ������������
	//template <class T, class Distance>
	//struct input_iterator {
	//	typedef input_iterator_tag	iterator_category;//��ʾ����������������
	//	typedef T					value_type;//��ʾ��������ָ���ݵ�����
	//	typedef Distance			difference_type;//��ʾ����������֮��ľ�������
	//	typedef T*					pointer;//��ʾ��������ָ���ݵ�ָ������
	//	typedef T&					reference; //��ʾ��������ָ���ݵ���������
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


	//������iterator�ķ���
	template <class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};
	//��ָ�����͵�ƫ�ػ�
	template <class T>
	struct iterator_traits < T* > {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};
	//�Գ���ָ�����͵�ƫ�ػ�
	template <class T>
	struct iterator_traits < const T* > {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};


	//��ȡ����������
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator& It) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();//����ɶ��˼��--->����һ����ʱ���� ���P96
	}
	//��ȡ��������ָ�������͡�������ָ�룿
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
	//��ȡ��������ֵ���͡�������ָ�룿
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::deference_type*>(0);
	}



}


#endif