#include "HeapManagerProxy.h"
#include "..\NewEngine\MemoryAllocators\BlockAllocator.h"
#include <Windows.h>
#include "..\NewEngine\Math\Vector\Vector3.h"
#include "..\NewEngine\Math\Vector\Vector4.h"

#include <assert.h>
#include <algorithm>
#include <vector>
#include <conio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>

#define SUPPORTS_ALIGNMENT
#define SUPPORTS_SHOWFREEBLOCKS
#define SUPPORTS_SHOWOUTSTANDINGALLOCATIONS
#define _CRTDBG_MAP_ALLOC

using namespace Engine;
using namespace std;
using namespace Math;
using namespace Vector;

int main()
{
	//HeapManager_UnitTest();
	Vector3* someVec = new Vector3(0,0,0);
	someVec[0]= 1000.f;
	cout << someVec->x();
#ifdef _CRTDBG_MAP_ALLOC
	_CrtDumpMemoryLeaks();
#endif // _CRT
	_getch();
}