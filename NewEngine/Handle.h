#pragma once
#include <cstdint>
#include <limits>

#define INVALID_INDEX std::numeric_limits<uint64_t>::max();

namespace Handle
{
	template<class tAsset, typename tKey>
	class Manager;
}

namespace Handle
{
	template<typename tAsset>
	class Handle
	{
	public:
		Handle() = default;

	private:
		uint64_t m_Value = INVALID_INDEX;
		uint64_t GetIndex() const { return m_Value; }
		void MakeInvalid() { m_Value = INVALID_INDEX; }
		Handle(const uint64_t i_Index):m_Value(i_Index){}
		uint64_t IncrementIndex(uint64_t i_Index) { return i_Index++; }

		template<class tAsset, typename tKey> friend class Manager;
	};
}
