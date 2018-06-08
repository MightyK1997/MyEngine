#pragma once


#ifndef __HEAP_MANAGER_PROXY_H
#define __HEAP_MANAGER_PROXY_H

#include <stdint.h>

#include "..\NewEngine\MemoryAllocators\BlockAllocator.h"

class BlockAllocator;

namespace HeapManagerProxy
{
	BlockAllocator* ba;
	BlockAllocator *	CreateHeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
	{
		ba = new BlockAllocator(i_sizeMemory, i_pMemory);
		return ba;
	}
	void Destroy(BlockAllocator * i_pManager)
	{
		ba->~BlockAllocator();
		delete ba;
	}

	void *			alloc(BlockAllocator * i_pManager, size_t i_size)
	{
		return ba->_alloc(i_size);
	}

	void *			alloc(BlockAllocator * i_pManager, size_t i_size, unsigned int i_alignment)
	{
		return ba->_alloc(i_size, i_alignment);
	}
	bool			free(BlockAllocator * i_pManager, void * i_ptr)
	{
		ba->_free(i_ptr);
		return true;
	}

	void			Collect(BlockAllocator * i_pManager)
	{
		ba->collect();
	}

	bool			Contains(const BlockAllocator * i_pManager, void * i_ptr)
	{
		return ba->Contains(i_ptr);
	}
	bool			IsAllocated(const BlockAllocator * i_pManager, void * i_ptr)
	{
		return ba->IsAllocated(i_ptr);
	}

	size_t			GetLargestFreeBlock(const BlockAllocator * i_pManager)
	{
		return ba->GetLargestFreeBlock();
	}
	size_t			GetTotalFreeMemory(const BlockAllocator * i_pManager)
	{
		return ba->GetTotalFreeMemory();
	}

	void			ShowFreeBlocks(const BlockAllocator * i_pManager);
	void			ShowOutstandingAllocations(const BlockAllocator * i_pManager);

} // namespace HeapManagerProxy

#endif // __HEAP_MANAGER_PROXY_H
