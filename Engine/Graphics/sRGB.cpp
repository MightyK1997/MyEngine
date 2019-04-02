#include "cmath"

#include "sRGB.h"

eae6320::Graphics::sColor eae6320::Graphics::Color::ConvertNormalizedsRGBToLinear(Graphics::sColor i_Color)
{
	Graphics::sColor returnColor;
	returnColor.r = i_Color.r < 0.04045f ? i_Color.r / 12.92f : pow(((i_Color.r + 0.055f) / 1.055f), 2.4f);
	returnColor.g = i_Color.g < 0.04045f ? i_Color.g / 12.92f : pow(((i_Color.g + 0.055f) / 1.055f), 2.4f);
	returnColor.b = i_Color.b < 0.04045f ? i_Color.b / 12.92f : pow(((i_Color.b + 0.055f) / 1.055f), 2.4f);
	returnColor.alpha = i_Color.alpha;
	return returnColor;
}

eae6320::Graphics::sColor eae6320::Graphics::Color::ConvertNormalizedLinearTosRGB(Graphics::sColor i_Color)
{
	Graphics::sColor returnColor;
	returnColor.r = (i_Color.r < 0.0031308f) ? (12.92f * i_Color.r) : ((pow(i_Color.r, 1 / 2.4f) * 1.055f) - 0.055f);
	returnColor.g = (i_Color.g < 0.0031308f) ? (12.92f * i_Color.g) : ((pow(i_Color.g, 1 / 2.4f) * 1.055f) - 0.055f);
	returnColor.b = (i_Color.b < 0.0031308f) ? (12.92f * i_Color.b) : ((pow(i_Color.b, 1 / 2.4f) * 1.055f) - 0.055f);
	returnColor.alpha = i_Color.alpha;
	return returnColor;
}