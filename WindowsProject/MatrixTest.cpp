#include "..\Engine\Math\Matrix4.h"
#include "..\Engine\Math\SIMDMatrix4.h"
#include <cassert>
#include <string>
#include <stdarg.h>
#include <Windows.h>
#include "..\Engine\Math\Vector4D.h"


using namespace Engine;

bool MatrixUnitTest()
{
	SIMDMatrix4 testMatrix = Engine::SIMDMatrix4::GetIdentityMatrix();
	//assert(testMatrix == Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

	SIMDMatrix4 XrotMatrix = Engine::SIMDMatrix4::CreateXRotation(1.57079633f);
	//assert(XrotMatrix. == -1.0f);

	SIMDMatrix4 YrotMatrix = Engine::SIMDMatrix4::CreateYRotation(1.57079633f);
	//assert(YrotMatrix.Getm13() == 1.0f);

	SIMDMatrix4 ZrotMatrix = Engine::SIMDMatrix4::CreateZRotation(1.57079633f);
	//assert(ZrotMatrix.Getm12() == -1.0f);

	SIMDMatrix4 transposeMatrix = testMatrix.GetTranspose();
	assert(transposeMatrix == testMatrix);

	SIMDMatrix4 scaleMatrix = Engine::SIMDMatrix4::CreateScale(10, 10, 10);

	SIMDMatrix4 m1 = { 3,2,0,1,4,0,1,2,3,0,2,1,0,0,0,1 };
	Matrix4 m3 = { 3,2,0,1,4,0,1,2,3,0,2,1,0,0,0,1 };
	Matrix4 test = m3.GetInverse();
	m1.Invert();
	SIMDMatrix4 m2 = { 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 };
	Vector4D v1 = { 1,2,3,4 };
	
	Vector4D v2 = m3*v1;

	Vector4D v3 = m1 * v1;

	if (m2 == m1)
	{
		return true;
	}

	return true;
}
