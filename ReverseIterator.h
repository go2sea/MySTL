#ifndef REVERSEITERATOR_H
#define REVERSEITERATOR_H

#include  "Iterator.h"

namespace MySTL {
	template <class Iterator>
	class reverse_iterator_t {
	public:
		typedef Iterator iterator_type;
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef const pointer const_pointer;
		typedef typename iterator_traits<Iterator>::reference reference;
		typedef const reference const_reference;

	private:
		Iterator base_;
		Iterator cur_;





	public:
		/**************构造***************/
		reverse_iterator_t() : base_(0), cur_(0) {};
		explicit reverse_iterator_t(const iterator_type& it) : base_(it) {
			auto temp = it;
			cur_ = --temp;
		}
		//拷贝构造函数
		template <class Iter>
		reverse_iterator_t(const reverse_iterator_t<Iter>& value) {
			base_ = (iterator_type)value.base_;
			auto temp = base_;
			cur_ = --temp;
		}
		

		iterator_type base() { return base_; }
		reference operator*() { return *cur_; }
		const_reference operator*()const { return *cur_; }
		pointer operator->() { return &(operator*()); }//注意
		const_pointer operator->()const { return &(operator*()); }
		//前置，返回引用
		reverse_iterator_t& operator++() {
			--base_;
			--cur_;
			return *this;
		}
		//后置，返回值
		reverse_iterator_t operator++(int) {
			reverse_iterator_t temp = *this;
			++(*this);
			return temp;
		}
		reverse_iterator_t& operator--() {
			++base_;
			++cur_;
			return *this;
		}
		reverse_iterator_t operator--(int) {
			reverse_iterator_t temp = *this;
			--(*this);
			return temp;
		}
		reference operator[] (difference_type n) {
			return base()[-n - 1];
		}
		reverse_iterator_t operator + (difference_type n)const;
		reverse_iterator_t& operator += (difference_type n);
		reverse_iterator_t operator - (difference_type n)const;
		reverse_iterator_t& operator -= (difference_type n);

		//友元函数的声明
		//注意参数个数（两个，因为不是成员函数）
		template <class Iterator>
		friend bool operator == (const reverse_iterator_t<Iterator>& left,
			const reverse_iterator_t<Iterator>& right);
		template <class Iterator>
		friend bool operator != (const reverse_iterator_t<Iterator>& left,
			const reverse_iterator_t<Iterator>& right);
		template <class Iterator>
		friend bool operator < (const reverse_iterator_t<Iterator>& left,
			const reverse_iterator_t<Iterator>& right);
		template <class Iterator>
		friend bool operator <= (const reverse_iterator_t<Iterator>& left,
			const reverse_iterator_t<Iterator>& right);
		template <class Iterator>
		friend bool operator > (const reverse_iterator_t<Iterator>& left,
			const reverse_iterator_t<Iterator>& right);
		template <class Iterator>
		friend bool operator >= (const reverse_iterator_t<Iterator>& left,
			const reverse_iterator_t<Iterator>& right);


	private:
		//random_access_iterator_tag：跨越式前进
		Iterator advanceNStep(Iterator it, difference_type n, bool right, random_access_iterator_tag) {
			//right：true-正向 false-反向
			difference_type flag = right ? 1 : -1;
			it += flag*n;
			return it;
		}
		//bidirectional_iterator_tag：一步一步来
		Iterator advanceNStep(Iterator it, difference_type n, bool right, bidirectional_iterator_tag) {
			//right：true-正向 false-反向
			difference_type direction = right ? 1 : -1;
			difference_type absN = n >= 0 ? n : -n;
			difference_type  step = n >= 0 ? direction : -direction;
			for (i = 0; i <= absN, i++)
				it = it + step;
		}
	};//end of reverse_iterator_t


	//部分成员函数定义
	template <class Iterator>
	reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator += (difference_type n) {
		base_ = advanceNStep(base_, n, false, iterator_category());
		cur_ = advanceNStep(cur_, n, false, iterator_category());
		return *this;
	}
	template <class Iterator>
	reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator -= (difference_type n) {
		base_ = advanceNStep(base_, n, true, iterator_category());
		cur_ = advanceNStep(cur_, n, true, iterator_category());
		return *this;
	}
	template <class Iterator>
	reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator + (difference_type n)const {
		reverse_iterator_t result = *this;
		result += n;
		return result;
	}
	template <class Iterator>
	reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator - (difference_type n)const {
		reverse_iterator_t result = *this;
		result -= n;
		return result;
	}



	//友元函数定义
	template <class Iterator>
	bool operator == (const reverse_iterator_t<Iterator>& left, const reverse_iterator_t<Iterator>& right) {
		return left.cur_ == right.cur_;
	}
	template <class Iterator>
	bool operator != (const reverse_iterator_t<Iterator>& left, const reverse_iterator_t<Iterator>& right) {
		return !(left == right);
	}
	template <class Iterator>
	bool operator < (const reverse_iterator_t<Iterator>& left, const reverse_iterator_t<Iterator>& right){
		return left.cur_ < right.cur_;
	}
	template <class Iterator>
	bool operator >(const reverse_iterator_t<Iterator>& left, const reverse_iterator_t<Iterator>& right){
		return left.cur_ > right.cur_;
	}
	template <class Iterator>
	bool operator >= (const reverse_iterator_t<Iterator>& left, const reverse_iterator_t<Iterator>& right){
		return !(left < right);
	}
	template <class Iterator>
	bool operator <= (const reverse_iterator_t<Iterator>& left, const reverse_iterator_t<Iterator>& right){
		return !(left > right);
	}
	template <class Iterator>
	reverse_iterator_t<Iterator> operator + (
		typename reverse_iterator_t<Iterator>::difference_type n,
		const reverse_iterator_t<Iterator>& rev_it) {
		return rev_it + n;
	}
	template <class Iterator>
	typename reverse_iterator_t<Iterator>::difference_type operator - (
		const reverse_iterator_t<Iterator>& left,
		const reverse_iterator_t<Iterator>& right) {
		return ;
	}
}



#endif