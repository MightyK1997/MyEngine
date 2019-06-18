#pragma once

#include <array>
#include <cassert>

namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
			class Vector3
			{
			public:
				Vector3() : m_Vector3{ 0,0,0 }{}
				Vector3(float x, float y, float z);
				Vector3(float);

				Vector3(const Vector3& i_Vector3) = default;
				Vector3& operator=(const Vector3& i_Vector3) = default;

				~Vector3() = default;
				float operator[](const int& i_Pos)
				{
					assert(i_Pos > 0 && i_Pos <= 3);
					return m_Vector3[i_Pos - 1];
				}
				float x() const { return m_Vector3[0]; }
				float y() const { return m_Vector3[1]; }
				float z() const { return m_Vector3[2]; }


				//Dot and cross product
				float Dot(const Vector3& i_Second) const;
				Vector3 Cross(const Vector3& i_Second) const;

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
