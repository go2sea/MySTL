#ifndef	UTILITY_H
#define UTILITY_H

namespace MySTL {

	template <class T>
	void swap(T& a, T&b) {
		T temp = a;	//是否有拷贝构造函数？
		a = b;		//是否有重载赋值操作符？
		b = temp;	//是否有重载赋值操作符？
	}
}



#endif