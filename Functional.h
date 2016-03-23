#ifndef	FUNCTIONAL_H
#define FUNCTIONAL_H

namespace MySTL {
	/*
	提供各种功能函数
	*/

	template <class T>
	struct less_struct {
		bool operator()(T& a, T&b) {
			return a < b;
		}
	};

	template <class T>
	struct equal_struct {
		bool operator()(T&a, T& b) {
			return a == b;
		}
	};

}
#endif