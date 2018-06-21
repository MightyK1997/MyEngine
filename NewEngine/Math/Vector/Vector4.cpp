#include "Vector4.h"


namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
			Vector4 Vector4::x(float i_Input)
			{
				m_Vector4.m128_f32[0] = i_Input;
				return *this;
			}
			Vector4 Vector4::y(float i_Input)
			{
				m_Vector4.m128_f32[1] = i_Input;
				return *this;
			}
			Vector4 Vector4::z(float i_Input)
			{
				m_Vector4.m128_f32[2] = i_Input;
				return *this;
			}
			Vector4 Vector4::w(float i_Input)
			{
				m_Vector4.m128_f32[3] = i_Input;
				return *this;
			}
			Vector4 Vector4::operator+=(Vector4 i_Vec)
			{
				m_Vector4 = _mm_add_ps(m_Vector4, i_Vec.m_Vector4);
				return *this;
			}
			Vector4 Vector4::operator-=(Vector4 i_Vec)
			{
				m_Vector4 = _mm_sub_ps(m_Vector4, i_Vec.m_Vector4);
				return *this;
			}
			Vector4 Vector4::operator*=(Vector4 i_Vec)
			{
				m_Vector4 = _mm_mul_ps(m_Vector4, i_Vec.m_Vector4);
				return *this;
			}
			bool Vector4::operator==(Vector4 i_Vec)
			{
				if (_mm_movemask_ps(_mm_cmpeq_ps(m_Vector4, i_Vec.m_Vector4)) == 0x15d) { return false; }
				return true;
			}
			std::array<float, 4> Vector4::Get()
			{
				std::array<float, 4> returnValue;
				for (size_t i = 0; i < 4; i++)
				{
					returnValue[i] = m_Vector4.m128_f32[i];
				}
				return returnValue;
			}
			Vector4 operator+(const Vector4& i_Lhs, const Vector4& i_Rhs)
			{
				return Vector4(_mm_add_ps(i_Lhs.GetVector(), i_Rhs.GetVector()));
			}
			Vector4 operator-(const Vector4 & i_Lhs, const Vector4 & i_Rhs)
			{
				return Vector4(_mm_sub_ps(i_Lhs.GetVector(), i_Rhs.GetVector()));
			}
			Vector4 operator-(const Vector4 & i_Lhs, const float & i_Rhs)
			{
				return Vector4(_mm_sub_ps(i_Lhs.GetVector(), __m128{i_Rhs, i_Rhs, i_Rhs, i_Rhs}));
			}
			Vector4 operator+(const Vector4 & i_Lhs, const float & i_Rhs)
			{
				return Vector4(_mm_add_ps(i_Lhs.GetVector(), __m128{i_Rhs, i_Rhs, i_Rhs, i_Rhs}));
			}
			Vector4 operator*(const Vector4 & i_Lhs, const Vector4 & i_Rhs)
			{
				return Vector4(_mm_mul_ps(i_Lhs.GetVector(), i_Rhs.GetVector()));
			}
			Vector4 operator*(const Vector4 & i_Lhs, const float & i_Rhs)
			{
				return Vector4(_mm_mul_ps(i_Lhs.GetVector(), __m128{i_Rhs, i_Rhs, i_Rhs, i_Rhs}));
			}
		}
	}
}