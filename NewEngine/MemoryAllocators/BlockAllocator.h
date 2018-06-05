#pragma once




struct BlockDestriptor
{
	void* p_ReturnPointer;
	size_t m_SizeOfBlock;
	bool b_IsAllocated;
};