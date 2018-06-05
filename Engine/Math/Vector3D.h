#pragma once
#include <cassert>
#include "..\Logging\ConsolePrint.h"
namespace Engine
{
	class Vector3D
	{
	public:
		Vector3D() { xAxis = 0; yAxis = 0; zAxis = 0; }
		Vector3D(float, float, float);
		Vector3D(float);
		Vector3D& operator+=(const Vector3D&);
		Vector3D& operator+=(float&);
		Vector3D& operator-=(const Vector3D&);
		Vector3D& operator-=(float&);
		bool operator== (const Vector3D&);
		bool operator!=(const Vector3D&);
		float getX() const { return xAxis; }
		float getY() const { return yAxis; }
		float getZ() const { return zAxis; }
		float GetMagnitude();
		void setX(float i_xAxis) { xAxis = i_xAxis; }
		void setY(float i_yAxis) { yAxis = i_yAxis; }
		void setZ(float i_zAxis) { zAxis = i_zAxis; }
		double dot(Vector3D& i_Input2);

	private:
		float xAxis, yAxis, zAxis;
	};

	extern inline Vector3D operator* (const Vector3D& i_Input1, const float& i_Input2); 
	extern inline Vector3D operator/ (const Vector3D& i_Input1, const float& i_Input2);
	extern inline Vector3D operator+(const Vector3D &i_Input1, const float &i_Input2);
	extern inline Vector3D operator+(const float &i_Input2, const Vector3D &i_Input1);
	extern inline Vector3D operator-(const Vector3D &i_Input1, const Vector3D &i_Input2);

	extern inline Vector3D operator-(const Vector3D &i_Input1, const float &i_Input2);

	extern inline Vector3D operator-(const float &i_Input2, const Vector3D &i_Input1);
	extern inline Vector3D operator+(const Vector3D &i_Input1, const Vector3D &i_Input2);
}

