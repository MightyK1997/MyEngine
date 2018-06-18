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
		}
	}
}