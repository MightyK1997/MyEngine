#pragma once

#include <array>
#include <immintrin.h>
#include <xmmintrin.h>

namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
			class Vector3
			{
			public:
				Vector3() {
					for (size_t i = 0; i < 3; i++)
					{
						m_Vector3[i] = 0;
					}
				}
				Vector3(float x, float y, float z);
				Vector3(float);
				float operator[](int i_Pos) { return m_Vector3[i_Pos]; }
				float x(void) { return m_Vector3[0]; }
				float y(void) { return m_Vector3[1]; }
				float z(void) { return m_Vector3[2]; }
				//Vector3 x(float i_Input);
				//Vector3 y(float i_Input);
				//Vector3 z(float i_Input);

			private:
				std::array<float, 3> m_Vector3;
			};

			extern inline Vector3 operator+(Vector3& i_Lhs, Vector3& i_Rhs);
			extern inline Vector3 operator-(Vector3& i_Lhs, Vector3& i_Rhs);
			extern inline Vector3 operator+(Vector3& i_Lhs , float& i_Rhs);
			extern inline Vector3 operator-(Vector3& i_Lhs , float& i_Rhs);
		}
	}
}