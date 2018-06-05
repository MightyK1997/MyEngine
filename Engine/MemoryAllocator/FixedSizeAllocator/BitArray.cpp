#include "BitArray.h"
#include <cassert>
#include <string>
#include <intrin.h>
#include <new>
#include "..\AllocatingAllocators.h"
#include "..\OverloadingNewAndDelete.h"



namespace Engine
{
	namespace MemoryAllocators
	{
		AllocatingAllocators* tp;
		BitArray::BitArray(size_t numberOfBits, HeapManager* i_pHeapManager, bool i_startClear)
		{
			m_Heap = i_pHeapManager;
			assert(numberOfBits);
			numberOfBytes = numberOfBits / BITS_PER_BYTE;
#if _WIN64
			byteArray = new(m_Heap) uint64_t[numberOfBytes];
#elif _WIN32
			byteArray = new(m_Heap) uint32_t[numberOfBytes];
#endif
			assert(byteArray);
			memset(byteArray, i_startClear ? 0 : 1, numberOfBytes);
		}

		BitArray::~BitArray()
		{
			m_Heap->_free(byteArray);
		}

		bool BitArray::FindFirstSetBit(size_t & o_bitNumber) const
		{
			size_t index = 0;
			unsigned long bitScanIndex;
			unsigned char isNonZero = NULL;
#if _WIN64
			while (byteArray[index] == 0x00000 && index < (numberOfBytes))
			{
				index ++;
			}
#elif _WIN32
			while (byteArray[index] == 0x0000 && index < (numberOfBytes))
			{
				index += 8;
			}
#endif

#if _WIN64
			isNonZero = _BitScanForward64(&bitScanIndex, byteArray[index]);
#elif _WIN32
			isNonZero = _BitScanForward(&bitScanIndex, (uint32_t)byteArray[index]);
#endif
			if (isNonZero && index < numberOfBytes)
			{
				auto temp = index * BITS_PER_BYTE;
				o_bitNumber = temp + bitScanIndex;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool BitArray::FindFirstClearBit(size_t & o_bitNumber) const
		{
			size_t m_bitNumber;
			size_t index = 0;
			auto tempArray = byteArray;
			while (index < numberOfBytes)
			{
				byteArray[index] ^= byteArray[index];
				index++;
			}
			if (FindFirstSetBit(m_bitNumber))
			{
				while (index < numberOfBytes)
				{
					byteArray[index] ^= byteArray[index];
					index++;
				}
				o_bitNumber = m_bitNumber;
				return true;
			}
			else
			{
				while (index < numberOfBytes)
				{
					byteArray[index] ^= byteArray[index];
					index++;
				}
				return false;
			}
		}

		void BitArray::SetBit(size_t i_bitNumber) {
			size_t numberOfBits = numberOfBytes * BITS_PER_BYTE;
			assert(i_bitNumber >= 0 && i_bitNumber <= numberOfBits);
			size_t arrayInd = i_bitNumber / BITS_PER_BYTE;
			size_t offset = i_bitNumber % BITS_PER_BYTE;
#if _WIN64 || _WIN32
			byteArray[arrayInd] |= (1i64 << offset);
			//#elif _WIN32
			//	byteArray[arrayInd] |= (1 << offset);
#endif
		}

		void BitArray::ClearBit(size_t i_bitNumber) {
			size_t numberOfBits = numberOfBytes * BITS_PER_BYTE;
			assert(i_bitNumber >= 0 && i_bitNumber <= numberOfBits);
			size_t arrayInd = i_bitNumber / BITS_PER_BYTE;
			size_t offset = i_bitNumber % BITS_PER_BYTE;
#if _WIN64
			byteArray[arrayInd] &= ~(1i64 << offset);
#elif _WIN32
			byteArray[arrayInd] &= ~(1 << offset);
#endif
		}

		void BitArray::ClearAll(void)
		{
			memset(byteArray, 0, numberOfBytes);
		}

		void BitArray::SetAll(void)
		{
			memset(byteArray, 1, numberOfBytes);
		}

		bool BitArray::IsBitSet(size_t i_bitNumber) const
		{
			size_t numberOfBits = numberOfBytes * BITS_PER_BYTE;
			assert(i_bitNumber >= 0 && (i_bitNumber <= numberOfBits));
			size_t m_arrayNumber = i_bitNumber / BITS_PER_BYTE;
			size_t m_indexNumber = i_bitNumber % BITS_PER_BYTE;
#if _WIN64 || _WIN32
			if ((byteArray[m_arrayNumber] & (1i64 << m_indexNumber)) == 0)
				return true;
#elif _WIN32
			if ((byteArray[m_arrayNumber] & (1 << m_indexNumber)) == 0)
				return true;
#endif
			return false;
		}

		bool BitArray::IsBitClear(size_t i_bitNumber) const
		{
			size_t numberOfBits = numberOfBytes * BITS_PER_BYTE;
			assert(i_bitNumber >= 0 && (i_bitNumber <= numberOfBits));
			size_t m_arrayNumber = i_bitNumber / BITS_PER_BYTE;
			size_t m_indexNumber = i_bitNumber % BITS_PER_BYTE;
#if _WIN64 || _WIN32
			if ((byteArray[m_arrayNumber] & (1i64 << m_indexNumber)) == 1)
				return true;
#elif _WIN32
			if ((byteArray[m_arrayNumber] & (1i64 << m_indexNumber)) == 1)
				return true;
#endif
			return false;
		}
	}
}