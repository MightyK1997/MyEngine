#include "FixedSizeAllocator.h"


namespace Engine
{
	namespace MemoryAllocators
	{
		FixedSizeAllocator::FixedSizeAllocator(BlockAllocator* i_pBlockAllocator, size_t i_BlockSize)
		{
			size_t numberOfBlocks = 2048;
			size_t totalFSASize = numberOfBlocks * (i_BlockSize + 16);
			p_FirstPointerToFSA = static_cast<uint8_t*>(i_pBlockAllocator->_alloc(totalFSASize, 64));
		}
	}
}