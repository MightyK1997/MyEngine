#pragma once

#include "AllocatingAllocators.h"


void * operator new(size_t size, Engine::MemoryAllocators::AllocatingAllocators * i_pManager);
void * operator new(size_t size, Engine::MemoryAllocators::HeapManager* i_pManager);
void * operator new(size_t size, Engine::MemoryAllocators::AllocatingAllocators * i_pManager, unsigned int alignment);
void * operator new(size_t size, Engine::MemoryAllocators::HeapManager * i_pManager, unsigned int alignment);
void * operator new[](size_t size, Engine::MemoryAllocators::AllocatingAllocators* i_pManager);
void * operator new[](size_t size, Engine::MemoryAllocators::HeapManager* i_pManager);
void operator delete(void * i_fMemory);
void operator delete[](void * i_fMemory);
void operator delete(void * i_fMemory, Engine::MemoryAllocators::AllocatingAllocators* i_pMananger);
void operator delete[](void * i_fMemory, Engine::MemoryAllocators::AllocatingAllocators* i_pMananger);
void operator delete(void * i_fMemory, Engine::MemoryAllocators::HeapManager* i_pMananger);
void operator delete[](void * i_fMemory, Engine::MemoryAllocators::HeapManager* i_pMananger);
void* operator new(size_t i_size);