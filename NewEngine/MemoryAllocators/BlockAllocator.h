#pragma once


class BlockAllocator
{
public:
	//Constructor and Destructor
	BlockAllocator(size_t i_sizeMemory, void* i_pMemory);
	static BlockAllocator* _Init(void* i_pMemory, size_t i_sizeMemory);
	~BlockAllocator() {}


	//Other Public Functions
	void* _alloc(size_t i_BlockSize);
	void _free(void* i_pPointer);
	void collect();
	size_t GetLargestFreeBlock();
	size_t GetTotalFreeMemory();
	const bool Contains(void* i_pPointer);
	bool IsBlockFree(void* i_pPointer);


private:
	struct BlockDestriptor
	{
		void* p_ReturnPointer;
		size_t m_SizeOfBlockAndIsBlockFilled;
		BlockDestriptor(size_t size, struct BlockDestriptor* i_pNextBlockDescriptor = nullptr) : m_SizeOfBlockAndIsBlockFilled(size), p_ReturnPointer(i_pNextBlockDescriptor) {}
	};

	size_t m_TotalMemoryHeapSize;
	void* p_FirstPointerToHeap;
};