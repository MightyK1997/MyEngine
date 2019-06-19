#pragma once


#include "Handle.h"
#include <string>
#include <map>
#include <Windows.h>
#include <vector>


namespace Handle
{
	template<class tAsset, typename tKey = std::string>
	class Manager
	{
	public:
		HRESULT Load(const tKey& i_Key, Handle<tAsset>& o_Handle);
		HRESULT Release(Handle<tAsset>*& io_Handle);


	private:
		struct AssetData
		{
			tAsset* m_Asset = nullptr;
			uint64_t id;
			uint32_t m_ReferenceCount;

			AssetData(tAsset* m_Asset, uint64_t id, uint32_t m_ReferenceCount) : m_Asset(m_Asset), id(id), m_ReferenceCount(m_ReferenceCount) {}
		};

		std::vector<AssetData> m_AllAssets;
		std::map<tKey, Handle<tAsset>> m_ListOfAllHandles;
	};
}