#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>

namespace MySTL {
	/*
		Alloc：以字节为单位配置内存
	*/
	class Alloc {
	public:
		static void* allocate(size_t bytes);
		static void deallocate(void* ptr, size_t bytes);
		//注意：没有拷贝原有内容
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
		//长度为16，存储各个链表的首地址
		static obj* free_list[ELenOfFreeList::len_of_free_list];
		static char* start_free;//内存池首地址
		static char* end_free;//内存池结束尾地址（最后一个字节的后一个字节）
		static size_t heap_size;//内存池大小

		//将bytes上调至8（EAlign::ALIGN）的倍数
		static size_t ROUND_UP(size_t bytes);
		//根据bytes大小得到区块号（free_list中的位置）（bytes未ROUND_UP）
		static size_t FREELIST_INDEX(size_t bytes);
		//返回一个大小为n的内存块的首地址，并为free_list相应元素指向的链表增加节点（个数不定）
		static void* refill(size_t n);
		//配置nobjs个大小为size的区块（不够时nobjs可能减小）
		static char* chunk_alloc(size_t size, size_t& nobjs);
	};
}

#endif