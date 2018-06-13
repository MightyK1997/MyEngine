#pragma once

#include <immintrin.h>
#include "Vector3.h"
#include <vector>
#include <array>

namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
			class Vector4 
			{
			public:
				Vector4(float x, float y, float z, float w);
				Vector4(const Vector3&);
				~Vector4(){}
			private:
				__m128 m_Vector4;
			};
		}
	}
}
