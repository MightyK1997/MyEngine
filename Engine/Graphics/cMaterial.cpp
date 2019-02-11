#include "cMaterial.h"
#include "cEffect.h"

eae6320::Assets::cManager<eae6320::Graphics::cMaterial> eae6320::Graphics::cMaterial::s_Manager;
namespace
{
	eae6320::Graphics::cEffect* m_TempEffect;
	std::string m_EffectLocation;
	uint8_t m_ConstantType;
	std::string m_ConstantName;
	std::vector<uint8_t> m_ConstantData;
	std::string m_TextureLocation;
}


eae6320::cResult LoadDataFile(const char* const i_FileName)
{
	auto result = eae6320::Results::Success;
	eae6320::Platform::sDataFromFile dataFromFile;
	std::string errorMessage;
	result = eae6320::Platform::LoadBinaryFile(i_FileName, dataFromFile, &errorMessage);
	uintptr_t offset = reinterpret_cast<uintptr_t>(dataFromFile.data);
	const uintptr_t finalOffset = offset + dataFromFile.size;
	m_EffectLocation = reinterpret_cast<char*>(offset);
	offset += m_EffectLocation.size() + 1;
	m_ConstantType = *reinterpret_cast<uint8_t*>(offset);
	offset += sizeof(uint8_t);
	m_ConstantName = reinterpret_cast<char*>(offset);
	offset += m_ConstantName.size() +1;
	for (size_t i = 0; i < m_ConstantType + 1; i++)
	{
		m_ConstantData.push_back(*reinterpret_cast<uint8_t*>(offset));
		offset += sizeof(uint8_t);
	}
	m_TextureLocation = reinterpret_cast<char*>(offset);
	result = eae6320::Graphics::cEffect::Load(m_EffectLocation, m_TempEffect);
	return result;
}


eae6320::cResult eae6320::Graphics::cMaterial::Load(const std::string& i_FilePath, cMaterial *& o_Material)
{
	cResult result = Results::Success;

	cMaterial* outputMaterial = new cMaterial();

	if (!(result = LoadDataFile(i_FilePath.c_str())))
	{
		return result;
	}
	if (m_TempEffect)
	{
		outputMaterial->m_Effect = m_TempEffect;
		outputMaterial->m_Color = { static_cast<float>(m_ConstantData[0]),
			static_cast<float>(m_ConstantData[1]),
			static_cast<float>(m_ConstantData[2]),
			static_cast<float>(m_ConstantData[3]) };
	}

	o_Material = outputMaterial;
	return result;
}