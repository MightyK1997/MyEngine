#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <bitset>
#include "..\NewEngine\MemoryAllocators\BlockAllocator.h"

using namespace std;



void main()
{

	 int f = -156;


	 void* BlockAllocatorBegin = _aligned_malloc(2048, 4);

	 BlockAllocator* ba = new BlockAllocator(2048, BlockAllocatorBegin);

	 cout << ba;

	/*cout.write(reinterpret_cast<const char*>(&f), sizeof(f));*/

	//std::cout << std::bitset<8>(f) << std::endl;

	//f = -f;
	//std::cout << std::bitset<8>(f) << std::endl;
	_getch();
}

