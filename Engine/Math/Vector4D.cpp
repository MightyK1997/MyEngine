#include <math.h>
#include "Vector4D.h"

namespace Engine
{
	Vector4D::Vector4D(float i_xAxis, float i_yAxis, float i_zAxis, float i_wAxis)
	{
		m_Vector[0] = i_xAxis;
		m_Vector[1] = i_yAxis;
		m_Vector[2] = i_zAxis;
		m_Vector[3] = i_wAxis;
	}

	bool Vector4D::operator!=(const Vector4D& i_Other)
	{
		return !(*this == i_Other);
	}

	bool Vector4D::operator==(const Vector4D& i_Other)
	{
		for (int i = 0;i < 4;i++)
		{
			if (m_Vector[i] != i_Other.m_Vector[i])
			{
				return false;
			}
		}
		return true;
	}

	Vector4D& Vector4D::operator+=(const Vector4D &i_Input1)
	{
//#ifdef _DEBUG
//		ConsolePrint_Windows("Vector Addition", __LINE__, __FILE__, "Engine");
//#endif
		m_Vector[0] += i_Input1.m_Vector[0];
		m_Vector[1] += i_Input1.m_Vector[1];
		m_Vector[2] += i_Input1.m_Vector[2];
		m_Vector[3] += i_Input1.m_Vector[3];
		return *this;
	}
	Vector4D& Vector4D::operator+=(float &i_Input1)
	{
		m_Vector[0] += i_Input1;
		m_Vector[1] += i_Input1;
		m_Vector[2] += i_Input1;
		m_Vector[3] += i_Input1;
		return *this;
	}
	Vector4D& Vector4D::operator-=(const Vector4D &i_Input1)
	{
//#ifdef _DEBUG
//		ConsolePrint_Windows("Vector Subtraction", __LINE__, __FILE__, "Engine");
//#endif
		m_Vector[0] -= i_Input1.m_Vector[0];
		m_Vector[1] -= i_Input1.m_Vector[1];
		m_Vector[2] -= i_Input1.m_Vector[2];
		m_Vector[3] -= i_Input1.m_Vector[3];
		return *this;
	}
	Vector4D& Vector4D::operator-=(float &i_Input1)
	{
		m_Vector[0] -= i_Input1;
		m_Vector[1] -= i_Input1;
		m_Vector[2] -= i_Input1;
		m_Vector[3] -= i_Input1;
		return *this;
	}
	bool operator==(Vector4D &i_Input1, Vector4D &i_Input2)
	{
		if ((i_Input1.getX() == i_Input2.getX()) && (i_Input1.getY() == i_Input2.getY()) && (i_Input1.getZ() == i_Input2.getZ()) && (i_Input1.getW() == i_Input2.getW())) return true;
		else return false;
	}
	bool operator!=(Vector4D &i_Input1, Vector4D &i_Input2)
	{
		if ((i_Input1.getX() != i_Input2.getX()) || (i_Input1.getY() != i_Input2.getY()) || (i_Input1.getZ() != i_Input2.getZ()) || (i_Input1.getW() != i_Input2.getW())) return true;
		else return false;
	}
	inline Vector4D operator* (const Vector4D& i_Input1, const float& i_Input2)
	{
		return Vector4D(i_Input1.getX() * i_Input2, i_Input1.getY()* i_Input2, i_Input1.getZ() * i_Input2, i_Input1.getW() * i_Input2);
	}

	inline Vector4D operator* (const float& i_Input2, const Vector4D& i_Input1)
	{
		return Vector4D(i_Input1.getX() * i_Input2, i_Input1.getY()* i_Input2, i_Input1.getZ() * i_Input2, i_Input1.getW()*i_Input2);
	}

	extern Vector4D operator*(const Vector4D& i_Input1, const Vector4D& i_Input2)
	{
		__m128 in1 = i_Input1.returnAsM128();
		__m128 in2 = i_Input2.returnAsM128();
		__m128 out = _mm_mul_ps(in1, in2);
		return Vector4D(out.m128_f32[0], out.m128_f32[1], out.m128_f32[2], out.m128_f32[3]);
	}

