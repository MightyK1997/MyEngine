// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif




#pragma once
#include <bitset>


class BlockAllocator
{
public:
	//Constructor and Destructor
	BlockAllocator(size_t i_sizeMemory, void* i_pMemory);
	static BlockAllocator* _Init(size_t i_sizeMemory, void* i_pMemory);
	~BlockAllocator() {}


	//Other Public Functions
	void* _alloc(size_t i_BlockSize, unsigned int alignment = 4);
	void _free(void* i_pPointer);
	void collect();
	size_t GetLargestFreeBlock();
	size_t GetTotalFreeMemory();
	const bool Contains(void* i_pPointer);
	bool IsBlockFree(void* i_pPointer);


private:
	struct BlockDescriptor
	{
		void* p_ReturnPointer;
		size_t m_SizeOfBlockAndIsBlockFilled;
		BlockDescriptor* p_NextDescriptor;
		BlockDescriptor(size_t size, struct BlockDescriptor* i_pNextBlockDescriptor = nullptr) : m_SizeOfBlockAndIsBlockFilled(size), p_ReturnPointer(i_pNextBlockDescriptor) {}
	};

	size_t m_TotalMemoryHeapSize;
	void* p_FirstPointerToHeap;
};