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
	������bug��

	Alloc.cpp 102�У�
	Ӧ�øĳ�for(int i = bytes;..........
	�����i = 0��ʼ����free_list���п��õ��㹻�������ʱ��
	������д�������ܷ�����Щ���飬�ڴ�ػ���С������һֱѭ��

	Vector.h
	insert_aux(iterator position, Integer n, const value_type& value, std::true_type)
	���ǲ��ֿ������ڴ�й¶

	ReverseIterator.h
	reverse_iterator_t& operator ++(int)
	��Ӧ�÷�������



	##########################################################################
	##########################################################################
	##########################################################################



	�Լ���ע��㣺

	40������swap��
	http://www.cnblogs.com/daoluanxiaozi/archive/2011/12/18/2291972.html��
	��effective C++������������������������һ��swap��Ա������
	�������ٵ���STL�����swap������letterָ�루��������Ҳ����
	��⣬ͼ���ˣ������������ݡ����Ա�׼�⣨STL���е�swap����
	�ǿ��������û������Զ������͵ģ�����˵class�������Ǵ����憪
	����ô���֪����Ҫע��ĵط�̫���ˣ�����Ч��Ҳ���ߣ������Ȼ
	��׼�⣨STL��ǿ������Ӧ����ѡ������á�
*/