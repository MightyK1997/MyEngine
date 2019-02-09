#pragma once

#include "BlockAllocator.h"

namespace Engine
{
	namespace MemoryAllocators
	{
		class FixedSizeAllocator
		{
		public:
			FixedSizeAllocator(BlockAllocator* p_BlockAllocator, size_t blockSize = 8);
			~FixedSizeAllocator();
		private:
			void* p_FirstPointerToFSA;
		};
	}
}
