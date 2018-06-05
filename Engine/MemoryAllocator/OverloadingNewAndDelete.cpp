#include "OverloadingNewAndDelete.h"
#include "..\EngineInit.h"
#include <cassert>



void* operator new(size_t i_size)
{
	void* m_returnPointer = nullptr;
	Engine::MemoryAllocators::AllocatingAllocators* m_allAllocators = Engine::GetAllocator();
	//Engine::MemoryAllocators::FixedSizeAllocator* m_pFSA = reinterpret_cast<Engine::MemoryAllocators::FixedSizeAllocator*>(m_allAllocators->GetFixedSizedAllocator(i_size));
	//if (m_pFSA)
	//{
	//	m_returnPointer = m_pFSA->_alloc();
	//}
	if (!m_returnPointer)
	{
		Engine::MemoryAllocators::HeapManager* m_BlockAllocator = reinterpret_cast<Engine::MemoryAllocators::HeapManager*>(m_allAllocators->GetBlockAllocator());
		m_returnPointer = m_BlockAllocator->_alloc(i_size);
	}
	return m_returnPointer;
}

void * operator new(size_t size, Engine::MemoryAllocators::AllocatingAllocators* i_pManager)
{
	assert(i_pManager);
	return i_pManager->_alloc(size);
}

void * operator new(size_t size, Engine::MemoryAllocators::HeapManager* i_pManager)
{
	assert(i_pManager);
	return i_pManager->_alloc(size);
}

void * operator new(size_t size, Engine::MemoryAllocators::AllocatingAllocators * i_pManager, unsigned int alignment)
{
	assert(i_pManager);
	return i_pManager->_alloc(size, alignment);
}
void * operator new(size_t size, Engine::MemoryAllocators::HeapManager * i_pManager, unsigned int alignment)
{
	assert(i_pManager);
	return i_pManager->_alloc(size, alignment);
}

void * operator new[](size_t size, Engine::MemoryAllocators::AllocatingAllocators* i_pManager)
{
	assert(i_pManager);
	return i_pManager->_alloc(size);
}

void * operator new[](size_t size, Engine::MemoryAllocators::HeapManager* i_pManager)
{
	assert(i_pManager);
	return i_pManager->_alloc(size);
}

void operator delete(void * i_fMemory)
{
	assert(i_fMemory);
	Engine::MemoryAllocators::AllocatingAllocators* m_allAllocators = Engine::GetAllocator();
	m_allAllocators->_free(i_fMemory);
}
void operator delete[](void * i_fMemory)
{
	assert(i_fMemory);
	Engine::MemoryAllocators::AllocatingAllocators* m_allAllocators = Engine::GetAllocator();
	m_allAllocators->_free(i_fMemory);
}


void operator delete(void * i_fMemory, Engine::MemoryAllocators::AllocatingAllocators* i_pMananger)
{
	assert(i_fMemory);
	i_pMananger->_free(i_fMemory);
}
void operator delete[](void * i_fMemory, Engine::MemoryAllocators::AllocatingAllocators* i_pMananger)
{
	assert(i_fMemory);
	i_pMananger->_free(i_fMemory);
}

void operator delete(void * i_fMemory, Engine::MemoryAllocators::HeapManager* i_pMananger)
{
	assert(i_fMemory);
	i_pMananger->_free(i_fMemory);
}
void operator delete[](void * i_fMemory, Engine::MemoryAllocators::HeapManager* i_pMananger)
{
	assert(i_fMemory);
	i_pMananger->_free(i_fMemory);
}