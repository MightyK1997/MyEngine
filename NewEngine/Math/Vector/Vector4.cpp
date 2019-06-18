#include "Vector4.h"


namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
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

			bool Vector4::operator==(const Vector4& i_Value) const
			{
				return _mm_movemask_ps(_mm_cmpeq_ps(m_Vector4, i_Value.m_Vector4)) == 0x15d;
			}

			std::array<float, 4> Vector4::Get()
			{
				const std::array<float, 4> returnValue = { m_Vector4.m128_f32[0], m_Vector4.m128_f32[1], m_Vector4.m128_f32[2], m_Vector4.m128_f32[3] };
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