#pragma once
#include <cassert>
#include "..\Logging\ConsolePrint.h"
#include "Vector3D.h"
#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
#include <array>

namespace Engine
{
	class Vector4D
	{
	public:
		Vector4D() 
		{
			for (int  i=0; i<4;i++)
			{
				m_Vector[i] = 0;
			}
		}
		Vector4D(float, float, float, float);
		Vector4D(const Vector3D& i_Vector3) : m_Vector{ i_Vector3.getX(), i_Vector3.getY(), i_Vector3.getZ(), 1.0f } {}
		Vector4D& operator+=(const Vector4D&);
		Vector4D& operator+=(float&);
		Vector4D& operator-=(const Vector4D&);
		Vector4D& operator-=(float&);
		bool operator==(const Vector4D&);
		bool operator!=(const Vector4D&);
		float getX() const { return m_Vector[0]; }
		float getY() const { return m_Vector[1]; }
		float getZ() const { return m_Vector[2]; }
		float getW() const { return m_Vector[3]; }
		float& operator[](int pos)
		{
			return m_Vector[pos];
		}
		//float GetMagnitude();
		void setX(float i_xAxis) { m_Vector[0] = i_xAxis; }
		void setY(float i_yAxis) { m_Vector[1] = i_yAxis; }
		void setZ(float i_zAxis) { m_Vector[2] = i_zAxis; }
		void setW(float i_wAxis) { m_Vector[3] = i_wAxis; }
		__m128 returnAsM128() const
		{ 
			__m128 returnValue = { m_Vector[0], m_Vector[1], m_Vector[2], m_Vector[3] };
			return returnValue;
		}

	private:
		/*float xAxis, yAxis, zAxis, wAxis;*/
		std::array<float, 4> m_Vector;
	};

	extern inline Vector4D operator* (const Vector4D& i_Input1, const float& i_Input2);
	extern inline Vector4D operator/ (const Vector4D& i_Input1, const float& i_Input2);
	extern inline Vector4D operator+(const Vector4D &i_Input1, const float &i_Input2);
	extern inline Vector4D operator+(const float &i_Input2, const Vector4D &i_Input1);
	extern inline Vector4D operator-(const Vector4D &i_Input1, const Vector4D &i_Input2);

	extern inline Vector4D operator-(const Vector4D &i_Input1, const float &i_Input2);

	extern inline Vector4D operator-(const float &i_Input2, const Vector4D &i_Input1);
	extern inline Vector4D operator+(const Vector4D &i_Input1, const Vector4D &i_Input2);
	extern inline Vector4D operator*(const Vector4D& i_Input1, const Vector4D& i_Input2);

}