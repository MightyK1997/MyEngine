#include "HeapManagerProxy.h"
#include "..\NewEngine\MemoryAllocators\BlockAllocator.h"
#include <Windows.h>
#include "..\NewEngine\Math\Vector\Vector3.h"
#include "..\NewEngine\Math\Vector\Vector4.h"

#include "../NewEngine/Audio/Sound.h"

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
using namespace Math;
using namespace Vector;

int main()
{
	CoInitialize(nullptr);
	Sound::Sound sound;
	//sound.Play("C:/Windows/media/Ring05.wav");
	sound.AddToDefaultQueue("C:/Windows/media/Ring05.wav");
	sound.AddToDefaultQueue("C:/Windows/media/Ring04.wav");
	_getch();
}