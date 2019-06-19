#include "Vector3.h"

namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
			float Vector3::Dot(const Vector3& i_Second) const
			{
				return m_Vector3[0] * i_Second.x() + m_Vector3[1] * i_Second.y() + m_Vector3[2] * i_Second.z();
			}

			Vector3 Vector3::Cross(const Vector3& i_Second) const
			{
				return { y() * i_Second.z() - z() * i_Second.y(), z() *i_Second.x() - x() * i_Second.z(), x() * i_Second.y() - y() * i_Second.x() };
			}


			Vector3 operator+(Vector3 & i_Lhs, Vector3 & i_Rhs)
			{
				return { i_Lhs.x() + i_Rhs.x(), i_Lhs.y() + i_Rhs.y(), i_Lhs.z() + i_Rhs.z() };
			}
			Vector3 operator-(Vector3 & i_Lhs, Vector3 & i_Rhs)
			{
				return { i_Lhs.x() - i_Rhs.x(), i_Lhs.y() - i_Rhs.y(), i_Lhs.z() - i_Rhs.z() };
			}

			Vector3 operator+(Vector3 & i_Lhs, float & i_Rhs)
			{
				return { i_Lhs.x() + i_Rhs, i_Lhs.y() + i_Rhs, i_Lhs.z() + i_Rhs };
			}
			Vector3 operator-(Vector3 & i_Lhs, float & i_Rhs)
			{
				return { i_Lhs.x() - i_Rhs, i_Lhs.y() - i_Rhs, i_Lhs.z() - i_Rhs };
			}
		}
	}
}