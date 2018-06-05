#include "SIMDMatrix4.h"


namespace Engine
{

	SIMDMatrix4::SIMDMatrix4()
	{
		for (int i = 0;i < 4;i++)
		{
				m_Matrix[i] = _mm_setzero_ps();
		}
	}

	SIMDMatrix4 SIMDMatrix4::GetIdentityMatrix()
	{
		SIMDMatrix4 returnMatrix;
		for ( int i=0;i<4;i++)
		{
			for (int j=0;j<4;j++)
			{
				returnMatrix.m_Matrix[i].m128_f32[j] = 1;
			}
		}
		return returnMatrix;
	}

	SIMDMatrix4 SIMDMatrix4::CreateXRotation(float i_RotationRadians)
	{
		float cosValue = static_cast<float>(cos(i_RotationRadians));
		float sinValue = static_cast<float>(sin(i_RotationRadians));
		return SIMDMatrix4(1.0f, 0, 0, 0, 0, cosValue, -sinValue, 0, 0, sinValue, cosValue, 0, 0, 0, 0, 1.0f);
	}

	SIMDMatrix4 SIMDMatrix4::CreateYRotation(float i_RotationRadians)
	{
		float cosValue = static_cast<float>(cos(i_RotationRadians));
		float sinValue = static_cast<float>(sin(i_RotationRadians));
		return SIMDMatrix4(cosValue, 0, sinValue, 0, 0, 1, 0, 0, -sinValue, 0, cosValue, 0, 0, 0, 0, 1.0f);
	}

	SIMDMatrix4 SIMDMatrix4::CreateZRotation(float i_RotationRadians)
	{
		float cosValue = static_cast<float>(cos(i_RotationRadians));
		float sinValue = static_cast<float>(sin(i_RotationRadians));
		return SIMDMatrix4(cosValue, -sinValue, 0, 0, sinValue, cosValue, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1.0f);
	}

	SIMDMatrix4 SIMDMatrix4::CreateTranslation(const Vector3D& i_trans)
	{
		SIMDMatrix4 returnMatrix;
		returnMatrix.m_Matrix[0].m128_f32[0] = 1.0f;
		returnMatrix.m_Matrix[1].m128_f32[1] = 1.0f;
		returnMatrix.m_Matrix[2].m128_f32[2] = 1.0f;
		returnMatrix.m_Matrix[3].m128_f32[3] = 1.0f;
		returnMatrix.m_Matrix[0].m128_f32[3] = i_trans.getX();
		returnMatrix.m_Matrix[1].m128_f32[3] = i_trans.getY();
		returnMatrix.m_Matrix[2].m128_f32[3] = i_trans.getZ();
		return returnMatrix;
	}

	SIMDMatrix4 SIMDMatrix4::CreateTranslation(float i_transX, float i_transY, float i_transZ)
	{
		SIMDMatrix4 returnMatrix;
		returnMatrix.m_Matrix[0].m128_f32[0] = 1.0f;
		returnMatrix.m_Matrix[1].m128_f32[1] = 1.0f;
		returnMatrix.m_Matrix[2].m128_f32[2] = 1.0f;
		returnMatrix.m_Matrix[3].m128_f32[3] = 1.0f;
		returnMatrix.m_Matrix[0].m128_f32[3] = i_transX;
		returnMatrix.m_Matrix[1].m128_f32[3] = i_transY;
		returnMatrix.m_Matrix[2].m128_f32[3] = i_transZ;
		return returnMatrix;
	}

