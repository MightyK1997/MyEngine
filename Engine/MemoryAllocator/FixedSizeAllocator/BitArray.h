#pragma once

#if _WIN64
#define BITS_PER_BYTE 8;
#define BIT_WIDTH 64;
#elif _WIN32
#define BITS_PER_BYTE 8;
#define BIT_WIDTH 32;
#endif
#include <stdint.h>
#include "..\BlockAllocator\BlockAllocator.h"

namespace Engine
{
	namespace MemoryAllocators
	{
		class BitArray
		{
		public:
			BitArray(size_t numberOfBits, HeapManager* i_pAllocator, bool i_startClear = true);
			~BitArray();

			//sets all bits to 0
			void ClearAll(void);

			//sets all bits to 1
			void SetAll(void);

			//gets the first set bit
			bool FindFirstSetBit(size_t & o_bitNumber) const;

			//gets the first clear bit
			bool FindFirstClearBit(size_t & o_bitNumber) const;

			//Check if bit is set.
			inline bool IsBitSet(size_t i_bitNumber) const;

			//Check if Bit is not Set
			inline bool IsBitClear(size_t i_bitNumber) const;

			//changes the bit to 1
			void SetBit(size_t i_bitNumber);

			//changes the bit to 0
			void ClearBit(size_t i_bitNumber);
		private:
#if _WIN64
			uint64_t* byteArray;
#elif _WIN32
			uint32_t* byteArray;
#endif
			size_t numberOfBytes;
			HeapManager* m_Heap;
		};
	}
}