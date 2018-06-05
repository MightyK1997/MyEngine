#include "SharedPointer.h"


namespace Engine
{
	template<class T>
	inline bool operator== (const SharedPtr<T>& i_lhsPointer, const SharedPtr<T>& i_rhsPointer)
	{
		if (i_lhsPointer->m_ClassPointer == i_rhsPointer->m_ClassPointer)
			return true;
		else
			return false;
	}

	template<class T>
	inline bool operator!= (const SharedPtr<T>& i_lhsPointer, const SharedPtr<T>& i_rhsPointer)
	{
		if (i_lhsPointer->m_ClassPointer != i_rhsPointer->m_ClassPointer)
			return true;
		else
			return false;
	}

	template<class T, class U>
	inline SharedPtr<T>* reinterpret_pointer_cast(const SharedPtr<U>& _Other) noexcept
	{
		auto p = reinterpret_cast<typename SharedPtr<T>::m_ClassPointer*>(_Other.get());
		return SharedPtr(_Other);
	}

	template<class T, class U>
	inline SharedPtr<T>* static_pointer_cast(const SharedPtr<U>& _Other) noexcept
	{
		auto p = static_cast<typename SharedPtr<T>::m_ClassPointer*>(_Other.get());
		return SharedPtr(_Other);
	}

	template<class T, class U>
	inline SharedPtr<T>* const_pointer_cast(const SharedPtr<U>& _Other) noexcept
	{
		auto p = const_cast<typename SharedPtr<T>::m_ClassPointer*>(_Other.get());
		return SharedPtr(_Other);
	}
}