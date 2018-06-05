#include "AllocatingAllocators.h"
#include "..\MemoryAllocator\BlockAllocator\BlockAllocator.h"
#include <malloc.h>
#include <new>
#include <cassert>
#include "..\MemoryAllocator\OverloadingNewAndDelete.h"

namespace Engine
{
	namespace MemoryAllocators
	{
		AllocatingAllocators::AllocatingAllocators()
		{
			_Init();
		}

		AllocatingAllocators::AllocatingAllocators(size_t i_size, void* i_pPointer, unsigned int i_numDescriptors)
		{
			m_blockAllocator = new(i_pPointer) HeapManager(i_size, i_pPointer, i_numDescriptors);
			InitializeFSA();
		}


		AllocatingAllocators::~AllocatingAllocators()
		{
		}
		void AllocatingAllocators::collect()
		{
			m_blockAllocator->collect();
		}
		

		void AllocatingAllocators::Destroy()
		{
			FSA_8Byte->~FixedSizeAllocator();
			FSA_16Byte->~FixedSizeAllocator();
			FSA_32Byte->~FixedSizeAllocator();
			FSA_64Byte->~FixedSizeAllocator();
			FSA_128Byte->~FixedSizeAllocator();
			FSA_256Byte->~FixedSizeAllocator();
			m_blockAllocator->destroy();
			free(m_newHeap);
		}


		AllocatingAllocators* AllocatingAllocators::_Init()
		{
				size_t i_sizeMemory = 8192 * 8192;
				void* i_pMemory = _aligned_malloc(i_sizeMemory, 4);
			m_newHeap = malloc(sizeof(HeapManager));
			m_blockAllocator = new(m_newHeap) HeapManager(i_sizeMemory, i_pMemory, 100);
			InitializeFSA();
			return this;
		}

		void* AllocatingAllocators::_alloc(size_t i_size, unsigned int i_alignment)
		{
			void* m_returnPointer = nullptr;
			if (FSA_8Byte && FSA_16Byte && FSA_32Byte && FSA_64Byte && FSA_128Byte && FSA_256Byte)
			{
				if (i_size <= 8)
				{
					m_returnPointer = FSA_8Byte->_alloc(i_alignment);
				}
				else if (i_size > 8 && i_size <= 16)
				{
					m_returnPointer = FSA_16Byte->_alloc(i_alignment);
				}
				else if (i_size > 16 && i_size <= 32)
				{
					m_returnPointer = FSA_32Byte->_alloc(i_alignment);
				}
				else if (i_size > 32 && i_size <= 64)
				{
					m_returnPointer = FSA_64Byte->_alloc(i_alignment);
				}
				else if (i_size > 64 && i_size <= 128)
				{
					m_returnPointer = FSA_128Byte->_alloc(i_alignment);
				}
				else if (i_size > 128 && i_size <= 256)
				{
					m_returnPointer = FSA_256Byte->_alloc(i_alignment);
				}
				else
				{
					m_returnPointer = m_blockAllocator->_alloc(i_size, i_alignment);
				}
			}
			else
			{
				m_returnPointer = m_blockAllocator->_alloc(i_size, i_alignment);
			}
			return m_returnPointer;
		}

		bool AllocatingAllocators::_free(void* i_pPointer)
		{
			assert(i_pPointer);
			if (FSA_8Byte->IsContained(i_pPointer))
			{
				FSA_8Byte->_free(i_pPointer);
				return true;
			}
			else if (FSA_16Byte->IsContained(i_pPointer))
			{
				FSA_16Byte->_free(i_pPointer);
				return true;
			}
			else if (FSA_32Byte->IsContained(i_pPointer))
			{
				FSA_32Byte->_free(i_pPointer);
				return true;
			}
			else if (FSA_64Byte->IsContained(i_pPointer))
			{
				FSA_64Byte->_free(i_pPointer);
				return true;
			}
			else if (FSA_128Byte->IsContained(i_pPointer))
			{
				FSA_128Byte->_free(i_pPointer);
				return true;
			}
			else if (FSA_256Byte->IsContained(i_pPointer))
			{
				FSA_256Byte->_free(i_pPointer);
				return true;
			}
			else if (m_blockAllocator->Contains(i_pPointer))
			{
				m_blockAllocator->_free(i_pPointer);
				return true;
			}
			else
			{
				return false;
			}
		}

		const bool AllocatingAllocators::Contains(void* i_pPointer)
		{
			assert(i_pPointer);
			if (FSA_8Byte->IsContained(i_pPointer) || FSA_16Byte->IsContained(i_pPointer) || FSA_32Byte->IsContained(i_pPointer) || FSA_64Byte->IsContained(i_pPointer) || FSA_128Byte->IsContained(i_pPointer) || FSA_256Byte->IsContained(i_pPointer) || m_blockAllocator->Contains(i_pPointer))
				return true;
			return false;
		}

		const size_t AllocatingAllocators::getLargestFreeBlock()
		{
			return m_blockAllocator->getLargestFreeBlock();
		}

		const size_t AllocatingAllocators::getTotalFreeMemory()
		{
			return m_blockAllocator->getTotalFreeMemory();
		}

		const bool AllocatingAllocators::IsAllocated(void* i_pPointer)
		{
			assert(i_pPointer);
			return m_blockAllocator->IsAllocated(i_pPointer);
		}
		void AllocatingAllocators::InitializeFSA()
		{
			FSA_8Byte = new(m_blockAllocator) FixedSizeAllocator(FSA_8Byte, m_blockAllocator);
			FSA_8Byte->SetFirstPointer(FSA_8Byte);
			FSA_16Byte = new(m_blockAllocator) FixedSizeAllocator(FSA_16Byte, m_blockAllocator, 16);
			FSA_16Byte->SetFirstPointer(FSA_16Byte);
			FSA_32Byte = new(m_blockAllocator) FixedSizeAllocator(FSA_32Byte, m_blockAllocator, 32);
			FSA_32Byte->SetFirstPointer(FSA_32Byte);
			FSA_64Byte = new(m_blockAllocator) FixedSizeAllocator(FSA_64Byte, m_blockAllocator, 64);
			FSA_64Byte->SetFirstPointer(FSA_64Byte);
			FSA_128Byte = new(m_blockAllocator) FixedSizeAllocator(FSA_128Byte, m_blockAllocator, 128);
			FSA_128Byte->SetFirstPointer(FSA_128Byte);
			FSA_256Byte = new(m_blockAllocator) FixedSizeAllocator(FSA_256Byte, m_blockAllocator, 256);
			FSA_256Byte->SetFirstPointer(FSA_256Byte);
		}

		void* AllocatingAllocators::GetFixedSizedAllocator(size_t i_size)
		{
			if (i_size <= 8 && FSA_8Byte)
			{
				return FSA_8Byte;
			}
			else if (i_size > 8 && i_size <= 16 && FSA_16Byte)
			{
				return FSA_16Byte;
			}
			else if (i_size > 16 && i_size <= 32 && FSA_32Byte)
			{
				return FSA_32Byte;
			}
			else if (i_size > 32 && i_size <= 64 && FSA_64Byte)
			{
				return FSA_64Byte;
			}
			else if (i_size > 64 && i_size <= 128 && FSA_128Byte)
			{
				return FSA_128Byte;
			}
			else if (i_size > 128 && i_size <= 256 && FSA_256Byte)
			{
				return FSA_256Byte;
			}
			else
				return nullptr;
		}

		void* AllocatingAllocators::GetBlockAllocator()
		{
			return m_blockAllocator;
		}
	}
}