	SIMDMatrix4 SIMDMatrix4::CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ)
	{
		SIMDMatrix4 returnMatrix;
		returnMatrix.m_Matrix[0].m128_f32[0] = i_scaleX;
		returnMatrix.m_Matrix[1].m128_f32[1] = i_scaleY;
		returnMatrix.m_Matrix[2].m128_f32[2] = i_scaleZ;
		returnMatrix.m_Matrix[3].m128_f32[3] = 1.0f;
		return returnMatrix;
	}

	bool SIMDMatrix4::MatrixIsIdentity()
	{
		return *this == GetIdentityMatrix();
	}

	void SIMDMatrix4::Invert(void)
	{
		SIMDMatrix4 ReturnMatrix = GetTranspose();
		__m128 T = ReturnMatrix.m_Matrix[3];
		Vector4D V = { T.m128_f32[0], T.m128_f32[1], T.m128_f32[2], T.m128_f32[3] };
		ReturnMatrix.m_Matrix[3] = _mm_setzero_ps();
		//for (int i = 0;i < 4;i++)
		//{
		//	for (int j = 0;j < 4;j++)
		//	{
		//		ReturnMatrix.m_Matrix[i].m128_f32[j] = -ReturnMatrix.m_Matrix[i].m128_f32[j];
		//	}
		//}
		V = ReturnMatrix * V;
		ReturnMatrix.m_Matrix[0].m128_f32[3] = -V[0];
		ReturnMatrix.m_Matrix[1].m128_f32[3] = -V[1];
		ReturnMatrix.m_Matrix[3].m128_f32[3] = -V[2];
		ReturnMatrix.m_Matrix[3].m128_f32[3] = 1;
		*this = ReturnMatrix;
	}

	SIMDMatrix4 SIMDMatrix4::GetInverse(void)
	{
		SIMDMatrix4 ReturnMatrix = GetTranspose();
		__m128 T = ReturnMatrix.m_Matrix[3];
		Vector4D V = { T.m128_f32[0], T.m128_f32[1], T.m128_f32[2], T.m128_f32[3] };
		ReturnMatrix.m_Matrix[3] = _mm_setzero_ps();
		//for (int i = 0;i < 4;i++)
		//{
		//	for (int j = 0;j < 4;j++)
		//	{
		//		ReturnMatrix.m_Matrix[i].m128_f32[j] = -ReturnMatrix.m_Matrix[i].m128_f32[j];
		//	}
		//}
		V = ReturnMatrix * V;
		ReturnMatrix.m_Matrix[0].m128_f32[3] = -V[0];
		ReturnMatrix.m_Matrix[1].m128_f32[3] = -V[1];
		ReturnMatrix.m_Matrix[3].m128_f32[3] = -V[2];
		ReturnMatrix.m_Matrix[3].m128_f32[3] = 1;
		return ReturnMatrix;
	}

	void SIMDMatrix4::Transpose(void)
	{
		_MM_TRANSPOSE4_PS(m_Matrix[0], m_Matrix[1], m_Matrix[2], m_Matrix[3]);
	}

	SIMDMatrix4 SIMDMatrix4::GetTranspose(void) const
	{
		SIMDMatrix4 returnMatrix = *this;
		_MM_TRANSPOSE4_PS(returnMatrix.m_Matrix[0], returnMatrix.m_Matrix[1], returnMatrix.m_Matrix[2], returnMatrix.m_Matrix[3]);
		return returnMatrix;
	}

	/*float SIMDMatrix4::GetDeterminant()
	{

	}*/

	SIMDMatrix4 SIMDMatrix4::operator*(SIMDMatrix4& i_other)
	{
		SIMDMatrix4 returnMatrix;
		SIMDMatrix4 tempMatrix = i_other.GetTranspose();
		__m128 tempVariable;
		for (int i = 0; i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				tempVariable.m128_f32[j] = _mm_cvtss_f32(_mm_dp_ps(m_Matrix[i], i_other.m_Matrix[j], 0xF1));
			}
			returnMatrix.m_Matrix[i] = tempVariable;
		}
		return returnMatrix;
	}

	SIMDMatrix4 SIMDMatrix4::operator*(const float i_Value)
	{
		SIMDMatrix4 returnMatrix;
		__m128 mulValue = { i_Value, i_Value, i_Value, i_Value };
		for (int i = 0;i < 4;i++)
		{
			returnMatrix.m_Matrix[i] = _mm_mul_ps(m_Matrix[i], mulValue);
		}
		return returnMatrix;
	}

	Vector4D SIMDMatrix4::operator*(Vector4D& i_Other)
	{
		__m128 tempVariable = { i_Other[0], i_Other[1], i_Other[2], i_Other[3] };
		__m128 temp2;
		for (int i = 0;i < 4;i++)
		{
			temp2.m128_f32[i] = _mm_cvtss_f32(_mm_dp_ps(m_Matrix[i], tempVariable, 0xF1));
		}
		return Vector4D(temp2.m128_f32[0], temp2.m128_f32[1], temp2.m128_f32[2], temp2.m128_f32[3]);
	}

	SIMDMatrix4 SIMDMatrix4::operator/(const float i_Value) const
	{
		SIMDMatrix4 returnMatrix;
		__m128 mulValue = { i_Value, i_Value, i_Value, i_Value };
		for (int i = 0;i < 4;i++)
		{
			returnMatrix.m_Matrix[i] = _mm_div_ps(m_Matrix[i], mulValue);
		}
		return returnMatrix;
	}

	bool SIMDMatrix4::operator!=(const SIMDMatrix4& i_Input)
	{
		return !(*this == i_Input);
	}

	bool SIMDMatrix4::operator==(const SIMDMatrix4& i_Input)
	{
		for (int i = 0;i < 4;i++)
		{
			__m128 temp = _mm_cmpeq_ps(m_Matrix[i], i_Input.m_Matrix[i]);
			if (_mm_movemask_ps(temp) != 0xF)
			{
				return false;
			}
		}
		return true;
	}

}
