#include "Vector3.h"

namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
			Vector3::Vector3(float x, float y, float z)
			{
				m_Vector3[0] = x;
				m_Vector3[1] = y;
				m_Vector3[2] = z;
			}
			Vector3::Vector3(float i_Input)
			{
				for (size_t i = 0; i < 3; i++)
				{
					m_Vector3[i] = i_Input;
				}
			}
			Vector3 Engine::Math::Vector::Vector3::x(float i_Input)
			{
				m_Vector3[0] = i_Input;
				return *this;
			}
			Vector3 Engine::Math::Vector::Vector3::y(float i_Input)
			{
				m_Vector3[1] = i_Input;
				return *this;
			}
			Vector3 Engine::Math::Vector::Vector3::z(float i_Input)
			{
				m_Vector3[2] = i_Input;
				return *this;
			}
			Vector3 operator+(Vector3 & i_Lhs, Vector3 & i_Rhs)
			{
				return Vector3(i_Lhs.x() + i_Rhs.x(), i_Lhs.y() + i_Rhs.y(), i_Lhs.z() + i_Rhs.z());
			}
			Vector3 operator-(Vector3 & i_Lhs, Vector3 & i_Rhs)
			{
				return Vector3(i_Lhs.x() - i_Rhs.x(), i_Lhs.y() - i_Rhs.y(), i_Lhs.z() - i_Rhs.z());
			}
			Vector3 operator+(Vector3 & i_Lhs, float & i_Rhs)
			{
				return Vector3(i_Lhs.x() + i_Rhs, i_Lhs.y() + i_Rhs, i_Lhs.z() + i_Rhs);
			}
			Vector3 operator-(Vector3 & i_Lhs, float & i_Rhs)
			{
				return Vector3(i_Lhs.x() - i_Rhs, i_Lhs.y() - i_Rhs, i_Lhs.z() - i_Rhs);
			}
		}
	}
}