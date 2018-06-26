#include "Matrix.h"

using namespace Engine::Math::Vector;
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

			Matrix Matrix::CreateTranslation(Vector3 i_Translation)
			{
				Matrix m;
				m.m_Matrix[0].m128_f32[0] = 1.0f;
				m.m_Matrix[1].m128_f32[1] = 1.0f;
				m.m_Matrix[2].m128_f32[2] = 1.0f;
				m.m_Matrix[3].m128_f32[3] = 1.0f;
				m.m_Matrix[0].m128_f32[3] = i_Translation.x();
				m.m_Matrix[1].m128_f32[3] = i_Translation.y();
				m.m_Matrix[2].m128_f32[3] = i_Translation.z();
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
			Matrix Matrix::GetInverse(void) const
			{
				Matrix returnMatrix = GetTranspose();
				__m128 T = returnMatrix.m_Matrix[3];
				Vector::Vector4 V = { T.m128_f32[0], T.m128_f32[1], T.m128_f32[2], T.m128_f32[3] };
				returnMatrix.m_Matrix[3] = _mm_setzero_ps();
				V = returnMatrix * V;
				returnMatrix.m_Matrix[0].m128_f32[3] = -V[0];
				returnMatrix.m_Matrix[1].m128_f32[3] = -V[1];
				returnMatrix.m_Matrix[3].m128_f32[3] = -V[2];
				returnMatrix.m_Matrix[3].m128_f32[3] = 1;
				return returnMatrix;
			}
			Matrix Matrix::GetTranspose(void) const
			{
				Matrix returnMatrix = *this;
				_MM_TRANSPOSE4_PS(returnMatrix.m_Matrix[0], returnMatrix.m_Matrix[1], returnMatrix.m_Matrix[2], returnMatrix.m_Matrix[3]);
				return returnMatrix;
			}
			float Matrix::GetDeterminant(void) const
			{
				float d1, d2, d3, d4;
				return 0.0f;
			}
			Matrix Matrix::operator*(const Matrix & i_Rhs)
			{
				Matrix returnMatrix;
				Matrix tempMatrix = i_Rhs.GetTranspose();
				__m128 tempVariable;
				for (size_t i = 0; i < 4; i++)
				{
					for (size_t j = 0; j < 4; j++)
					{
						tempVariable.m128_f32[j] = _mm_cvtss_f32(_mm_dp_ps(m_Matrix[i], i_Rhs.m_Matrix[j], 0xF1));
					}
					returnMatrix.m_Matrix[i] = tempVariable;
				}
				return returnMatrix;
			}
			Matrix Matrix::operator*(const float & i_Rhs)
			{
				for (size_t i = 0; i < 4; i++)
				{
					_mm_mul_ps(m_Matrix[i], __m128{i_Rhs, i_Rhs, i_Rhs, i_Rhs});
				}
				return *this;
			}
			Vector4 Matrix::operator*(Vector4 & i_Rhs)
			{
				__m128 tempVariable = { i_Rhs[0], i_Rhs[1], i_Rhs[2], i_Rhs[3] };
				__m128 temp2;
				for (int i = 0; i < 4; i++)
				{
					temp2.m128_f32[i] = _mm_cvtss_f32(_mm_dp_ps(m_Matrix[i], tempVariable, 0xF1));
				}
				return Vector4(temp2.m128_f32[0], temp2.m128_f32[1], temp2.m128_f32[2], temp2.m128_f32[3]);
			}
			Matrix operator+(Matrix m1, Matrix m2)
			{
				Matrix returnMatrix;
				for (size_t i = 0; i < 4; i++)
				{
					returnMatrix[i] = _mm_add_ps(m1[i], m2[i]);
				}
				return returnMatrix;
			}
			Matrix operator-(Matrix m1, Matrix m2)
			{
				Matrix returnMatrix;
				for (size_t i = 0; i < 4; i++)
				{
					returnMatrix[i] = _mm_sub_ps(m1[i], m2[i]);
				}
				return returnMatrix;
			}
}
	}
}