#ifndef SLISTTEST_H
#define SLISTTEST_H

#include "TestUtil.h"
#include "Slist.h"

#include <string>
#include <cassert>
#include <list>

namespace MySTL {
	namespace SlistTest {
		template <class T>
		using msSL = MySTL::slist < T > ;
		template <class T>
		using stdL = std::list < T > ;

		void testCase1();
		void testCase2();
		void testCase3();

		void testAllCases();
	}
}

#endif