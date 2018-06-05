#pragma once


namespace Engine
{
	template<class T>
	class SharedPtr
	{
	public:

		//Default Constructor
		SharedPtr() :
			m_ClassPointer(nullptr), m_ReferenceCount(0), m_weakPointerReferenceCount(0)
		{
		}
		//Constructor
		SharedPtr(T* i_ptr) :
			m_ClassPointer(i_ptr), m_ReferenceCount(new long(1)), m_weakPointerReferenceCount(0)
		{
		}

		//Copy Constructor
		SharedPtr(const T& i_CopyPtr) :
			m_ClassPointer(i_CopyPtr.m_ClassPointer), m_ReferenceCount(i_CopyPtr.m_ReferenceCount), m_weakPointerReferenceCount(i_CopyPtr.m_weakPointerReferenceCount)
		{
			(*m_ReferenceCount)++;
		}
		//Destructor
		~SharedPtr()
		{
			ReleaseCurrentReference();
		}
		//Overloading Operators.
		inline T* operator->()
		{
			return m_ClassPointer;
		}

		inline T* get()
		{
			return m_ClassPointer;
		}

		inline T& operator*()
		{
			assert(m_ClassPointer != nullptr);
			return *m_ClassPointer;
		}

		inline SharedPtr * operator = (const SharedPtr * i_Other)
		{
			if (i_Other == nullptr)
			{
				ReleaseCurrentReference();
				return nullptr;
			}
			else
			{
				ReleaseCurrentReference();
				AcquireNewReference(i_Other);
			}
			return *this;
		}

		inline bool operator == (const SharedPtr<T>& i_rhs)
		{
			if (this->m_ClassPointer == i_rhs->m_ClassPointer)
				return true;
			else
				return false;
		}

		inline bool operator != (const SharedPtr<T>& i_rhs)
		{
			if (this->m_ClassPointer != i_rhs->m_ClassPointer)
				return true;
			else
				return false;
		}


	private:
		T* m_ClassPointer;
		long* m_ReferenceCount;
		long* m_weakPointerReferenceCount;

		void ReleaseCurrentReference()
		{
			if (--(*m_ReferenceCount) == 0)
			{
				delete m_ReferenceCount;
				delete m_ClassPointer;
				delete m_weakPointerReferenceCount;
			}
			m_ClassPointer = nullptr;
			m_ReferenceCount = nullptr;
			m_weakPointerReferenceCount = nullptr;
		}
		void AcquireNewReference(SharedPtr& i_Other)
		{
			this->m_ClassPointer = i_Other.m_ClassPointer;
			this->m_ReferenceCount = i_Other.m_ReferenceCount;
			(*this->m_ReferenceCount)++;
		}
	};
}