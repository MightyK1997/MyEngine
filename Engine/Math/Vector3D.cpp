#include <math.h>
#include "Vector3D.h"

namespace Engine
{
	Vector3D::Vector3D(float i_xAxis, float i_yAxis, float i_zAxis)
	{
		xAxis = i_xAxis;
		yAxis = i_yAxis;
		zAxis = i_zAxis;
	}

	Vector3D::Vector3D(float i_Input)
	{
		xAxis = i_Input;
		yAxis = i_Input;
		zAxis = i_Input;
	}

	bool Vector3D::operator==(const Vector3D& i_Other)
	{
		return (xAxis == i_Other.getX() && yAxis == i_Other.getY() && zAxis == i_Other.getZ());
	}

	Vector3D& Vector3D::operator+=(const Vector3D &i_Input1)
	{
		xAxis += i_Input1.xAxis;
		yAxis += i_Input1.yAxis;
		zAxis += i_Input1.zAxis;
		return *this;
	}
	Vector3D& Vector3D::operator+=(float &i_Input1)
	{
		xAxis += i_Input1;
		yAxis += i_Input1;
		zAxis += i_Input1;
		return *this;
	}
	Vector3D& Vector3D::operator-=(const Vector3D &i_Input1)
	{
		xAxis -= i_Input1.xAxis;
		yAxis -= i_Input1.yAxis;
		zAxis -= i_Input1.zAxis;
		return *this;
	}
	Vector3D& Vector3D::operator-=(float &i_Input1)
	{
		xAxis -= i_Input1;
		yAxis -= i_Input1;
		zAxis -= i_Input1;
		return *this;
	}

	bool Vector3D::operator!=(const Vector3D& i_Other)
	{
		return !(*this == i_Other);
	}

	float Vector3D::GetMagnitude()
	{
		return (float)sqrt((xAxis*xAxis) + (yAxis * yAxis) + (zAxis*zAxis));
	}
	double Vector3D::dot(Vector3D &i_Input2)
	{
		double o_Output;
		o_Output = xAxis * i_Input2.getX() + yAxis * i_Input2.getY() + zAxis * i_Input2.getZ();
		return o_Output;
	}

	bool operator==(Vector3D &i_Input1, Vector3D &i_Input2)
	{
		if ((i_Input1.getX() == i_Input2.getX()) && (i_Input1.getY() == i_Input2.getY()) && (i_Input1.getZ() == i_Input2.getZ())) return true;
		else return false;
	}
	bool operator!=(Vector3D &i_Input1, Vector3D &i_Input2)
	{
		if ((i_Input1.getX() != i_Input2.getX()) || (i_Input1.getY() != i_Input2.getY()) || (i_Input1.getZ() != i_Input2.getZ())) return true;
		else return false;
	}

	inline Vector3D operator* (const Vector3D& i_Input1, const float& i_Input2)
	{
		return Vector3D(i_Input1.getX() * i_Input2, i_Input1.getY()* i_Input2, i_Input1.getZ() * i_Input2);
	}

	inline Vector3D operator* (const float& i_Input2, const Vector3D& i_Input1)
	{
		return Vector3D(i_Input1.getX() * i_Input2, i_Input1.getY()* i_Input2, i_Input1.getZ() * i_Input2);
	}

	inline Vector3D operator/ (const Vector3D& i_Input1, const float& i_Input2)
	{
		assert(i_Input2 != 0);
		return Vector3D(i_Input1.getX() / i_Input2, i_Input1.getY() / i_Input2, i_Input1.getZ() / i_Input2);
	}

	inline Vector3D operator/ (const float& i_Input2, const Vector3D& i_Input1)
	{
		assert(i_Input2 != 0);
		return Vector3D(i_Input1.getX() / i_Input2, i_Input1.getY() / i_Input2, i_Input1.getZ() / i_Input2);
	}

	inline Vector3D operator+(const Vector3D &i_Input1, const float &i_Input2)
	{
		Vector3D o_Output;
		o_Output.setX(i_Input1.getX() + i_Input2);
		o_Output.setY(i_Input1.getY() + i_Input2);
		o_Output.setZ(i_Input1.getZ() + i_Input2);
		return o_Output;
	}

	inline Vector3D operator+(const float &i_Input2, const Vector3D &i_Input1)
	{
		Vector3D o_Output;
		o_Output.setX(i_Input1.getX() + i_Input2);
		o_Output.setY(i_Input1.getY() + i_Input2);
		o_Output.setZ(i_Input1.getZ() + i_Input2);
		return o_Output;
	}

	inline Vector3D operator-(const Vector3D &i_Input1, const Vector3D &i_Input2)
	{
		Vector3D o_Output;
		o_Output.setX(i_Input1.getX() - i_Input2.getX());
		o_Output.setY(i_Input1.getY() - i_Input2.getY());
		o_Output.setZ(i_Input1.getZ() - i_Input2.getZ());
		return o_Output;
	}

	inline Vector3D operator-(const Vector3D &i_Input1, const float &i_Input2)
	{
		Vector3D o_Output;
		o_Output.setX(i_Input1.getX() - i_Input2);
		o_Output.setY(i_Input1.getY() - i_Input2);
		o_Output.setZ(i_Input1.getZ() - i_Input2);
		return o_Output;
	}

	inline Vector3D operator-(const float &i_Input2, const Vector3D &i_Input1)
	{
		Vector3D o_Output;
		o_Output.setX(i_Input1.getX() - i_Input2);
		o_Output.setY(i_Input1.getY() - i_Input2);
		o_Output.setZ(i_Input1.getZ() - i_Input2);
		return o_Output;
	}

	inline Vector3D operator+(const Vector3D &i_Input1, const Vector3D &i_Input2)
	{
		Vector3D o_Output;
		o_Output.setX(i_Input1.getX() + i_Input2.getX());
		o_Output.setY(i_Input1.getY() + i_Input2.getY());
		o_Output.setZ(i_Input1.getZ() + i_Input2.getZ());
		return o_Output;
	}
}