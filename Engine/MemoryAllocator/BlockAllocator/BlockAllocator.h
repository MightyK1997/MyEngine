#pragma once
#include <windows.h>
namespace Engine
{
	namespace MemoryAllocators
	{
		class HeapManager
		{

		public:
			//Create Heap Manager
			HeapManager(size_t i_sizeMemory, void* i_pMemory, unsigned int i_numDescriptors);
			static HeapManager* _Init(void* i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);

			//Destroy Heap Manager
			void destroy();

			//ALlocate memory based on the size and alignment
			void*_alloc(size_t i_size, unsigned int i_alignment = 4);

			//Clear memory based on the pointer
			bool _free(void* i_ptr);

			//Garbage COllection
			void collect();

			~HeapManager();

			//Check whether memory contains the given pointer
			const bool Contains(void* i_ptr);
			//Check whether the pointer is allocated or not
			const bool IsAllocated(void* i_ptr);
			//get the size of the largest contiguous memory available in the heap
			const size_t getLargestFreeBlock();
			//get the total amount of free memory available in the heap.
			const size_t getTotalFreeMemory();

			void* ReturnFirstPointer();

			//DWORD WINAPI MiddleFunction(LPVOID ptr);


		private:
			struct blockDescriptors
			{
				size_t sizeOfHeap;
				blockDescriptors* nextDescriptor;
				bool isAllocated;
				blockDescriptors(size_t size, struct blockDescriptors * i_Next = nullptr) :

					sizeOfHeap(size),
					nextDescriptor(i_Next)
				{
				}
			};
			void* pFirstPointerToHeap;
			size_t totalHeapSize;
			unsigned int numberOfDescriptors;
			LPDWORD threadID = NULL;
			HANDLE returnValue = NULL;
		};
	}
}