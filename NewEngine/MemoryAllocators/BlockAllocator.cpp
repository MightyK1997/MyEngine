

#include "BlockAllocator.h"
#ifdef ENVIRONMENT64
const int someValue = 63;
#elif
const int someValue = 31;
#endif

BlockAllocator::BlockAllocator(size_t i_TotalSizeOfMemory, void* i_pPointerToFirstBlock) :
	p_FirstPointerToHeap(i_pPointerToFirstBlock), m_TotalMemoryHeapSize(i_TotalSizeOfMemory)
{
	BlockDescriptor* blkDesc = static_cast<BlockDescriptor*>(i_pPointerToFirstBlock);
	blkDesc->m_SizeOfBlockAndIsBlockFilled = i_TotalSizeOfMemory - sizeof(BlockDescriptor);
	blkDesc->p_NextDescriptor = nullptr;
//#ifdef ENVIRONMENT64
//	std::bitset<sizeof(blkDesc->m_SizeOfBlockAndIsBlockFilled)>(blkDesc->m_SizeOfBlockAndIsBlockFilled)[63] = 0;
//#elif ENVIRONMENT32
//	std::bitset<sizeof(blkDesc->m_SizeOfBlockAndIsBlockFilled)>(blkDesc->m_SizeOfBlockAndIsBlockFilled)[31] = 0;
//#endif
}

BlockAllocator* BlockAllocator::_Init(size_t i_sizeMemory, void* i_pMemory)
{
	return new BlockAllocator(i_sizeMemory, i_pMemory);
}

void* BlockAllocator::_alloc(size_t size, unsigned int alignment)
{
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		auto padding = (alignment - ((reinterpret_cast<uintptr_t>(tempDesc) + sizeof(BlockDescriptor)) % (alignment)));
		auto m_blockSize =  padding == 0 ? size + sizeof(BlockDescriptor) : size + sizeof(BlockDescriptor) + padding;
		if (std::bitset<someValue>(tempDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] == 0 && tempDesc->m_SizeOfBlockAndIsBlockFilled > m_blockSize)
		{
			BlockDescriptor* newDesc = reinterpret_cast<BlockDescriptor*>(reinterpret_cast<bool*>(tempDesc) + m_blockSize);
			newDesc->m_SizeOfBlockAndIsBlockFilled = tempDesc->m_SizeOfBlockAndIsBlockFilled - m_blockSize;
			newDesc->p_NextDescriptor = tempDesc->p_NextDescriptor;
			std::bitset<someValue>(newDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] = 0;

			tempDesc->p_NextDescriptor = newDesc;
			tempDesc->m_SizeOfBlockAndIsBlockFilled = size + padding;
			std::bitset<someValue>(tempDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] = 1;
			return reinterpret_cast<void*>(reinterpret_cast<bool*>(tempDesc) + m_blockSize - size);
		}
		else if (std::bitset<someValue>(tempDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] == 0 && tempDesc->m_SizeOfBlockAndIsBlockFilled <= m_blockSize && tempDesc->m_SizeOfBlockAndIsBlockFilled > size + padding)
		{
			std::bitset<someValue>(tempDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] = 1;
			return reinterpret_cast<void*>(reinterpret_cast<bool*>(tempDesc) + m_blockSize - size);
		}
	}
	return nullptr;
}

void BlockAllocator::_free(void* i_pPointer)
{
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		if (std::bitset<someValue>(tempDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] == 1)
		{
			BlockDescriptor* temp = new BlockDescriptor(0, nullptr);
			temp->m_SizeOfBlockAndIsBlockFilled = tempDesc->m_SizeOfBlockAndIsBlockFilled;
 			std::bitset<someValue>(temp->m_SizeOfBlockAndIsBlockFilled)[someValue] = 0;

			if (i_pPointer >= reinterpret_cast<bool*>(tempDesc) + sizeof(BlockDescriptor) && (i_pPointer <= reinterpret_cast<void*>(reinterpret_cast<bool*>(tempDesc) + sizeof(BlockDescriptor) + temp->m_SizeOfBlockAndIsBlockFilled)))
			{
				std::bitset<someValue>(tempDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] = 0;
			}
		}
	}
}

void BlockAllocator::collect()
{
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		for (auto i = tempDesc->p_NextDescriptor; i && (std::bitset<someValue>(i->m_SizeOfBlockAndIsBlockFilled)[someValue] == 0) && (std::bitset<someValue>(tempDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] == 0); i = i->p_NextDescriptor)
		{
			tempDesc->m_SizeOfBlockAndIsBlockFilled += sizeof(BlockAllocator) + i->m_SizeOfBlockAndIsBlockFilled;
			tempDesc->p_NextDescriptor = i->p_NextDescriptor;
		}
	}
}

size_t BlockAllocator::GetTotalFreeMemory()
{
	size_t totalSize = 0;
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		if ((std::bitset<someValue>(tempDesc->m_SizeOfBlockAndIsBlockFilled)[someValue] == 0))
		{
			if (tempDesc->m_SizeOfBlockAndIsBlockFilled > totalSize)
			{
				totalSize = tempDesc->m_SizeOfBlockAndIsBlockFilled;
			}
		}
	}
	return totalSize;
}
