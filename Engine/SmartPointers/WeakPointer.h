#pragma once

#include "SharedPointer.h"

template<class T>
class WeakPtr
{
public:
	//Constructor
	WeakPtr(T* i_ptr) :
		m_ClassPointer(i_ptr), m_sharedPointerReferenceCount(0), m_weakPointerReferenceCount(new long(1))
	{
	}

	//Destructor
	~WeakPtr()
	{
		ReleaseWeakPointer();
	}

	WeakPtr() :
		m_ClassPointer(nullptr), m_sharedPointerReferenceCount(0), m_weakPointerReferenceCount(0)
	{
	}

	inline WeakPtr* operator = (const WeakPtr* i_Other)
	{
		if (i_Other == nullptr)
		{
			return nullptr;
		}
		return *this;
	}

	inline SharedPtr<T>* GetSharedPointer()
	{
		if (m_ClassPointer == nullptr)
		{
			return nullptr;
		}
		else
		{
			SharedPtr<T> sharedPointer;
			sharedPointer.m_ClassPointer = m_ClassPointer;
			sharedPointer.m_ReferenceCount = m_sharedPointerReferenceCount;
			sharedPointer.m_weakPointerReferenceCount = m_weakPointerReferenceCount;
			m_sharedPointerReferenceCount++;
			return sharedPointer;
		}
	}

	inline bool operator == (const WeakPtr<T>& i_rhs)
	{
		if (this->m_ClassPointer == i_rhs->m_ClassPointer)
			return true;
		else
			return false;
	}

	inline bool operator != (const WeakPtr<T>& i_rhs)
	{
		if (this->m_ClassPointer != i_rhs->m_ClassPointer)
			return true;
		else
			return false;
	}

private:
	T* m_ClassPointer;
	long* m_weakPointerReferenceCount;
	long* m_sharedPointerReferenceCount;

	void ReleaseWeakPointer()
	{
		if ((--(*m_weakPointerReferenceCount) == 0) && (m_sharedPointerReferenceCount == 0))
		{
			delete m_sharedPointerReferenceCount;
			delete m_weakPointerReferenceCount;
		}
		m_ClassPointer = nullptr;
		m_sharedPointerReferenceCount = nullptr;
		m_weakPointerReferenceCount = nullptr;
	}
};
