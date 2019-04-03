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
				E_DRAWSPRITE = 0x04,
			};

			enum class BitMasksForRenderCommands : uint8_t
			{
				E_COMMONBITMASK = 7,
				E_TYPEBITMASK = 4,
			};

			enum class BitShiftsForCommonRenderCommands : uint8_t
			{
				E_TYPESHIFT = 16,
				E_MESHSHIFT = 8,
			};

			enum class BitShiftsForIndependentRenderCommands :uint64_t
			{
				E_EFFECTSHIFT = 56,
				E_MATERIALSHIFT = 48,
				E_DEPTHSHIFT = 40,
			};

			enum class BitShiftsForDependentRenderCommands :uint64_t
			{
				E_EFFECTSHIFT = 48,
				E_MATERIALSHIFT = 40,
				E_DEPTHSHIFT = 56,
			};
		}
	}
}
