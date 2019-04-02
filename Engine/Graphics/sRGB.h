#pragma once

#include "ConstantBufferFormats.h"


namespace eae6320
{
	namespace Graphics
	{
		namespace Color
		{
			Graphics::sColor ConvertNormalizedsRGBToLinear(Graphics::sColor i_Color);
			Graphics::sColor ConvertNormalizedLinearTosRGB(Graphics::sColor i_Color);
		}
	}
}