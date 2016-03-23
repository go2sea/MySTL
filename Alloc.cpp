#include "Alloc.h"
#include <iostream>
#include <cassert>

namespace MySTL {
	Alloc::obj* Alloc::free_list[Alloc::ELenOfFreeList::len_of_free_list]
		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char* Alloc::start_free = 0;
	char* Alloc::end_free = 0;
	size_t Alloc::heap_size = 0;//当前总申请内存池的大小，是累加递增的，内存池申请后的最终归宿是free_list

	size_t Alloc::ROUND_UP(size_t bytes) {
		return (bytes + EAlign::ALIGN - 1) / EAlign::ALIGN*EAlign::ALIGN;
		//效率高： 
		//return ((bytes+EAlign::ALIGN-1)&~(EAlign::ALIGN-1));
	}

	size_t Alloc::FREELIST_INDEX(size_t bytes) {
		size_t bytes_round_up = ROUND_UP(bytes);
		return bytes_round_up / EAlign::ALIGN - 1;
	}

	void Alloc::deallocate(void* ptr, size_t bytes) {
		//大内存直接free
		if (bytes > EMaxBytes::MAXBYTES)
			free(ptr);
		//小内存，插入free_list中相应节点指向的链表
		else {
			obj** my_free_list = free_list + FREELIST_INDEX(bytes);
			obj* new_node = static_cast<obj*>(ptr);
			assert(new_node != *my_free_list);//检测合法性
			new_node->next = *my_free_list;
			*my_free_list = new_node;
		}
	}

	//原先内存块的内容就没啦！
	void* Alloc::reallocate(void* ptr, size_t old_size, size_t new_size) {
		deallocate(ptr, old_size);
		return allocate(new_size);
	}

	void* Alloc::allocate(size_t bytes) {
		//第一级处理
		if (bytes > EMaxBytes::MAXBYTES)
			return malloc(bytes);
		//从free_list中取
		obj** my_free_list = free_list + FREELIST_INDEX(bytes);
		if (*my_free_list) {//此list中还有空间
			void* result = *my_free_list;
			*my_free_list = ((obj*)result)->next;
			assert(result != *my_free_list);//检测合法性
			return result;
		}
		//此list中没有空间，从内存池中取
		return refill(ROUND_UP(bytes));
	}

	//bytes已ROUND_UP
	void* Alloc::refill(size_t bytes) {
		size_t nobjs = ENumOfObjs::num_of_objs;
		//从内存池中取
		char* chunk = chunk_alloc(bytes, nobjs);
		if (nobjs == 1)	//只够一个
			return chunk;
		obj* result = (obj*)chunk;
		obj** my_free_list = free_list + FREELIST_INDEX(bytes);
		*my_free_list = (obj*)(chunk + bytes);
		//剩余空间加入free_list中相应元素指向的链表
		obj* current = *my_free_list;
		obj* next = (obj*)((char*)current + bytes);
		//obj* next = *my_free_list;
		for (int i = 1; i < nobjs - 1; i++) {
			current->next = next;
			current = next;
			next = (obj*)((char*)next + bytes);
		}
		current->next = 0;
		assert(result != *my_free_list);//检测合法性
		return result;
	}



	//配置nobjs个大小为size的区块（不够时nobjs可能减小）
	//size已ROUND_UP
	char* Alloc::chunk_alloc(size_t size, size_t& nobjs) {
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;
		//内存池完全满足需要
		if (bytes_left >= total_bytes) {
			char* temp = start_free;
			start_free += total_bytes;
			return temp;
		}
		//内存池不能完全满足，但能提供至少一个区块
		else if (bytes_left >= size) {
			nobjs = bytes_left / size;//给个反馈到底申请了多少
			char* result = start_free;
			start_free += nobjs * size;
			return result;
		}
		//一个都不能满足
		else {
			//默认增长模式
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(bytes_left >> 4);
			if (bytes_left > 0) {
				//注意：该机制保证bytes_left在[8,128]中，且是8的倍数，即可作为free_list中的一个区块
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				//将当前内存池当做区块插入到free_list中
				((obj*)start_free)->next = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			start_free = (char*)malloc(bytes_to_get);
			//卧槽！整个system heap空间都不够了！连malloc都跪了：四处寻找有无"尚有未用区块，且区块足够大 "之free_lists.找到了就挖一块交出
			if (!start_free) {
				for (int i = size /*i = 0*/; i < EMaxBytes::MAXBYTES; i += EAlign::ALIGN) {
					obj** my_free_list = free_list + FREELIST_INDEX(i);
					obj* p = *my_free_list;
					if (*my_free_list) {
						*my_free_list = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						//四处寻找成功，内存池已更新，但chunk_alloc尚未成功，递归chunk_alloc
						return chunk_alloc(size, nobjs);
					}
				}
				//彻底没招
				/*
					TODO:我也不知道
				*/
			}
			//malloc成功，内存池已更新，但chunk_alloc尚未成功，递归chunk_alloc
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, nobjs);
		}
	}
}