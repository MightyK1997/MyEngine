#pragma once

#include <immintrin.h>
#include <xmmintrin.h>
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
				Vector4() { m_Vector4 = { 0,0,0,0 }; }
				Vector4(float x, float y, float z, float w) { m_Vector4 = { x,y,z,w }; }
				Vector4(Vector3 i_Vector3) { m_Vector4 = { i_Vector3[0], i_Vector3[1],i_Vector3[2], 0 }; }
				Vector4(__m128 i_Input) { m_Vector4 = i_Input; }
				~Vector4(){}
				float operator[](int i_Pos) { return m_Vector4.m128_f32[i_Pos]; }
				float x() { return m_Vector4.m128_f32[0]; }
				float y() { return m_Vector4.m128_f32[1]; }
				float z() { return m_Vector4.m128_f32[2]; }
				float w() { return m_Vector4.m128_f32[3]; }
				//Vector4 x(float i_Input);
				//Vector4 y(float i_Input);
				//Vector4 z(float i_Input);
				//Vector4 w(float i_Input);
				Vector4 operator+=(Vector4);
				Vector4 operator-=(Vector4);
				Vector4 operator*=(Vector4);
				bool operator==(Vector4);
				bool operator!=(Vector4 i_Vec) { return !(*this == i_Vec); }
				__m128 GetVector() const { return m_Vector4; }
				std::array<float, 4> Get();
			private:
				__m128 m_Vector4;
			};

			extern inline Vector4 operator+(const Vector4& i_Lhs, const Vector4& i_Rhs);
			extern inline Vector4 operator-(const Vector4& i_Lhs, const Vector4& i_Rhs);
			extern inline Vector4 operator-(const Vector4& i_Lhs, float& i_Rhs);
			extern inline Vector4 operator+(const Vector4& i_Lhs, float& i_Rhs);
			extern inline Vector4 operator*(const Vector4& i_Lhs, const Vector4& i_Rhs);
			extern inline Vector4 operator*(const Vector4& i_Lhs, const float& i_Rhs);
		}
	}
}

