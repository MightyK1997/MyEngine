#pragma once


#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
#include <math.h>
#include <array>
#include <vector>
#include "Vector4D.h"

namespace Engine
{
	class SIMDMatrix4
	{
	public:
		SIMDMatrix4();

		SIMDMatrix4(
			float i_11, float i_12, float i_13, float i_14,
			float i_21, float i_22, float i_23, float i_24,
			float i_31, float i_32, float i_33, float i_34,
			float i_41, float i_42, float i_43, float i_44
		)
		{
			m_Matrix = { i_11, i_12, i_13, i_14, i_21, i_22, i_23, i_24, i_31, i_32, i_33, i_34, i_41, i_42, i_43, i_44 };
		}

		~SIMDMatrix4() {}
		//Useful Stuff
		static SIMDMatrix4 GetIdentityMatrix();
		static SIMDMatrix4 CreateXRotation(float i_RotationRadians);
		static SIMDMatrix4 CreateYRotation(float i_RotationRadians);
		static SIMDMatrix4 CreateZRotation(float i_RotationRadians);

		static SIMDMatrix4 CreateTranslation(const Vector3D& i_trans);
		static SIMDMatrix4 CreateTranslation(float i_transX, float i_transY, float i_transZ);
		static SIMDMatrix4 CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ);
		bool MatrixIsIdentity();

		//Invert This Instance
		void Invert(void);

		//Return Inverse Matrix
		SIMDMatrix4 GetInverse(void);

		//Return Adjoint Matrix
		SIMDMatrix4 GetAdjoint(void);

		//Transpose This Instance
		void Transpose(void);

		//Transpose this instance to a new matrix
		SIMDMatrix4 GetTranspose(void) const;

		float GetDeterminant();

		//Matrix Multiplication
		SIMDMatrix4 operator*(SIMDMatrix4& i_other);

		SIMDMatrix4 operator*(const float i_Value);

		Vector4D operator*(Vector4D& i_Other);

		//Division

		SIMDMatrix4 operator/(const float i_Value) const;
		//Other Operators

		bool operator== (const SIMDMatrix4& i_Input);
		bool operator!= (const SIMDMatrix4& i_Input);
	private:
		std::array<__m128, 4> m_Matrix;
	};
}
