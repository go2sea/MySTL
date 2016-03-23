#ifndef QUEUETEST_H
#define QUEUETEST_H

#include "TestUtil.h"
#include "Queue.h"

#include <cassert>
#include <string>
#include <Queue>

namespace MySTL {
	namespace QueueTest {
		template <class T>
		using stdQ = std::queue < T > ;
		template <class T>
		using msQ = MySTL::queue < T > ;

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();

		void testAllCases();
	}
}


#endif