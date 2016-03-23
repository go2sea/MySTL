#include "AlgorithmTest.h"

namespace MySTL {
	namespace AlgorithmTest {
		void testHeapAlgorithm(){
			std::cout << "AlgorithmTest testHeapAlgorithm !!!" << std::endl;
			
			int myints[] = { 10, 20, 30, 5, 15 };
			std::vector<int> v1(myints, myints + 5);
			std::vector<int> v2(myints, myints + 5);
			std::make_heap(v1.begin(), v1.end());
			MySTL::make_heap(v2.begin(), v2.end());
			assert(MySTL::Test::container_equal(v1, v2));

			std::pop_heap(v1.begin(), v1.end()); v1.pop_back();
			MySTL::pop_heap(v2.begin(), v2.end()); v2.pop_back();
			assert(MySTL::Test::container_equal(v1, v2));

			v1.push_back(99); std::push_heap(v1.begin(), v1.end());
			v2.push_back(99); MySTL::push_heap(v2.begin(), v2.end());
			assert(MySTL::Test::container_equal(v1, v2));

			std::sort_heap(v1.begin(), v1.end());
			MySTL::sort_heap(v2.begin(), v2.end());
			assert(MySTL::Test::container_equal(v1, v2));
		}
		void testIsHeap(){
			std::cout << "AlgorithmTest testIsHeap !!!" << std::endl;

			std::vector<int> v1{ 9, 5, 2, 6, 4, 1, 3, 8, 7 };
			std::vector<int> v2{ 9, 5, 2, 6, 4, 1, 3, 8, 7 };

			if (!std::is_heap(v1.begin(), v1.end()))
				std::make_heap(v1.begin(), v1.end());
			if (!MySTL::is_heap(v2.begin(), v2.end()))
				MySTL::make_heap(v2.begin(), v2.end());
			assert(MySTL::Test::container_equal(v1, v2));
		}
		
		void testAllCases() {
			testHeapAlgorithm();
			testIsHeap();
		}
	}
}