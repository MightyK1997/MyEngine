#pragma once

#include <array>
#include <cassert>

namespace Engine
{
	namespace Math
	{
		namespace Vector
		{
			class Vector2
			{
			public:
				Vector2() : m_Vector2{ 0,0 } {}
				Vector2(float x, float y);
				Vector2(float);

				Vector2(const Vector2& i_Vector2) = default;
				Vector2& operator=(const Vector2& i_Vector2) = default;

				~Vector2() = default;
				float operator[](const int& i_Pos)
				{
					assert(i_Pos > 0 && i_Pos <= 2);
					return m_Vector2[i_Pos - 1];
				}
				float x() const { return m_Vector2[0]; }
				float y() const  { return m_Vector2[1]; }


				//Dot and Cross Product

				float Dot(const Vector2& i_Second) const;


			private:
				std::array<float, 2> m_Vector2;
			};

			extern inline Vector2 operator+(Vector2& i_Lhs, Vector2& i_Rhs);
			extern inline Vector2 operator-(Vector2& i_Lhs, Vector2& i_Rhs);
			extern inline Vector2 operator+(Vector2& i_Lhs, float& i_Rhs);
			extern inline Vector2 operator-(Vector2& i_Lhs, float& i_Rhs);
		}
	}
}
