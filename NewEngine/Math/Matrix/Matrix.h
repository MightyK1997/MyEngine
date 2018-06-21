#pragma once

#define _USE_MATH_DEFINES

#include <array>
#include <math.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include "../Vector/Vector4.h"




namespace Engine
{
	namespace Math
	{
		namespace Matrix
		{
			class Matrix
			{
			public:
				Matrix();
				~Matrix(){}
				Matrix(float i_11, float i_12, float i_13, float i_14,
					float i_21, float i_22, float i_23, float i_24,
					float i_31, float i_32, float i_33, float i_34,
					float i_41, float i_42, float i_43, float i_44)
				{
					m_Matrix = { i_11, i_12, i_13, i_14, i_21, i_22, i_23, i_24, i_31, i_32, i_33, i_34, i_41, i_42, i_43, i_44 };
				}
				Matrix GetIdentityMatrix();
				Matrix CreateXRotation(float i_Degrees);
				Matrix CreateYRotation(float i_Degrees);
				Matrix CreateZRotation(float i_Degrees);
				Matrix CreateTranslation(const Vector::Vector3& i_Translation);
				Matrix CreateTranslation(const float i_xTrans, const float i_yTrans, const float i_zTrans);
				Matrix CreateScale(const float i_xScale, const float i_yScale, const float i_zScale);
				Matrix GetInverse(void) const;
				Matrix GetTranspose(void) const;
				float GetDeterminant(void) const;
				Matrix operator*(const Matrix& i_Rhs);
				Matrix operator*(const float& i_Rhs);
				Vector::Vector4& operator*(const Vector::Vector4& i_Rhs);
			private:
				std::array<__m128, 4> m_Matrix;
			};
		}
	}
}