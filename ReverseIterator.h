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
		/**************����***************/
		reverse_iterator_t() : base_(0), cur_(0) {};
		explicit reverse_iterator_t(const iterator_type& it) : base_(it) {
			auto temp = it;
			cur_ = --temp;
		}
		//�������캯��
		template <class Iter>
		reverse_iterator_t(const reverse_iterator_t<Iter>& value) {
			base_ = (iterator_type)value.base_;
			auto temp = base_;
			cur_ = --temp;
		}
		

		iterator_type base() { return base_; }
		reference operator*() { return *cur_; }
		const_reference operator*()const { return *cur_; }
		pointer operator->() { return &(operator*()); }//ע��
		const_pointer operator->()const { return &(operator*()); }
		//ǰ�ã���������
		reverse_iterator_t& operator++() {
			--base_;
			--cur_;
			return *this;
		}
		//���ã�����ֵ
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

		//��Ԫ����������
		//ע�������������������Ϊ���ǳ�Ա������
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
		//random_access_iterator_tag����Խʽǰ��
		Iterator advanceNStep(Iterator it, difference_type n, bool right, random_access_iterator_tag) {
			//right��true-���� false-����
			difference_type flag = right ? 1 : -1;
			it += flag*n;
			return it;
		}
		//bidirectional_iterator_tag��һ��һ����
		Iterator advanceNStep(Iterator it, difference_type n, bool right, bidirectional_iterator_tag) {
			//right��true-���� false-����
			difference_type direction = right ? 1 : -1;
			difference_type absN = n >= 0 ? n : -n;
			difference_type  step = n >= 0 ? direction : -direction;
			for (i = 0; i <= absN, i++)
				it = it + step;
		}
	};//end of reverse_iterator_t


	//���ֳ�Ա��������
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



	//��Ԫ��������
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