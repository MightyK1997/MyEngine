#include "Manager.h"

template<class tAsset, typename tKey>
HRESULT Handle::Manager<tAsset, tKey>::Load(const tKey& i_Key, Handle<tAsset>& o_Handle)
{
	HRESULT hr = S_OK;
	auto iterator = m_ListOfAllHandles.find(i_Key);
	if (iterator != m_ListOfAllHandles.end())
	{
		auto existingHandle = iterator->second();
		auto existingIndex = existingHandle.GetIndex();
		if (existingIndex < m_ListOfAllHandles.size())
		{
			auto& assetRecord = m_AllAssets[existingIndex];
			if (assetRecord.id == existingIndex)
			{
				assetRecord.m_ReferenceCount += 1;
				o_Handle = existingHandle;
				return S_OK;
			}
			else
			{
				return ERROR;
			}
		}
	}

	//New Record
	tAsset* newAsset = nullptr;

	return hr;
}
