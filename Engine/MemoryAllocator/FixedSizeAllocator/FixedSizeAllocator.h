#pragma once
#define GUARD_BAND_SIZE 8;
#include "..\BlockAllocator\BlockAllocator.h"
#include "BitArray.h"

namespace Engine
{
	namespace MemoryAllocators 
	{
		class FixedSizeAllocator
		{
		public:

			//Constructor for creating different Allocators with default containing 8 byte blocks.
			FixedSizeAllocator(FixedSizeAllocator* i_pFirst, HeapManager* mHeapManager, size_t blockSize = 8);

			//Destructor.
			~FixedSizeAllocator();

			//Allocates memory
			void* _alloc(unsigned int i_alignment = 8);

			//Frees memory
			void _free(void* i_pPointer);


			//Checks if a pointer returned by a program for freeing is a pointer to a block in the given allocator.

			//TODO - Implement this function.
			bool IsValidBlockPointer(void* i_pPointer);

			//Checks if a pointer belongs to a particular sized allocator.
			bool IsContained(void* i_pPointer);

			//Checks if a block is free or not
			bool IsFree(void* i_pPointer);

			//Returns the pointer to the FSA.
			void* ReturnFirstPointer();

			//Sets the pointer to the FSA.
			void SetFirstPointer(FixedSizeAllocator* i_pPointer);



		private:
			struct allocationList
			{
				allocationList* nextPointer;
			};
			FixedSizeAllocator* pFirstPointer;
			void* pLastPointerToHeap = nullptr;
			BitArray* myBitArray;
			size_t allocBlockSize;
			size_t totalBlockSize;
			HeapManager* mHeapManager;
			void* pFirstAllocationPointer = nullptr;
		};
	}
}