#include "BlockAllocator.h"

#include <stdint.h>
#include<assert.h>
#include<malloc.h>
#include <windows.h>
#include <process.h>
#include "..\..\Logging\ConsolePrint.h"


int count = 0;
namespace Engine
{
	namespace MemoryAllocators
	{
		HeapManager* HeapManager::_Init(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
		{
			return new HeapManager(i_sizeMemory, i_pMemory, i_numDescriptors);
		}

		DWORD WINAPI MiddleFunction(LPVOID ptr)
		{
			Engine::ConsolePrint_Windows("Enter MiddleFunction", __LINE__, __FILE__, "BlockAllocator");
			HeapManager* hMgr = (HeapManager*)ptr;
			hMgr->collect();
			return 1;
		}

		HeapManager::HeapManager(size_t i_sizeMemory, void * i_pMemory, unsigned int i_numDescriptors) :
			pFirstPointerToHeap(i_pMemory),
			totalHeapSize(i_sizeMemory),
			numberOfDescriptors(i_numDescriptors)
		{
			//availableHeapSize = totalHeapSize;
			blockDescriptors* blkDesc = (blockDescriptors*)i_pMemory;
			blkDesc->isAllocated = false;
			blkDesc->nextDescriptor = nullptr;
			blkDesc->sizeOfHeap = i_sizeMemory - sizeof(blockDescriptors);
			if (!returnValue)
			{
				returnValue = CreateThread(NULL, 0, MiddleFunction, this, 0, threadID);
				SetThreadPriority(returnValue, 0x00010000);
			}
		}
		


		void* HeapManager::_alloc(size_t size, unsigned int alignment)
		{

			for (blockDescriptors* tempDesc = (blockDescriptors*)pFirstPointerToHeap; tempDesc/*->nextDescriptor != nullptr*/; tempDesc = tempDesc->nextDescriptor)
			{
				auto padding = alignment - ((reinterpret_cast<uintptr_t>(tempDesc) + sizeof(blockDescriptors)) % (alignment));
				if (padding == alignment) padding = 0;

				size_t newSize = size + sizeof(blockDescriptors) + padding;

				if (tempDesc->isAllocated == false && tempDesc->sizeOfHeap > newSize)
				{
					blockDescriptors* newDesc = (blockDescriptors*)((bool*)tempDesc + newSize);
					newDesc->isAllocated = false;
					newDesc->nextDescriptor = tempDesc->nextDescriptor;
					newDesc->sizeOfHeap = tempDesc->sizeOfHeap - newSize;

					tempDesc->isAllocated = true;
					tempDesc->nextDescriptor = newDesc;
					tempDesc->sizeOfHeap = size + padding;

					return (void*)((bool*)(tempDesc)+newSize - size);
				}
				else if ((tempDesc->isAllocated == false) && tempDesc->sizeOfHeap <= newSize && tempDesc->sizeOfHeap > size + padding)
				{
					tempDesc->isAllocated = true;
					return (void*)((bool*)(tempDesc)+newSize - size);
				}
			}
			//assert(false);
			return nullptr;
		}

		void HeapManager::destroy()
		{
			_aligned_free(pFirstPointerToHeap);
		}

		const size_t HeapManager::getLargestFreeBlock()
		{
			size_t largestMemory = 0;
			for (blockDescriptors* tempDesc = (blockDescriptors*)pFirstPointerToHeap; tempDesc; tempDesc = tempDesc->nextDescriptor)
			{
				if (!tempDesc->isAllocated)
				{
					if (tempDesc->sizeOfHeap > largestMemory)
					{
						largestMemory = tempDesc->sizeOfHeap;
					}
				}
			}
			return largestMemory;
		}

		const size_t HeapManager::getTotalFreeMemory()
		{
			size_t freeMemory = 0;
			for (blockDescriptors* tempDesc = (blockDescriptors*)pFirstPointerToHeap; tempDesc; tempDesc = tempDesc->nextDescriptor)
			{
				if (!tempDesc->isAllocated)
				{
					freeMemory += tempDesc->sizeOfHeap;
				}
			}
			return freeMemory;
		}



		void HeapManager::collect()
		{
			count++;
			Engine::ConsolePrint_Windows("In Collect", __LINE__, __FILE__, "BlockAllocator");
			for (blockDescriptors* tempDesc = (blockDescriptors*)pFirstPointerToHeap; tempDesc; tempDesc = tempDesc->nextDescriptor)
			{
				for (auto i = tempDesc->nextDescriptor; i && (i->isAllocated == false) && (tempDesc->isAllocated == false); i = i->nextDescriptor)
				{
					tempDesc->sizeOfHeap += sizeof(blockDescriptors) + i->sizeOfHeap;
					tempDesc->nextDescriptor = i->nextDescriptor;
				}
			}
		}

		bool HeapManager::_free(void* ptr)
		{
			for (blockDescriptors* tempDesc = (blockDescriptors*)pFirstPointerToHeap; tempDesc; tempDesc = tempDesc->nextDescriptor)
			{
				if (ptr >= (bool*)tempDesc + sizeof(blockDescriptors) && (ptr <= (void*)((bool*)tempDesc + sizeof(blockDescriptors) + tempDesc->sizeOfHeap)))
				{
					tempDesc->isAllocated = false;
					return true;
				}
			}
			return false;
		}

		const bool HeapManager::Contains(void* ptr)
		{
			if (ptr > pFirstPointerToHeap && ptr < (bool*)pFirstPointerToHeap + totalHeapSize)
			{
				return true;
			}
			else
			{
				return false;
			}

			//return true;
		}

		const bool HeapManager::IsAllocated(void* ptr)
		{
			for (blockDescriptors* tempDesc = (blockDescriptors*)pFirstPointerToHeap; tempDesc; tempDesc = tempDesc->nextDescriptor)
			{
				if (ptr >= (bool*)tempDesc + sizeof(blockDescriptors) && (ptr <= (void*)((bool*)tempDesc + sizeof(blockDescriptors) + tempDesc->sizeOfHeap)))
				{
					return tempDesc->isAllocated;
				}
			}
			return false;
		}

		void* HeapManager::ReturnFirstPointer()
		{
			return pFirstPointerToHeap;
		}

		HeapManager::~HeapManager()
		{
			destroy();
		}
	}
}