#include <iostream>

#include "VectorTest.h"
#include "ListTest.h"
#include "StackTest.h"
#include "QueueTest.h"
#include "AlgorithmTest.h"
#include "PriorityQueueTest.h"
#include "DequeTest.h"
#include "SlistTest.h"

using namespace std;

void main() {
	MySTL::VectorTest::testAllCases();
	MySTL::ListTest::testAllCases();
	MySTL::StackTest::testAllCases();
	MySTL::QueueTest::testAllCases();
	MySTL::AlgorithmTest::testAllCases();
	MySTL::PriorityQueueTest::testAllCases();
	MySTL::DequeTest::testAllCases();
	MySTL::SlistTest::testAllCases();




	system("pause");
}


/*
	邹晓航bug：

	Alloc.cpp 102行，
	应该改成for(int i = bytes;..........
	如果从i = 0开始，当free_list中有可用的足够大的区块时，
	这样的写法将不能发现这些区块，内存池会在小区块上一直循环

	Vector.h
	insert_aux(iterator position, Integer n, const value_type& value, std::true_type)
	覆盖部分可能有内存泄露

	ReverseIterator.h
	reverse_iterator_t& operator ++(int)
	不应该返回引用



	##########################################################################
	##########################################################################
	##########################################################################



	自己的注意点：

	40、关于swap：
	http://www.cnblogs.com/daoluanxiaozi/archive/2011/12/18/2291972.html：
	《effective C++》里面有种做法，就是设置一个swap成员函数，
	在里面再调用STL里面的swap来交换letter指针（更聪明，也容易
	理解，图解了）和其他的数据。所以标准库（STL）中的swap无疑
	是可以用来置换两个自定义类型的（比如说class），但是从上面啰
	嗦那么多就知道需要注意的地方太多了，而且效率也不高，因此虽然
	标准库（STL）强大，我们应该有选择的利用。
*/