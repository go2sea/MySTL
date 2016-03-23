#include "DequeTest.h"

namespace MySTL {
	namespace DequeTest{
		void testCase1(){
			std::cout << "DequeTest testCase1 !!!" << std::endl;

			stdDQ<int> dq1(10, 0);
			msDQ<int> dq2(10, 0);
			assert(MySTL::Test::container_equal(dq1, dq2));

			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			stdDQ<int> dq3(std::begin(arr), std::end(arr));
			msDQ<int> dq4(std::begin(arr), std::end(arr));
			assert(MySTL::Test::container_equal(dq3, dq4));

			auto dq5(dq1);
			auto dq6(dq2);
			assert(MySTL::Test::container_equal(dq5, dq6));

			auto dq7 = dq3;
			auto dq8 = dq4;
			assert(MySTL::Test::container_equal(dq7, dq8));

			auto dq9 = std::move(dq7);
			auto dq10 = std::move(dq8);
			assert(MySTL::Test::container_equal(dq9, dq10));
		}
		void testCase2(){
			std::cout << "DequeTest testCase2 !!!" << std::endl;

			msDQ<int> dq1;
			stdDQ<int> dq11;
			assert(dq1.empty());
			dq1.push_back(1);
			dq1.pop_back();
			assert(dq1.size() == 0);

			msDQ<int> dq2(10, 0);
			assert(!dq2.empty());
			assert(dq2.size() == 10);
		}
		void testCase3(){
			std::cout << "DequeTest testCase3 !!!" << std::endl;

			stdDQ<std::string> dq1(10, "10");
			msDQ<std::string> dq2(10, "10");

			dq1[0] = "0"; dq1[9] = "9";
			dq2[0] = "0"; dq2[9] = "9";

			assert(dq1.front() == dq2.front());
			assert(dq1.back() == dq2.back());
		}
		void testCase4(){
			std::cout << "DequeTest testCase4 !!!" << std::endl;

			stdDQ<int> dq1;
			msDQ<int> dq2;

			for (auto i = 0; i != 10; ++i){
				dq1.push_back(i);
				dq2.push_back(i);
			}
			assert(MySTL::Test::container_equal(dq1, dq2));

			for (auto i = 10; i != 20; ++i){
				dq1.push_front(i);
				dq2.push_front(i);
			}
			assert(MySTL::Test::container_equal(dq1, dq2));

			for (auto i = 0; i != 5; ++i){
				dq1.pop_back();
				dq2.pop_back();
			}
			assert(MySTL::Test::container_equal(dq1, dq2));

			for (auto i = 0; i != 5; ++i){
				dq1.pop_front();
				dq2.pop_front();
			}
			assert(MySTL::Test::container_equal(dq1, dq2));
		}
		void testCase5(){
			std::cout << "DequeTest testCase5 !!!" << std::endl;

			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			msDQ<int> foo(arr, arr + 3), bar(arr + 3, arr + 10);

			assert(foo.size() == 3 && bar.size() == 7);
			foo.swap(bar);
			assert(foo.size() == 7 && bar.size() == 3);
			MySTL::swap(foo, bar);
			assert(foo.size() == 3 && bar.size() == 7);
		}
		void testCase6(){
			std::cout << "DequeTest testCase6 !!!" << std::endl;

			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			msDQ<int> foo1(arr, arr + 3), bar(arr + 3, arr + 10);

			assert(foo1 != bar);
			auto foo2 = bar;
			assert(foo2 == bar);
		}


		void testAllCases(){
			testCase1();
			testCase2();
			testCase3();
			testCase4();
			testCase5();
			testCase6();
		}
	}
}