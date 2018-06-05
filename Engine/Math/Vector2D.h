#pragma once

#include "..\Logging\ConsolePrint.h"
namespace Engine
{
	class Vector2D
	{
	public:
		Vector2D() { xAxis = 0; yAxis = 0; }
		Vector2D(float, float);
		Vector2D(float);
		Vector2D operator+=(Vector2D&);
		Vector2D operator+=(float&);
		Vector2D operator-=(Vector2D&);
		Vector2D operator-=(float&);
		float getX() { return xAxis; }
		float getY() { return yAxis; }
		void setX(float i_xAxis) { xAxis = i_xAxis; }
		void setY(float i_yAxis) { yAxis = i_yAxis; }

	private:
		float xAxis, yAxis;
	};

	inline Vector2D operator/(Vector2D& i_Input1, float& i_Input2)
	{
		Vector2D o_Output;

		o_Output.setX(i_Input1.getX() / i_Input2);
		o_Output.setY(i_Input1.getY() / i_Input2);

		return o_Output;
	}

	inline Vector2D operator*(Vector2D& i_Input1, float& i_Input2)
	{
		Vector2D o_Output;

		o_Output.setX(i_Input1.getX() * i_Input2);
		o_Output.setY(i_Input1.getY() * i_Input2);

		return o_Output;
	}

	inline Vector2D operator+(Vector2D &i_Input1, Vector2D &i_Input2)
	{
#ifdef _DEBUG
		ConsolePrint_Windows("Vector overloaded +", __LINE__, __FILE__, "Engine");
#endif
		Vector2D o_Output;
		o_Output.setX(i_Input1.getX() + i_Input2.getX());
		o_Output.setY(i_Input1.getY() + i_Input2.getY());
		return o_Output;
	}
	inline Vector2D operator-(Vector2D &i_Input1, Vector2D &i_Input2)
	{
#ifdef _DEBUG
		ConsolePrint_Windows("Vector overloaded -", __LINE__, __FILE__, "Engine");
#endif
		Vector2D o_Output;
		o_Output.setX(i_Input1.getX() - i_Input2.getX());
		o_Output.setY(i_Input1.getY() - i_Input2.getY());
		return o_Output;
	}
	inline double dot(Vector2D &i_Input1, Vector2D &i_Input2)
	{
		float o_Output;
		o_Output = i_Input1.getX() * i_Input2.getX() + i_Input1.getY() * i_Input2.getY();
		return o_Output;
	}
}