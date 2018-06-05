#include "Matrix4.h"

namespace Engine
{

	Matrix4::Matrix4()
	{
		for (auto itr = m_Matrix.begin();itr != m_Matrix.end();itr++)
		{
			for (auto jtr = (*itr).begin(); jtr!=(*itr).end();jtr++)
			{
				(*jtr) = 0;
			}
		}
	}

	Engine::Matrix4 Matrix4::GetIdentityMatrix()
	{
		return Matrix4(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	}

	Engine::Matrix4 Matrix4::CreateXRotation(float i_RotationRadians)
	{
		float cosValue = static_cast<float>(cos(i_RotationRadians));
		float sinValue = static_cast<float>(sin(i_RotationRadians));
		return Matrix4(1.0f, 0, 0, 0, 0, cosValue, -sinValue, 0, 0, sinValue, cosValue, 0, 0, 0, 0, 1.0f);
	}

	Engine::Matrix4 Matrix4::CreateYRotation(float i_RotationRadians)
	{
		float cosValue = static_cast<float>(cos(i_RotationRadians));
		float sinValue = static_cast<float>(sin(i_RotationRadians));
		return Matrix4(cosValue, 0, sinValue, 0, 0, 1, 0, 0, -sinValue, 0, cosValue, 0, 0, 0, 0, 1.0f);
	}

	Engine::Matrix4 Matrix4::CreateZRotation(float i_RotationRadians)
	{
		float cosValue = static_cast<float>(cos(i_RotationRadians));
		float sinValue = static_cast<float>(sin(i_RotationRadians));
		return Matrix4(cosValue, -sinValue, 0, 0, sinValue, cosValue, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1.0f);
	}

	Engine::Matrix4 Matrix4::CreateRotation(const Vector3D& i_Rot, const float i_AngleInDegrees)
	{
		Matrix4 returnMatrix;
		return returnMatrix.Zero();
	}

	Engine::Matrix4 Matrix4::CreateTranslation(float i_transX, float i_transY, float i_transZ)
	{
		Matrix4 returnMatrix;
		returnMatrix.m_Matrix[0][0] = 1.0f;
		returnMatrix.m_Matrix[1][1] = 1.0f;
		returnMatrix.m_Matrix[2][2] = 1.0f;
		returnMatrix.m_Matrix[3][3] = 1.0f;
		returnMatrix.m_Matrix[0][3] = i_transX;
		returnMatrix.m_Matrix[1][3] = i_transY;
		returnMatrix.m_Matrix[2][3] = i_transZ;

		return returnMatrix;
	}

	Engine::Matrix4 Matrix4::CreateTranslation(const Vector3D& i_trans)
	{
		Matrix4 returnMatrix;
		returnMatrix.m_Matrix[0][0] = 1.0f;
		returnMatrix.m_Matrix[1][1] = 1.0f;
		returnMatrix.m_Matrix[2][2] = 1.0f;
		returnMatrix.m_Matrix[3][3] = 1.0f;
		returnMatrix.m_Matrix[0][3] = i_trans.getX();
		returnMatrix.m_Matrix[1][3] = i_trans.getY();
		returnMatrix.m_Matrix[2][3] = i_trans.getZ();

		return returnMatrix;
	}

	Engine::Matrix4 Matrix4::CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ)
	{
		Matrix4 returnMatrix;
		returnMatrix.m_Matrix[0][0] = i_scaleX;
		returnMatrix.m_Matrix[1][1] = i_scaleY;
		returnMatrix.m_Matrix[2][2] = i_scaleZ;
		returnMatrix.m_Matrix[3][3] = 1.0f;
		return returnMatrix;
	}

	Engine::Matrix4 Matrix4::CreateAffineTransformation(Vector4D ScalingVector, Vector4D RotationOrigin, Vector4D RotationQuaternion, Vector4D TranslationPoint)
	{
		return Matrix4(ScalingVector.getX(), RotationOrigin.getX(), RotationQuaternion.getX(), TranslationPoint.getX(), ScalingVector.getY(), RotationOrigin.getY(), RotationQuaternion.getY(), TranslationPoint.getY(),
			ScalingVector.getZ(), RotationOrigin.getZ(), RotationQuaternion.getZ(), TranslationPoint.getZ(), ScalingVector.getW(), RotationOrigin.getW(), RotationQuaternion.getW(), TranslationPoint.getW());
	}

	bool Matrix4::MatrixIsIdentity()
	{
		Matrix4 IdentityMatrix = GetIdentityMatrix();
		return (*this == IdentityMatrix);
	}

	void Matrix4::Invert(void)
	{
		Matrix4 returnMatrix;
		assert(GetDeterminant() == 0);
		returnMatrix = GetAdjoint().GetInverse() / GetDeterminant();
		*this = returnMatrix;
	}

