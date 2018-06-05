//#include <assert.h>
//#include <stdio.h>
//#include <stdint.h>
//#include <Windows.h>
//
//#if defined _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#endif // _DEBUG
//
//#include "..\Engine\Renderer\Renderer.h"
//
//using namespace Engine;
//using namespace Renderer;
//
//void * LoadFile(const char * i_pFilename, size_t & o_sizeFile);
//Sprites::Sprite * CreateSprite(const char * i_pFilename);
//
//void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
//{
//#ifdef _DEBUG
//	const size_t	lenBuffer = 65;
//	char			Buffer[lenBuffer];
//
//	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
//	OutputDebugStringA(Buffer);
//#endif // __DEBUG
//}
//
//int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
//{
//	// IMPORTANT: first we need to initialize  
//	bool bSuccess =  Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, 800, 600);
//
//	if (bSuccess)
//	{
//		// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
//		 SetKeyStateChangeCallback(TestKeyCallback);
//
//		// Create a couple of sprites using our own helper routine CreateSprite
//		 Sprites::Sprite * pGoodGuy = CreateSprite("data\\GoodGuy.dds");
//		 Sprites::Sprite * pBadGuy = CreateSprite("data\\BadGuy.dds");
//
//		bool bQuit = false;
//
//		do
//		{
//			// IMPORTANT: We need to let GLib do it's thing. 
//			 Service(bQuit);
//
//			if (!bQuit)
//			{
//				// IMPORTANT: Tell GLib that we want to start rendering
//				 BeginRendering();
//				// Tell GLib that we want to render some sprites
//				 Sprites::BeginRendering();
//
//				if (pGoodGuy)
//				{
//					static float			moveDist = .01f;
//					static float			moveDir = moveDist;
//
//					static  Point2D	Offset = { -180.0f, -100.0f };
//
//					if (Offset.x < -220.0f)
//						moveDir = moveDist;
//					else if (Offset.x > -140.0f)
//						moveDir = -moveDist;
//
//					Offset.x += moveDir;
//
//					// Tell GLib to render this sprite at our calculated location
//					 Sprites::RenderSprite(*pGoodGuy, Offset, 0.0f);
//				}
//				if (pBadGuy)
//				{
//					static float			moveDist = .02f;
//					static float			moveDir = -moveDist;
//
//					static  Point2D	Offset = { 180.0f, -100.0f };
//
//					if (Offset.x > 200.0f)
//						moveDir = -moveDist;
//					else if (Offset.x < 160.0f)
//						moveDir = moveDist;
//
//					Offset.x += moveDir;
//
//					// Tell GLib to render this sprite at our calculated location
//					 Sprites::RenderSprite(*pBadGuy, Offset, 0.0f);
//				}
//
//				// Tell GLib we're done rendering sprites
//				 Sprites::EndRendering();
//				// IMPORTANT: Tell GLib we're done rendering
//				 EndRendering();
//			}
//		} while (bQuit == false);
//
//		if (pGoodGuy)
//			 Sprites::Release(pGoodGuy);
//		if (pBadGuy)
//			 Sprites::Release(pBadGuy);
//
//		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
//		 Shutdown();
//	}
//
//#if defined _DEBUG
//	_CrtDumpMemoryLeaks();
//#endif // _DEBUG
//
//}
//
// Sprites::Sprite * CreateSprite(const char * i_pFilename)
//{
//	assert(i_pFilename);
//
//	size_t sizeTextureFile = 0;
//
//	// Load the source file (texture data)
//	void * pTextureFile = LoadFile(i_pFilename, sizeTextureFile);
//
//	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
//	 Texture * pTexture = pTextureFile ?  CreateTexture(pTextureFile, sizeTextureFile) : nullptr;
//
//	// exit if something didn't work
//	// probably need some debug logging in here!!!!
//	if (pTextureFile)
//		delete[] pTextureFile;
//
//	if (pTexture == nullptr)
//		return NULL;
//
//	unsigned int width = 0;
//	unsigned int height = 0;
//	unsigned int depth = 0;
//
//	// Get the dimensions of the texture. We'll use this to determine how big it is on screen
//	bool result =  GetDimensions(pTexture, width, height, depth);
//	assert(result == true);
//	assert((width > 0) && (height > 0));
//
//	// Define the sprite edges
//	 Sprites::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
//	 Sprites::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
//	 RGBA							Color = { 255, 255, 255, 255 };
//
//	// Create the sprite
//	 Sprites::Sprite * pSprite =  Sprites::CreateSprite(Edges, 0.1f, Color, UVs);
//	if (pSprite == nullptr)
//	{
//		 Release(pTexture);
//		return nullptr;
//	}
//
//	// Bind the texture to sprite
//	 Sprites::SetTexture(*pSprite, *pTexture);
//
//	return pSprite;
//}
//
//void * LoadFile(const char * i_pFilename, size_t & o_sizeFile)
//{
//	assert(i_pFilename != NULL);
//
//	FILE * pFile = NULL;
//
//	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
//	if (fopenError != 0)
//		return NULL;
//
//	assert(pFile != NULL);
//
//	int FileIOError = fseek(pFile, 0, SEEK_END);
//	assert(FileIOError == 0);
//
//	long FileSize = ftell(pFile);
//	assert(FileSize >= 0);
//
//	FileIOError = fseek(pFile, 0, SEEK_SET);
//	assert(FileIOError == 0);
//
//	uint8_t * pBuffer = new uint8_t[FileSize];
//	assert(pBuffer);
//
//	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
//	assert(FileRead == FileSize);
//
//	fclose(pFile);
//
//	o_sizeFile = FileSize;
//
//	return pBuffer;
//}
/*
struct Counters
{
long		m_strongCount;
long		m_weakCount;

Counters(long i_strongCount = 0, long i_weakCount = 0) :
m_strongCount(i_strongCount),
m_weakCount(i_weakCount)
{

}
};

template<typename T>
class StrongPointer
{
public:
StrongPointer(T * i_ptr = NULL);
~StrongPointer();

StrongPointer(const StrongPointer & i_other);
StrongPointer &operator=(const StrongPointer & i_other);

T * operator->();
T & operator*();

operator bool() const;

bool operator==(const StrongPointer & i_other) const;
};

template<typename T>
class WeakPointer
{
public:
WeakPointer(T * i_ptr = NULL);

WeakPointer(const WeakPointer & i_other);
WeakPointer &operator=(const WeakPointer & i_other);

StrongPointer<T> Acquire() const;

bool IsValid() const { return m_pObject != nullptr; }
operator bool() const { return IsValid();  }

T * operator->();
T & operator*();


private:
T *			m_pObject;
Counters *	m_pCounters;
};

class UniquePointer
{
public:
UniquePointer(T * i_ptr = NULL);

UniquePointer(const UniquePointer & i_other);
UniquePointer &operator=(const UniquePointer & i_other);

bool IsValid() const { return m_pObject != nullptr; }
operator bool() const { return IsValid(); }

T * operator->();
T & operator*();

private:
T *			m_pObject;
};
class Vector3
{
}

class GameObject
{

public:
Vector3 GetPosition();
};

struct Renderable
{
WeakPointer<GameObject> m_pObject;
GLib::Sprite *			m_pSprite;
};

void Draw( Renderable & i_Renderable)
{
StrongPointer<GameObject> MyObj = i_Renderable.m_pObject.Acquire();

if (MyObj)
{
// if StrongPointer is valid draw it
GLib::Sprites::RenderSprite(*i_Renderable.m_pSprite, MyObj->GetPosition(), 0.0f);
}
else
{
// Need to destroy this Renderable since the GameObject has been released
}
}

void Test()
{
// Ptr1 is the owner of the new GameObject
UniquePointer<GameObject> Ptr1(new GameObject("Joe"));

// Now Ptr2 is the owner and Ptr1 points to NULL
UniquePointer<GameObject> Ptr2 = Ptr1;

// Now Ptr1 is the owner and Ptr2 points to NULL
Ptr1 = Ptr2;
}
*/
