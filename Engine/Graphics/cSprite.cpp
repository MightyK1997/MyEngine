#include "cSprite.h"
#include "../Platform/Platform.h"

eae6320::Assets::cManager<eae6320::Graphics::cSprite> eae6320::Graphics::cSprite::s_Manager;

eae6320::cResult eae6320::Graphics::cSprite::Load(const std::string& i_FilePath, cSprite*& o_Sprite)
{
	cResult result = Results::Success;

	VertexFormats::sSprite spriteCorners[4];
	{
		spriteCorners[0].x = -1;
		spriteCorners[0].y = -1;
		spriteCorners[0].x = 0;
		spriteCorners[0].y = -1;
		spriteCorners[0].x = -1;
		spriteCorners[0].y = 0;
		spriteCorners[0].x = 0;
		spriteCorners[0].y = 0;
	}

	cSprite* outputSprite = nullptr;
	if (!(CreateSprite(spriteCorners, 4, outputSprite)))
	{
		return result;
	}
	o_Sprite = outputSprite;
	return result;
}