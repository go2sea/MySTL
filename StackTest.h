#ifndef	STACKTEST_H
#define STACKTEST_H

#include "TestUtil.h"
#include "Stack.h"

#include <stack>
#include <cassert>
#include <string>


namespace MySTL {
	namespace StackTest {
		template <class T>
		using stdSt = std::stack < T > ;
		template <class T>
		using msSt = MySTL::stack < T > ;

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();

		void testAllCases();
	}
}

#endif