#ifndef	UTILITY_H
#define UTILITY_H

namespace MySTL {

	template <class T>
	void swap(T& a, T&b) {
		T temp = a;	//�Ƿ��п������캯����
		a = b;		//�Ƿ������ظ�ֵ��������
		b = temp;	//�Ƿ������ظ�ֵ��������
	}
}



#endif