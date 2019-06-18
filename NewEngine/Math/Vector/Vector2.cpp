#include "Vector2.h"

namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
			Vector2::Vector2(float x, float y)
			{
				m_Vector2 = { x, y };
			}

			Vector2::Vector2(float i_Input)
			{
				m_Vector2 = { i_Input };
			}

			float Vector2::Dot(const Vector2& i_Second) const
			{
				return m_Vector2[0] * i_Second.x() + m_Vector2[1] * i_Second.y();
			}

			Vector2 operator+(Vector2 & i_Lhs, Vector2 & i_Rhs)
			{
				return { i_Lhs.x() + i_Rhs.x(), i_Lhs.y() + i_Rhs.y() };
			}
			Vector2 operator-(Vector2 & i_Lhs, Vector2 & i_Rhs)
			{
				return { i_Lhs.x() - i_Rhs.x(), i_Lhs.y() - i_Rhs.y() };
			}

			Vector2 operator+(Vector2 & i_Lhs, float & i_Rhs)
			{
				return { i_Lhs.x() + i_Rhs, i_Lhs.y() + i_Rhs };
			}
			Vector2 operator-(Vector2 & i_Lhs, float & i_Rhs)
			{
				return { i_Lhs.x() - i_Rhs, i_Lhs.y() - i_Rhs };
			}
		}
	}
}