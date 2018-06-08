#include <iostream>
#include "BlockAllocator.h"
#ifdef ENVIRONMENT64
const int someValue = 63;
#elif
const int someValue = 31;
#endif

using namespace std;
BlockAllocator::BlockAllocator(size_t i_TotalSizeOfMemory, void* i_pPointerToFirstBlock) :
	p_FirstPointerToHeap(i_pPointerToFirstBlock), m_TotalMemoryHeapSize(i_TotalSizeOfMemory)
{
	BlockDescriptor* blkDesc = static_cast<BlockDescriptor*>(i_pPointerToFirstBlock);
	blkDesc->m_SizeOfBlockAndIsBlockFilled = i_TotalSizeOfMemory - sizeof(BlockDescriptor);
	blkDesc->p_NextDescriptor = nullptr;
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
		if (padding == alignment) padding = 0;
		auto m_blockSize =  size + sizeof(BlockDescriptor) + padding;
		if ((((tempDesc->m_SizeOfBlockAndIsBlockFilled >> someValue) & 1ULL) == 0) && tempDesc->m_SizeOfBlockAndIsBlockFilled > m_blockSize)
		{
			BlockDescriptor* newDesc = reinterpret_cast<BlockDescriptor*>(reinterpret_cast<bool*>(tempDesc) + m_blockSize);
			newDesc->m_SizeOfBlockAndIsBlockFilled = tempDesc->m_SizeOfBlockAndIsBlockFilled - m_blockSize;
			newDesc->p_NextDescriptor = tempDesc->p_NextDescriptor;
			newDesc->m_SizeOfBlockAndIsBlockFilled &= ~(1ULL << someValue);

			tempDesc->p_NextDescriptor = newDesc;
			tempDesc->m_SizeOfBlockAndIsBlockFilled = size + padding;
			tempDesc->m_SizeOfBlockAndIsBlockFilled ^= 1ULL << someValue;
			return reinterpret_cast<void*>(reinterpret_cast<bool*>(tempDesc) + m_blockSize - size);
		}
		else if ((((tempDesc->m_SizeOfBlockAndIsBlockFilled >> someValue) & 1ULL) == 0) && tempDesc->m_SizeOfBlockAndIsBlockFilled <= m_blockSize && tempDesc->m_SizeOfBlockAndIsBlockFilled > size + padding)
		{
			tempDesc->m_SizeOfBlockAndIsBlockFilled |= 1ULL << someValue;
			return reinterpret_cast<void*>(reinterpret_cast<bool*>(tempDesc) + m_blockSize - size);
		}
	}
	return nullptr;
}

void BlockAllocator::_free(void* i_pPointer)
{
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		if ((((tempDesc->m_SizeOfBlockAndIsBlockFilled >> someValue) & 1ULL) == 1))
		{
			BlockDescriptor* temp = new BlockDescriptor(0, nullptr);
			temp->m_SizeOfBlockAndIsBlockFilled = tempDesc->m_SizeOfBlockAndIsBlockFilled;
			temp->m_SizeOfBlockAndIsBlockFilled &= ~(1ULL << someValue);

			if (i_pPointer >= reinterpret_cast<bool*>(tempDesc) + sizeof(BlockDescriptor) && (i_pPointer <= reinterpret_cast<void*>(reinterpret_cast<bool*>(tempDesc) + sizeof(BlockDescriptor) + temp->m_SizeOfBlockAndIsBlockFilled)))
			{
				tempDesc->m_SizeOfBlockAndIsBlockFilled &= ~(1ULL << someValue);
			}
			delete temp;
		}
	}
}

void BlockAllocator::collect()
{
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		for (auto i = tempDesc->p_NextDescriptor; i && (((i->m_SizeOfBlockAndIsBlockFilled >> someValue) & 1ULL) == 0) && (((tempDesc->m_SizeOfBlockAndIsBlockFilled >> someValue) & 1ULL) == 0); i = i->p_NextDescriptor)
		{
			tempDesc->m_SizeOfBlockAndIsBlockFilled += (sizeof(BlockDescriptor) + i->m_SizeOfBlockAndIsBlockFilled);
			tempDesc->p_NextDescriptor = i->p_NextDescriptor;
		}
	}
}

size_t BlockAllocator::GetLargestFreeBlock()
{
	size_t totalSize = 0;
	
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		if (((tempDesc->m_SizeOfBlockAndIsBlockFilled >> someValue) & 1ULL) == 0)
		{
			if (tempDesc->m_SizeOfBlockAndIsBlockFilled > totalSize)
			{
				totalSize = tempDesc->m_SizeOfBlockAndIsBlockFilled;
			}
		}
	}
	return totalSize;
}


size_t BlockAllocator::GetTotalFreeMemory()
{
	size_t totalSize = 0;
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		if (((tempDesc->m_SizeOfBlockAndIsBlockFilled >> someValue) & 1ULL) == 0)
		{
			totalSize += tempDesc->m_SizeOfBlockAndIsBlockFilled;
		}
	}
	return totalSize;
}

bool BlockAllocator::Contains(void* i_pPointer)
{
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		if (i_pPointer >= reinterpret_cast<bool*>(tempDesc) + sizeof(BlockDescriptor))
		{
			return true;
		}
	}
	return false;
}

bool BlockAllocator::IsAllocated(void* i_pPointer)
{
	for (BlockDescriptor* tempDesc = static_cast<BlockDescriptor*>(p_FirstPointerToHeap); tempDesc; tempDesc = tempDesc->p_NextDescriptor)
	{
		BlockDescriptor* temp = new BlockDescriptor(0, nullptr);
		temp->m_SizeOfBlockAndIsBlockFilled = tempDesc->m_SizeOfBlockAndIsBlockFilled;
		temp->m_SizeOfBlockAndIsBlockFilled &= ~(1ULL << someValue);
		if (i_pPointer >= reinterpret_cast<bool*>(tempDesc) + sizeof(BlockDescriptor) && (i_pPointer <= reinterpret_cast<void*>(reinterpret_cast<bool*>(tempDesc) + sizeof(BlockDescriptor) + temp->m_SizeOfBlockAndIsBlockFilled)))
		{
			delete temp;
			return ((tempDesc->m_SizeOfBlockAndIsBlockFilled >> someValue) & 1ULL) == 1 ? true : false;
		}
		delete temp;
	}
	return false;
}
BlockAllocator::~BlockAllocator()
{
	destroy();
}

void BlockAllocator::destroy()
{
	//_aligned_free(p_FirstPointerToHeap);
}