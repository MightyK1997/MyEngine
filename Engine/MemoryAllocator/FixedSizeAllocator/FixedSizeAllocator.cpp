#include "FixedSizeAllocator.h"
#include <malloc.h>
#include<cassert>
#include <stdio.h>
#include <iostream>
#include <new>
#include "..\OverloadingNewAndDelete.h"


using namespace std;

namespace Engine
{
	namespace MemoryAllocators
	{
		FixedSizeAllocator::FixedSizeAllocator(FixedSizeAllocator* i_pFirst, HeapManager* i_mHeapManager, size_t i_blockSize) : pFirstPointer(i_pFirst), allocBlockSize(i_blockSize), mHeapManager(i_mHeapManager)
		{
			size_t mNumberOfBlocks = 2048;
			totalBlockSize = i_blockSize + 2 * GUARD_BAND_SIZE;
			size_t sizeFSA = mNumberOfBlocks * totalBlockSize;
			pFirstAllocationPointer = reinterpret_cast<uint8_t*>(i_mHeapManager->_alloc(sizeFSA, 64));
			pLastPointerToHeap = reinterpret_cast<uint8_t*>(pFirstAllocationPointer) + (mNumberOfBlocks * totalBlockSize);
			myBitArray = new(mHeapManager) BitArray(mNumberOfBlocks, mHeapManager, false);
		}

		FixedSizeAllocator::~FixedSizeAllocator()
		{
			//TODO

			//Need to add better destructor.
			myBitArray->~BitArray();
			mHeapManager->_free(pFirstAllocationPointer);
		}

		void* FixedSizeAllocator::_alloc(unsigned int i_alignment)
		{
			size_t mBitNumber;
			//adding this because compiler giving errors when using macros inside "()"
			size_t mChangeSize = GUARD_BAND_SIZE;

			//cout << "New allocation" << endl;
			if (myBitArray->FindFirstSetBit(mBitNumber))
			{
				myBitArray->ClearBit(mBitNumber);
				void* mReturnPointer = reinterpret_cast<uint8_t*>(pFirstAllocationPointer) + (mBitNumber * (totalBlockSize)+mChangeSize);
				auto padding = i_alignment - (reinterpret_cast<uintptr_t>(mReturnPointer) % i_alignment);
				mReturnPointer = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(mReturnPointer) + padding);
				return mReturnPointer;
			}
			return nullptr;
		}

		void FixedSizeAllocator::_free(void* i_pPointer)
		{
			size_t m_gbSize = GUARD_BAND_SIZE;
			assert(i_pPointer);
			if (IsContained(i_pPointer))
			{
				//cout << "Can Free" << endl;
				size_t mIndexFromTop = ((reinterpret_cast<uint8_t*>(i_pPointer) - m_gbSize) - reinterpret_cast<uint8_t*>(pFirstAllocationPointer)) / totalBlockSize;
				myBitArray->SetBit(mIndexFromTop);
			}
		}

		bool FixedSizeAllocator::IsContained(void* i_pPointer)
		{
			//assert(i_pPointer);
			if ((i_pPointer >= pFirstAllocationPointer) && (i_pPointer < pLastPointerToHeap))
				return true;
			return false;
		}

		bool FixedSizeAllocator::IsFree(void* i_pPointer)
		{
			assert(i_pPointer);
			//TODo

			//A
			return true;
		}

		void* FixedSizeAllocator::ReturnFirstPointer()
		{
			return pFirstPointer;
		}

		void FixedSizeAllocator::SetFirstPointer(FixedSizeAllocator* i_pPointer)
		{
			pFirstPointer = i_pPointer;
		}
	}
}