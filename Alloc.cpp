#include "Alloc.h"
#include <iostream>
#include <cassert>

namespace MySTL {
	Alloc::obj* Alloc::free_list[Alloc::ELenOfFreeList::len_of_free_list]
		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char* Alloc::start_free = 0;
	char* Alloc::end_free = 0;
	size_t Alloc::heap_size = 0;//��ǰ�������ڴ�صĴ�С�����ۼӵ����ģ��ڴ�����������չ�����free_list

	size_t Alloc::ROUND_UP(size_t bytes) {
		return (bytes + EAlign::ALIGN - 1) / EAlign::ALIGN*EAlign::ALIGN;
		//Ч�ʸߣ� 
		//return ((bytes+EAlign::ALIGN-1)&~(EAlign::ALIGN-1));
	}

	size_t Alloc::FREELIST_INDEX(size_t bytes) {
		size_t bytes_round_up = ROUND_UP(bytes);
		return bytes_round_up / EAlign::ALIGN - 1;
	}

	void Alloc::deallocate(void* ptr, size_t bytes) {
		//���ڴ�ֱ��free
		if (bytes > EMaxBytes::MAXBYTES)
			free(ptr);
		//С�ڴ棬����free_list����Ӧ�ڵ�ָ�������
		else {
			obj** my_free_list = free_list + FREELIST_INDEX(bytes);
			obj* new_node = static_cast<obj*>(ptr);
			assert(new_node != *my_free_list);//���Ϸ���
			new_node->next = *my_free_list;
			*my_free_list = new_node;
		}
	}

	//ԭ���ڴ������ݾ�û����
	void* Alloc::reallocate(void* ptr, size_t old_size, size_t new_size) {
		deallocate(ptr, old_size);
		return allocate(new_size);
	}

	void* Alloc::allocate(size_t bytes) {
		//��һ������
		if (bytes > EMaxBytes::MAXBYTES)
			return malloc(bytes);
		//��free_list��ȡ
		obj** my_free_list = free_list + FREELIST_INDEX(bytes);
		if (*my_free_list) {//��list�л��пռ�
			void* result = *my_free_list;
			*my_free_list = ((obj*)result)->next;
			assert(result != *my_free_list);//���Ϸ���
			return result;
		}
		//��list��û�пռ䣬���ڴ����ȡ
		return refill(ROUND_UP(bytes));
	}

	//bytes��ROUND_UP
	void* Alloc::refill(size_t bytes) {
		size_t nobjs = ENumOfObjs::num_of_objs;
		//���ڴ����ȡ
		char* chunk = chunk_alloc(bytes, nobjs);
		if (nobjs == 1)	//ֻ��һ��
			return chunk;
		obj* result = (obj*)chunk;
		obj** my_free_list = free_list + FREELIST_INDEX(bytes);
		*my_free_list = (obj*)(chunk + bytes);
		//ʣ��ռ����free_list����ӦԪ��ָ�������
		obj* current = *my_free_list;
		obj* next = (obj*)((char*)current + bytes);
		//obj* next = *my_free_list;
		for (int i = 1; i < nobjs - 1; i++) {
			current->next = next;
			current = next;
			next = (obj*)((char*)next + bytes);
		}
		current->next = 0;
		assert(result != *my_free_list);//���Ϸ���
		return result;
	}



	//����nobjs����СΪsize�����飨����ʱnobjs���ܼ�С��
	//size��ROUND_UP
	char* Alloc::chunk_alloc(size_t size, size_t& nobjs) {
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;
		//�ڴ����ȫ������Ҫ
		if (bytes_left >= total_bytes) {
			char* temp = start_free;
			start_free += total_bytes;
			return temp;
		}
		//�ڴ�ز�����ȫ���㣬�����ṩ����һ������
		else if (bytes_left >= size) {
			nobjs = bytes_left / size;//�����������������˶���
			char* result = start_free;
			start_free += nobjs * size;
			return result;
		}
		//һ������������
		else {
			//Ĭ������ģʽ
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(bytes_left >> 4);
			if (bytes_left > 0) {
				//ע�⣺�û��Ʊ�֤bytes_left��[8,128]�У�����8�ı�����������Ϊfree_list�е�һ������
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				//����ǰ�ڴ�ص���������뵽free_list��
				((obj*)start_free)->next = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			start_free = (char*)malloc(bytes_to_get);
			//�Բۣ�����system heap�ռ䶼�����ˣ���malloc�����ˣ��Ĵ�Ѱ������"����δ�����飬�������㹻�� "֮free_lists.�ҵ��˾���һ�齻��
			if (!start_free) {
				for (int i = size /*i = 0*/; i < EMaxBytes::MAXBYTES; i += EAlign::ALIGN) {
					obj** my_free_list = free_list + FREELIST_INDEX(i);
					obj* p = *my_free_list;
					if (*my_free_list) {
						*my_free_list = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						//�Ĵ�Ѱ�ҳɹ����ڴ���Ѹ��£���chunk_alloc��δ�ɹ����ݹ�chunk_alloc
						return chunk_alloc(size, nobjs);
					}
				}
				//����û��
				/*
					TODO:��Ҳ��֪��
				*/
			}
			//malloc�ɹ����ڴ���Ѹ��£���chunk_alloc��δ�ɹ����ݹ�chunk_alloc
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, nobjs);
		}
	}
}