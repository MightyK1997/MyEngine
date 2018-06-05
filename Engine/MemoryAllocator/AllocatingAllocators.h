#pragma once

#include "..\MemoryAllocator\BlockAllocator\BlockAllocator.h"
#include "..\MemoryAllocator\FixedSizeAllocator\FixedSizeAllocator.h"
namespace Engine
{
	namespace MemoryAllocators
	{
		class AllocatingAllocators
		{
		public:

			//Constructor
			AllocatingAllocators();

			//Overloaded Constructor.
			AllocatingAllocators(size_t i_size, void* i_pPointer, unsigned int i_numDescriptors);

			//Destructor.
			~AllocatingAllocators();

			//Contains code for the initialization of block allocator.
			AllocatingAllocators* _Init();

			//Contains code for allocation based on the size of the allocation.
			void* _alloc(size_t i_size, unsigned int i_alignment = 4);

			//Checks for pointer in all allocators and frees it.
			bool _free(void* i_pPointer);

			//Garbage COllection (Only for block allocator)
			void collect();

			//Check whether memory contains the given pointer
			const bool Contains(void* i_ptr);
			//Check whether the pointer is allocated or not
			const bool IsAllocated(void* i_ptr);
			//get the size of the largest contiguous memory available in the heap (Only for block allocator)
			const size_t getLargestFreeBlock();
			//get the total amount of free memory available in the heap. (Only for block allocator)
			const size_t getTotalFreeMemory();

			//Get the pointer to specific FSA for the given size
			void* GetFixedSizedAllocator(size_t i_size);

			//Get the heap allocator pointer.
			void* GetBlockAllocator();

			//Initialize all the FSAs from the heap allocator.
			void InitializeFSA();

			//Calls destructors of all allocators.
			void Destroy();

		private:
			HeapManager* m_blockAllocator = nullptr;
			FixedSizeAllocator* FSA_8Byte = nullptr;
			FixedSizeAllocator* FSA_16Byte = nullptr;
			FixedSizeAllocator* FSA_32Byte = nullptr;
			FixedSizeAllocator* FSA_64Byte = nullptr;
			FixedSizeAllocator* FSA_128Byte = nullptr;
			FixedSizeAllocator* FSA_256Byte = nullptr;
			void* i_pMemory;
			void* m_newHeap;
		};
	}
}