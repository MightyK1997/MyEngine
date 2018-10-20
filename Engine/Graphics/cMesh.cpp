#include "cMesh.h"
#include <External/Lua/Includes.h>

#include "../Platform/Platform.h"

eae6320::Assets::cManager<eae6320::Graphics::cMesh> eae6320::Graphics::cMesh::s_Manager;

namespace
{
	eae6320::Graphics::VertexFormats::sMesh* m_TempMesh;
	eae6320::Graphics::VertexFormats::sIndex* m_TempIndex;
	uint16_t m_NumberOfVertices;
	uint16_t m_NumberOfIndices;
}

eae6320::cResult LoadFile(const char* const i_FileName) 
{
	auto result = eae6320::Results::Success;
	eae6320::Platform::sDataFromFile dataFromFile;
	std::string errorMessage;
	result = eae6320::Platform::LoadBinaryFile(i_FileName, dataFromFile, &errorMessage);
	uintptr_t offset = reinterpret_cast<uintptr_t>(dataFromFile.data);
	const uintptr_t finalOffset = offset + dataFromFile.size;
	m_NumberOfIndices = *reinterpret_cast<uint16_t*>(offset);
	offset += sizeof(m_NumberOfIndices);
	m_NumberOfVertices = *reinterpret_cast<uint16_t*>(offset);
	offset += sizeof(m_NumberOfVertices);
	m_TempIndex = reinterpret_cast<eae6320::Graphics::VertexFormats::sIndex*>(offset);
	offset += (sizeof(eae6320::Graphics::VertexFormats::sIndex) * m_NumberOfIndices);
	m_TempMesh = reinterpret_cast<eae6320::Graphics::VertexFormats::sMesh*>(offset);
	return result;
}

//Manager load Function
eae6320::cResult eae6320::Graphics::cMesh::Load(const char * const i_FilePath, cMesh *& o_Mesh)
{
	cResult result = Results::Success;
	cMesh* outputMesh = nullptr;
	if (!(result = LoadFile(i_FilePath)))
	{
		return result;
	}
	if (!(CreateMesh(m_TempMesh, m_TempIndex, static_cast<unsigned int>(m_NumberOfIndices), static_cast<unsigned int>(m_NumberOfVertices), outputMesh)))
	{
		return result;
	}
	o_Mesh = outputMesh;
	return result;
}