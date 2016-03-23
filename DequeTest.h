#ifndef DEQUETEST_H
#define DEQUETEST_H

#include <deque>
#include "Deque.h"
#include "TestUtil.h"
#include <assert.h>
#include <string>

namespace MySTL {
	namespace DequeTest {
		template <class T>
		using stdDQ = std::deque < T > ;
		template <class T>
		using msDQ = MySTL::deque < T > ;

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();
		void testCase6();

		void testAllCases();
	}
}



#endif