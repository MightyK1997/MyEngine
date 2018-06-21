#include "Matrix.h"


namespace Engine
{
	namespace Math
	{
		namespace Matrix
		{
			Matrix::Matrix()
			{
				for (size_t i = 0; i < 4; i++)
				{
					m_Matrix[i] = _mm_setzero_ps();
				}
			}
			Matrix Matrix::GetIdentityMatrix()
			{
				return Matrix(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);
			}
			Matrix Matrix::CreateXRotation(float i_Degrees)
			{
				float cosValue = static_cast<float>(cos(i_Degrees * M_PI / 180));
				float sinValue = static_cast<float>(sin(i_Degrees * M_PI / 180));
				return Matrix(1.0f, 0, 0, 0, 0, cosValue, -sinValue, 0, 0, sinValue, cosValue, 0, 0, 0, 0, 1.0f);
			}

			Matrix Matrix::CreateYRotation(float i_Degrees)
			{
				float cosValue = static_cast<float>(cos(i_Degrees * M_PI / 180));
				float sinValue = static_cast<float>(sin(i_Degrees * M_PI / 180));
				return Matrix(cosValue, 0, sinValue, 0, 0, 1, 0, 0, -sinValue, 0, cosValue, 0, 0, 0, 0, 1.0f);
			}

			Matrix Matrix::CreateZRotation(float i_Degrees)
			{
				float cosValue = static_cast<float>(cos(i_Degrees * M_PI / 180));
				float sinValue = static_cast<float>(sin(i_Degrees * M_PI / 180));
				return Matrix(cosValue, -sinValue, 0, 0, sinValue, cosValue, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1.0f);
			}

			Matrix Matrix::CreateTranslation(const Vector::Vector3 & i_Translation)
			{
				Matrix m;
				m.m_Matrix[0].m128_f32[0] = 1.0f;
				m.m_Matrix[1].m128_f32[1] = 1.0f;
				m.m_Matrix[2].m128_f32[2] = 1.0f;
				m.m_Matrix[3].m128_f32[3] = 1.0f;
				m.m_Matrix[0].m128_f32[3] = i_Translation.x;
				m.m_Matrix[1].m128_f32[3] = i_Translation.y;
				m.m_Matrix[2].m128_f32[3] = i_Translation.z;
				return m;
			}
			Matrix Matrix::CreateTranslation(const float i_xTrans, const float i_yTrans, const float i_zTrans)
			{
				Matrix m;
				m.m_Matrix[0].m128_f32[0] = 1.0f;
				m.m_Matrix[1].m128_f32[1] = 1.0f;
				m.m_Matrix[2].m128_f32[2] = 1.0f;
				m.m_Matrix[3].m128_f32[3] = 1.0f;
				m.m_Matrix[0].m128_f32[3] = i_xTrans;
				m.m_Matrix[1].m128_f32[3] = i_yTrans;
				m.m_Matrix[2].m128_f32[3] = i_zTrans;
				return m;
			}
			Matrix Matrix::CreateScale(const float i_xScale, const float i_yScale, const float i_zScale)
			{
				return Matrix();
			}
		}
	}
}