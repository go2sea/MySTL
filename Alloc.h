#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>

namespace MySTL {
	/*
		Alloc�����ֽ�Ϊ��λ�����ڴ�
	*/
	class Alloc {
	public:
		static void* allocate(size_t bytes);
		static void deallocate(void* ptr, size_t bytes);
		//ע�⣺û�п���ԭ������
		static void* reallocate(void* ptr, size_t old_size, size_t new_size);
	private:
		enum EAlign { ALIGN = 8 };
		enum EMaxBytes { MAXBYTES = 128 };
		enum ELenOfFreeList { len_of_free_list = EMaxBytes::MAXBYTES / EAlign::ALIGN };
		enum ENumOfObjs { num_of_objs = 20 };

		union obj {
			union obj* next;
			char client_data[1];
		};
		//����Ϊ16���洢����������׵�ַ
		static obj* free_list[ELenOfFreeList::len_of_free_list];
		static char* start_free;//�ڴ���׵�ַ
		static char* end_free;//�ڴ�ؽ���β��ַ�����һ���ֽڵĺ�һ���ֽڣ�
		static size_t heap_size;//�ڴ�ش�С

		//��bytes�ϵ���8��EAlign::ALIGN���ı���
		static size_t ROUND_UP(size_t bytes);
		//����bytes��С�õ�����ţ�free_list�е�λ�ã���bytesδROUND_UP��
		static size_t FREELIST_INDEX(size_t bytes);
		//����һ����СΪn���ڴ����׵�ַ����Ϊfree_list��ӦԪ��ָ����������ӽڵ㣨����������
		static void* refill(size_t n);
		//����nobjs����СΪsize�����飨����ʱnobjs���ܼ�С��
		static char* chunk_alloc(size_t size, size_t& nobjs);
	};
}

#endif