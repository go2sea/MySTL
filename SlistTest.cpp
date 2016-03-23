#include "SlistTest.h"
#include "TestUtil.h"
#include <algorithm>

namespace MySTL {
	namespace SlistTest {
		void testCase1() {
			std::cout << "SlistTest testCase1 !!!" << std::endl;
			msSL<int> l1;
			assert(l1.size() == 0);
			for (int i = 0; i < 5; i++)
				l1.push_front(i);
			assert(l1.size() == 5);

			msSL<int>::iterator cur = l1.begin();
			while (*cur != 2)
				cur++;
			assert(*cur == 2);
			
			cur = l1.insert(cur, 99);
			assert(*cur == 99);

			cur = l1.erase(cur);
			assert(*cur == 2);
		}
		void testCase2() {
			std::cout << "SlistTest testCase2 !!!" << std::endl;
			msSL<int> l2;
			for (int i = 0; i < 5; i++)
				l2.push_front(i);
			for (int i = 0; i < 11;i++)
				l2.reverse();

			auto ite = l2.begin();
			for (int i = 0; i < 5; i++)
				assert(*ite++ == i);
		}

		void testCase3() {
			std::cout << "SlistTest testCase3 !!!" << std::endl;

			msSL<int> l3, l4;
			stdL<int> l5;
			for (int i = 0; i < 5; i++)
				l3.push_front(i);
			for (int i = 0; i < 5; i++)
				l5.push_front(i);
			for (int i = 0; i < 2; i++)
				l4.push_front(i);
			l4 = l3;
			l3.pop_front();
			assert(l4.size() == 5 && l3.size() == 4);
			l4.insert_after(l4.begin(), l3.begin(), l3.end());
			//MySTL::Test::print_container(l4, "l4");
			l4.insert_after(l4.begin(), 99);
			//MySTL::Test::print_container(l4, "l4");
			l4.insert_after(l4.begin(), 3, 3);
			//MySTL::Test::print_container(l4, "l4");

			l4.insert(l4.begin(), l3.begin(), l3.end());
			//MySTL::Test::print_container(l4, "l4");
			l4.insert(l4.begin(), 99);
			//MySTL::Test::print_container(l4, "l4");
			l4.insert(l4.begin(), 3, 3);
			//MySTL::Test::print_container(l4, "l4");

		}


		void testAllCases() {
			testCase1();
			testCase2();
			testCase3();
		}
	}

}