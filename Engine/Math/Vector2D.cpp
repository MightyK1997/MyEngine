#pragma once

#include "Vector2D.h"


namespace Engine
{
	Vector2D::Vector2D(float i_xAxis, float i_yAxis)
	{
		xAxis = i_xAxis;
		yAxis = i_yAxis;
	}
	Vector2D::Vector2D(float i_xAxis)
	{
		xAxis = i_xAxis;
		yAxis = 0;
	}

	Vector2D Vector2D::operator+=(Vector2D &i_Input1)
	{
#ifdef _DEBUG
		ConsolePrint_Windows("Vector Addition", __LINE__, __FILE__, "Engine");
#endif
		xAxis += i_Input1.xAxis;
		yAxis += i_Input1.yAxis;
		return *this;
	}
	Vector2D Vector2D::operator+=(float &i_Input1)
	{
		xAxis += i_Input1;
		yAxis += i_Input1;
		return *this;
	}
	Vector2D Vector2D::operator-=(Vector2D &i_Input1)
	{
#ifdef _DEBUG
		ConsolePrint_Windows("Vector Subtraction", __LINE__, __FILE__, "Engine");
#endif
		xAxis -= i_Input1.xAxis;
		yAxis -= i_Input1.yAxis;
		return *this;
	}
	Vector2D Vector2D::operator-=(float &i_Input1)
	{
		xAxis += i_Input1;
		yAxis += i_Input1;
		return *this;
	}
	bool operator==(Vector2D &i_Input1, Vector2D &i_Input2)
	{
		if ((i_Input1.getX() == i_Input2.getX()) && (i_Input1.getY() == i_Input2.getY())) return true;
		else return false;
	}
	bool operator!=(Vector2D &i_Input1, Vector2D &i_Input2)
	{
		if ((i_Input1.getX() != i_Input2.getX()) || (i_Input1.getY() != i_Input2.getY())) return true;
		else return false;
	}
}