#ifndef PRIORITYQUEUETEST_H
#define PRIORITYQUEUETEST_H

#include "TestUtil.h"
#include "PriorityQueue.h"

#include <queue>
#include <algorithm>
#include <cassert>
#include <string>


namespace MySTL {
	namespace PriorityQueueTest {
		template <class T>
		using stdPQ = std::priority_queue < T > ;
		template <class T>
		using msPQ = MySTL::priority_queue < T > ;

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();

		void testAllCases();
	}
}

#endif