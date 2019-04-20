#include "cMaterial.h"
#include "cEffect.h"

eae6320::Assets::cManager<eae6320::Graphics::cMaterial> eae6320::Graphics::cMaterial::s_Manager;
namespace
{
	eae6320::Graphics::cEffect::Handle m_TempHandle;
	eae6320::Graphics::cTexture::Handle m_TempTextureHandle;
	eae6320::Graphics::cTexture::Handle m_TempNormalHandle;
	eae6320::Graphics::cTexture::Handle m_TempGlossHandle;
	std::string m_EffectLocation;
	uint8_t m_ConstantType;
	uint8_t m_ConstantVariant;
	eae6320::Graphics::sColor m_ConstantData;
	std::string m_TextureLocation;
	std::string m_NormalLocation;
	std::string m_GlossLocation;
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
	m_ConstantVariant = *reinterpret_cast<uint8_t*>(offset);
	offset += sizeof(uint8_t);
	m_ConstantData = *reinterpret_cast<eae6320::Graphics::sColor*>(offset);
	offset += sizeof(eae6320::Graphics::sColor);
	m_TextureLocation = reinterpret_cast<char*>(offset);
	offset += m_TextureLocation.size() + 1;
	m_NormalLocation = reinterpret_cast<char*>(offset);
	offset += m_NormalLocation.size() + 1;
	m_GlossLocation = reinterpret_cast<char*>(offset);
	result = eae6320::Graphics::cEffect::s_Manager.Load(m_EffectLocation, m_TempHandle);
	result = eae6320::Graphics::cTexture::s_manager.Load(m_TextureLocation, m_TempTextureHandle);
	result = eae6320::Graphics::cTexture::s_manager.Load(m_NormalLocation, m_TempNormalHandle);
	result = eae6320::Graphics::cTexture::s_manager.Load(m_GlossLocation, m_TempGlossHandle);
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
	outputMaterial->m_EffectHandle = m_TempHandle;
	outputMaterial->m_TextureHandle = m_TempTextureHandle;
	outputMaterial->m_NormalMapHandle = m_TempNormalHandle;
	outputMaterial->m_GlossMapHandle = m_TempGlossHandle;
	outputMaterial->m_Color = m_ConstantData;

	o_Material = outputMaterial;
	return result;
}

void eae6320::Graphics::cMaterial::Shutdown()
{
	eae6320::Graphics::cEffect::s_Manager.Release(m_EffectHandle);
	eae6320::Graphics::cTexture::s_manager.Release(m_TextureHandle);
	eae6320::Graphics::cTexture::s_manager.Release(m_NormalMapHandle);
	eae6320::Graphics::cTexture::s_manager.Release(m_GlossMapHandle);
}