	Engine::Matrix4 Matrix4::GetInverse(void)
	{
		Matrix4 returnMatrix;
		float det = GetDeterminant();
		//assert(GetDeterminant() == 0);
		return returnMatrix = GetAdjoint().GetTranspose() / det;
	}

	Engine::Matrix4 Matrix4::GetAdjoint(void)
	{
		Matrix4 adjointMatrix;

		std::vector<std::vector<float>> m;
		for (int i = 0; i < 4; i++)
			m.push_back({ m_Matrix[i][0], m_Matrix[i][1], m_Matrix[i][2], m_Matrix[i][3] });

		float sum = 0;
		for (int i = 0; i < m.size(); i++)
		{
			for (int j = 0; j < m.size(); j++)
			{
				std::vector<std::vector<float>> n;

				for (int x = 0; x < m.size(); x++)
				{
					if (x != i)
					{
						std::vector<float> nRow;
						for (int y = 0; y < m.size(); y++)
							if (y != j)
								nRow.push_back(m[x][y]);
						n.push_back(nRow);
					}
				}
				if ((i + j) % 2)
					adjointMatrix[i][j] = -FindSubDeterminant(n);
				else
					adjointMatrix[i][j] = FindSubDeterminant(n);

			}
		}
		return adjointMatrix;

	}

	void Matrix4::Transpose(void)
	{
		for (int i=0;i<4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				std::swap(m_Matrix[i][j], m_Matrix[j][i]);
			}
		}
	}

	Engine::Matrix4 Matrix4::GetTranspose(void) const
	{
		Matrix4 returnMatrix;
		for (int i = 0;i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				returnMatrix.m_Matrix[i][j] = m_Matrix[j][i];
			}
		}
		return returnMatrix;
	}


	float Matrix4::FindSubDeterminant(std::vector<std::vector<float>> m)
	{
		if (m.size() == 1)
			return m[0][0];
		else if (m.size() == 2)
			return ((m[0][0] * m[1][1]) - (m[0][1] * m[1][0]));
		else
		{
			float sum = 0;
			for (int i = 0; i < m.size(); i++)
			{
				std::vector<std::vector<float>> n;
				for (int j = 1; j < m.size(); j++)
				{
					std::vector<float> nRow;
					for (int k = 0; k < m.size(); k++)
						if (k != i)
							nRow.push_back(m[j][k]);
					n.push_back(nRow);
				}
				float value = m[0][i] * FindSubDeterminant(n);

				if (i % 2)
					sum -= value;
				else
					sum += value;
			}
			return sum;
		}
	}


	float Matrix4::GetDeterminant()
	{
		std::vector<std::vector<float>> n;
		for (int i = 0; i < 4; i++)
			n.push_back({ m_Matrix[i][0], m_Matrix[i][1], m_Matrix[i][2], m_Matrix[i][3] });

		return FindSubDeterminant(n);
	}

	Engine::Matrix4 Matrix4::operator*(const Matrix4& i_other) const
	{
		Matrix4 c;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					c.m_Matrix[i][j] += (m_Matrix[i][k] * i_other.m_Matrix[k][j]);
				}
			}
		}

		return c;
	}

	Engine::Matrix4 Matrix4::operator*(const float i_Value) const
	{
		Matrix4 returnMatrix = *this;
		for (int i=0;i<4;i++)
		{
			for (int j=0;j<4;j++)
			{
				returnMatrix[i][j] = m_Matrix[i][j] * i_Value;
			}
		}
		return returnMatrix;
	}

	Engine::Vector4D Matrix4::operator*(Vector4D& i_Other) const
	{
		Vector4D returnValue;

		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 4; k++)
			{
				returnValue[i] += (m_Matrix[i][k] * i_Other[k]);
			}
		}

		return returnValue;
	}

	Engine::Matrix4 Matrix4::operator/(const float i_Value) const
	{
		Matrix4 returnMatrix;
		for (int i = 0;i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				returnMatrix[i][j] = m_Matrix[i][j] / i_Value;
			}
		}
		return returnMatrix;
	}

	std::array<float, 4>& Matrix4::operator[](int pos)
	{
		return m_Matrix[pos];
	}

	bool Matrix4::operator==(const Matrix4& i_Input)
	{
		for (int i = 0;i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				if (m_Matrix[i][j] != i_Input.m_Matrix[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}

	bool Matrix4::operator!=(const Matrix4& i_Input)
	{
		for (int i=0;i<4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				if (m_Matrix[i][j] == i_Input.m_Matrix[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}

	Engine::Vector4D Matrix4::MultiplyLeft(Vector4D& i_Vector) const
	{
		Vector4D returnValue;

		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 4; k++)
			{
				returnValue[i] += (m_Matrix[i][k] * i_Vector[k]);
			}
		}

		return returnValue;
	}

	Engine::Matrix4 Matrix4::Zero()
	{
		return Matrix4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

}