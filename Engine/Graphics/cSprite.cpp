#include "cSprite.h"
#include "../Platform/Platform.h"

namespace
{
	eae6320::Graphics::VertexFormats::sMesh* m_TempMesh;
	uint8_t m_NumberOfVertices = 4;
}

eae6320::cResult eae6320::Graphics::cSprite::Load(const std::string& i_FilePath, cSprite*& o_Sprite)
{
	cResult result = Results::Success;
	cMesh* outputSprite = nullptr;
	if (!(result = LoadFile(i_FilePath.c_str())))
	{
		return result;
	}
	if (!(CreateSprite(m_TempMesh, static_cast<unsigned int>(m_NumberOfVertices), outputSprite)))
	{
		return result;
	}
	o_Sprite = outputSprite;
	return result;
}
