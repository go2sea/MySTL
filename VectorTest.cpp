#include <assert.h>

#include "VectorTest.h"
#include "TestUtil.h"


namespace MySTL {
	namespace VectorTest {
		void testCase1() {
			std::cout << "VectorTest testCase1 !!!" << std::endl;

			stdVec<std::string> v1(10, "zxh");
			msVec<std::string> v2(10, "zxh");
			assert(MySTL::Test::container_equal(v1, v2));

			stdVec<std::string> v3(10);
			msVec<std::string> v4(10);
			assert(MySTL::Test::container_equal(v3, v4));

			std::array<std::string, 3> arr = { "abc", "def", "ghi" };
			stdVec<std::string> v5(std::begin(arr), std::end(arr));
			msVec<std::string> v6(std::begin(arr), std::end(arr));
			assert(MySTL::Test::container_equal(v5, v6));
		}
		void testCase2(){
			std::cout << "VectorTest testCase2 !!!" << std::endl;

			stdVec<int> temp1(10, 0);
			msVec<int> temp2(10, 0);

			auto v1(temp1);
			auto v2(temp2);
			assert(MySTL::Test::container_equal(v1, v2));

			auto v3(std::move(temp1));
			auto v4(std::move(temp2));
			assert(MySTL::Test::container_equal(v3, v4));

			auto v5 = v1;
			auto v6 = v2;
			assert(MySTL::Test::container_equal(v5, v6));

			auto v7 = std::move(v3);
			auto v8 = std::move(v4);
			assert(MySTL::Test::container_equal(v7, v8));
		}
		void testCase3(){
			std::cout << "VectorTest testCase3 !!!" << std::endl;

			msVec<int> v1, v2;
			for (int i = 0; i != 100; ++i){
				v1.push_back(i);
				v2.push_back(i);
			}
			assert(v1 == v2);
			assert(!(v1 != v2));
		}
		void testCase4(){
			std::cout << "VectorTest testCase4 !!!" << std::endl;

			msVec<int> myvector;
			for (int i = 1; i <= 5; i++) myvector.push_back(i);

			auto i = 1;
			for (msVec<int>::iterator it = myvector.begin(); it != myvector.end(); ++it, ++i){
				assert(*it == i);
			}

			i = 1;
			for (msVec<int>::const_iterator it = myvector.cbegin(); it != myvector.cend(); ++it, ++i){
				assert(*it == i);
			}
		}
		void testCase5(){
			std::cout << "VectorTest testCase5 !!!" << std::endl;

			msVec<int> myvector(5);  // 5 default-constructed ints
			int i = 0;
			msVec<int>::reverse_iterator rit = myvector.rbegin();
			for (; rit != myvector.rend(); ++rit)
				*rit = ++i;

			i = 5;
			for (msVec<int>::iterator it = myvector.begin(); it != myvector.end(); ++it, --i){
				assert(*it == i);
			}

			i = 1;
			for (msVec<int>::reverse_iterator it = myvector.rbegin(); it != myvector.rend(); ++it, ++i){
				assert(*it == i);
			}
		}
		void testCase6(){
			std::cout << "VectorTest testCase6 !!!" << std::endl;

			msVec<int> v(11, 0);
			assert(v.size() == 11);

			v.resize(5, 0);
			assert(v.size() == 5);

			v.resize(20, 0);
			assert(v.size() == 20);
		}
		void testCase7(){
			std::cout << "VectorTest testCase7 !!!" << std::endl;

			msVec<int> v;
			v.reserve(20);
			assert(v.capacity() == 20);
		}
		void testCase8(){
			std::cout << "VectorTest testCase8 !!!" << std::endl;

			stdVec<int> v1(10);
			msVec<int> v2(10);
			for (unsigned i = 0; i < 10; i++){
				v1[i] = i;
				v2[i] = i;
			}
			assert(MySTL::Test::container_equal(v1, v2));

			v1.front() = 99;
			v2.front() = 99;
			v1.back() = 100;
			v2.back() = 100;

			assert(MySTL::Test::container_equal(v1, v2));
		}
		void testCase9(){
			std::cout << "VectorTest testCase9 !!!" << std::endl;

			stdVec<int> v1(5);
			msVec<int> v2(5);

			auto p1 = v1.data();
			auto p2 = v2.data();
			*p1 = 10; ++p1; *p1 = 20; p1[2] = 100;
			*p2 = 10; ++p2; *p2 = 20; p2[2] = 100;

			assert(MySTL::Test::container_equal(v1, v2));
		}
		void testCase10(){
			std::cout << "VectorTest testCase10 !!!" << std::endl;

			msVec<int> foo(3, 100);   // three ints with a value of 100
			msVec<int> bar(2, 200);   // five ints with a value of 200

			assert(MySTL::Test::container_equal(foo, stdVec < int > { 100, 100, 100 }));
			assert(MySTL::Test::container_equal(bar, stdVec < int > {200, 200}));

			foo.swap(bar);
			assert(MySTL::Test::container_equal(bar, stdVec < int > { 100, 100, 100 }));
			assert(MySTL::Test::container_equal(foo, stdVec < int > {200, 200}));
		}
		void testCase11(){
			std::cout << "VectorTest testCase11 !!!" << std::endl;

			stdVec<std::string> v1;
			msVec<std::string> v2;

			v1.push_back("hello "); v1.push_back("world");
			v2.push_back("hello "); v2.push_back("world");
			assert(MySTL::Test::container_equal(v1, v2));

			v1.pop_back();
			v2.pop_back();
			assert(MySTL::Test::container_equal(v1, v2));
		}
		void testCase12(){
			std::cout << "VectorTest testCase12 !!!" << std::endl;

			stdVec<int> v1;
			msVec<int> v2;

			v1.insert(v1.begin(), 0);
			std::cout << "VectorTest testCase12 !!!" << std::endl;

			v2.insert(v2.begin(), 0);
			std::cout << "VectorTest testCase12 !!!" << std::endl;

			assert(MySTL::Test::container_equal(v1, v2));

			v1.insert(v1.end(), 1);
			v2.insert(v2.end(), 1);
			assert(MySTL::Test::container_equal(v1, v2));

			v1.insert(v1.begin() + v1.size() / 2, 10, 0);
			v2.insert(v2.begin() + v2.size() / 2, 10, 0);
			assert(MySTL::Test::container_equal(v1, v2));

			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			v1.insert(v1.end(), std::begin(arr), std::end(arr));
			v2.insert(v2.end(), std::begin(arr), std::end(arr));
			assert(MySTL::Test::container_equal(v1, v2));
		}
		void testCase13(){
			std::cout << "VectorTest testCase13 !!!" << std::endl;

			stdVec<int> v1;
			msVec<int> v2;
			for (int i = 1; i <= 10; i++) {
				v1.push_back(i);
				v2.push_back(i);
			}
			v1.erase(v1.begin() + 5);
			v2.erase(v2.begin() + 5);
			assert(MySTL::Test::container_equal(v1, v2));

			v1.erase(v1.begin(), v1.begin() + 3);
			v2.erase(v2.begin(), v2.begin() + 3);
			assert(MySTL::Test::container_equal(v1, v2));
		}
		void testCase14(){
			std::cout << "VectorTest testCase14 !!!" << std::endl;

			msVec<int> foo(3, 100);
			msVec<int> bar(2, 200);

			assert(!(foo == bar));
			assert(foo != bar);
		}

		class TestItem {
		public:
			TestItem() { ++count; }
			TestItem(const TestItem & other) { ++count; }
			virtual ~TestItem() { --count;  }
			static int getCount() { return count; }
		private:
			static int count;
		};
		int TestItem::count = 0;

		void testCase15()
		{
			std::cout << "VectorTest testCase15 !!!" << std::endl;

			assert(TestItem::getCount() == 0);
			{
				typedef MySTL::vector<TestItem> TVector;
				TVector t(10);
				t.push_back(TestItem());
				t.push_back(TestItem());
				t.push_back(TestItem());

				t.insert(t.begin(), t.begin(), t.begin() + 1);
				t.insert(t.begin(), t.begin(), t.end());

				t.insert(t.begin(), 1, *(t.begin()));
				t.insert(t.begin(), 100, *(t.begin()));
			}
			assert(TestItem::getCount() == 0);
		}
		void testAllCases(){
			testCase1();
			testCase2();
			testCase3();
			testCase4();
			testCase5();
			testCase6();
			testCase7();
			testCase8();
			testCase9();
			testCase10();
			testCase11();
			testCase12();
			testCase13();
			testCase14();
			testCase15();
		}

	}
}