	inline Vector4D operator/ (const Vector4D& i_Input1, const float& i_Input2)
	{
		assert(i_Input2 != 0);
		return Vector4D(i_Input1.getX() / i_Input2, i_Input1.getY() / i_Input2, i_Input1.getZ() / i_Input2, i_Input1.getW() / i_Input2);
	}

	inline Vector4D operator/ (const float& i_Input2, const Vector4D& i_Input1)
	{
		assert(i_Input2 != 0);
		return Vector4D(i_Input1.getX() / i_Input2, i_Input1.getY() / i_Input2, i_Input1.getZ() / i_Input2, i_Input1.getW() / i_Input2);
	}

	inline Vector4D operator+(const Vector4D &i_Input1, const float &i_Input2)
	{
//#ifdef _DEBUG
//		ConsolePrint_Windows("Vector overloaded +", __LINE__, __FILE__, "Engine");
//#endif
		Vector4D o_Output;
		o_Output.setX(i_Input1.getX() + i_Input2);
		o_Output.setY(i_Input1.getY() + i_Input2);
		o_Output.setZ(i_Input1.getZ() + i_Input2);
		o_Output.setW(i_Input1.getW() + i_Input2);
		return o_Output;
	}

	inline Vector4D operator+(const float &i_Input2, const Vector4D &i_Input1)
	{
//#ifdef _DEBUG
//		ConsolePrint_Windows("Vector overloaded +", __LINE__, __FILE__, "Engine");
//#endif
		Vector4D o_Output;
		o_Output.setX(i_Input1.getX() + i_Input2);
		o_Output.setY(i_Input1.getY() + i_Input2);
		o_Output.setZ(i_Input1.getZ() + i_Input2);
		o_Output.setW(i_Input1.getW() + i_Input2);
		return o_Output;
	}

	inline Vector4D operator-(const Vector4D &i_Input1, const Vector4D &i_Input2)
	{
//#ifdef _DEBUG
//		ConsolePrint_Windows("Vector overloaded -", __LINE__, __FILE__, "Engine");
//#endif
		Vector4D o_Output;
		o_Output.setX(i_Input1.getX() - i_Input2.getX());
		o_Output.setY(i_Input1.getY() - i_Input2.getY());
		o_Output.setZ(i_Input1.getZ() - i_Input2.getZ());
		o_Output.setW(i_Input1.getW() - i_Input2.getW());
		return o_Output;
	}

	inline Vector4D operator-(const Vector4D &i_Input1, const float &i_Input2)
	{
//#ifdef _DEBUG
//		ConsolePrint_Windows("Vector overloaded -", __LINE__, __FILE__, "Engine");
//#endif
		Vector4D o_Output;
		o_Output.setX(i_Input1.getX() - i_Input2);
		o_Output.setY(i_Input1.getY() - i_Input2);
		o_Output.setZ(i_Input1.getZ() - i_Input2);
		o_Output.setW(i_Input1.getW() - i_Input2);
		return o_Output;
	}

	inline Vector4D operator-(const float &i_Input2, const Vector4D &i_Input1)
	{
//#ifdef _DEBUG
//		ConsolePrint_Windows("Vector overloaded -", __LINE__, __FILE__, "Engine");
//#endif
		Vector4D o_Output;
		o_Output.setX(i_Input1.getX() - i_Input2);
		o_Output.setY(i_Input1.getY() - i_Input2);
		o_Output.setZ(i_Input1.getZ() - i_Input2);
		o_Output.setW(i_Input1.getW() - i_Input2);
		return o_Output;
	}

	inline Vector4D operator+(const Vector4D &i_Input1, const Vector4D &i_Input2)
	{
//#ifdef _DEBUG
//		ConsolePrint_Windows("Vector overloaded +", __LINE__, __FILE__, "Engine");
//#endif
		Vector4D o_Output;
		o_Output.setX(i_Input1.getX() + i_Input2.getX());
		o_Output.setY(i_Input1.getY() + i_Input2.getY());
		o_Output.setZ(i_Input1.getZ() + i_Input2.getZ());
		o_Output.setW(i_Input1.getW() + i_Input2.getW());
		return o_Output;
	}
}