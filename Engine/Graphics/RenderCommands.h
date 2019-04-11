#pragma once
#include <cstdint>


namespace eae6320
{
	namespace Graphics
	{
		namespace RenderCommands
		{
			enum class RenderCommandTypes : uint8_t
			{
				E_DRAWINDEPENDENT = 0x01,
				E_DRAWDEPENDENT = 0X02,
			};

			enum class BitMasksForRenderCommands : uint8_t
			{
				E_COMMONBITMASK = 7,
			};

			enum class BitShiftsForRenderCommands :uint64_t
			{
				E_EFFECTSHIFT = 56,
				E_MATERIALSHIFT = 48,
				E_DEPTHSHIFT = 40,
				E_TYPESHIFT = 13,
				E_MESHSHIFT = 7,
			};
		}
	}
}
