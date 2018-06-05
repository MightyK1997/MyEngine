#pragma once


struct BlockDestriptor
{
	void* p_ReturnPointer;
	size_t m_SizeOfBlockAndIsBlockFilled;
	BlockDestriptor* p_NextBlock;
};

class BlockAllocator
{
public:
	void* _alloc(size_t i_BlockSize);
	void _free(void* i_pPointer);
	size_t GetLargestFreeBlock();
	bool IsBlockFree(void* i_pPointer);




private:


	size_t m_TotalMemoryHeapSize;
	void* p_FirstPointerToHeap;
};