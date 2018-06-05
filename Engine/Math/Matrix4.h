#pragma once

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#include <math.h>
#include <array>
#include <vector>
#include "Vector4D.h"
#include "..\..\Engine\FeatureCheck.h"
#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

namespace Engine
{
	class Matrix4
	{
	public:
		Matrix4();

		Matrix4(
			float i_11, float i_12, float i_13, float i_14,
			float i_21, float i_22, float i_23, float i_24,
			float i_31, float i_32, float i_33, float i_34,
			float i_41, float i_42, float i_43, float i_44
		) 
		{
			m_Matrix = { i_11, i_12, i_13, i_14, i_21, i_22, i_23, i_24, i_31, i_32, i_33, i_34, i_41, i_42, i_43, i_44 };
		}

		~Matrix4(){}
		//Useful Stuff
		static Matrix4 GetIdentityMatrix();
		static Matrix4 CreateXRotation(float i_RotationRadians);
		static Matrix4 CreateYRotation(float i_RotationRadians);
		static Matrix4 CreateZRotation(float i_RotationRadians);
		static Matrix4 CreateRotation(const Vector3D& i_Rot, const float i_AngleInDegrees);

		static Matrix4 CreateTranslation(const Vector3D& i_trans);
		static Matrix4 CreateTranslation(float i_transX, float i_transY, float i_transZ);
		static Matrix4 CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ);

		static Matrix4 CreateAffineTransformation(Vector4D ScalingVector, Vector4D RotationOrigin, Vector4D RotationQuaternion, Vector4D TranslationPoint);

		bool MatrixIsIdentity();

		//Invert This Instance
		void Invert(void);
		
		//Return Inverse Matrix
		Matrix4 GetInverse(void);

		//Return Adjoint Matrix
		Matrix4 GetAdjoint(void);

		//Transpose This Instance
		void Transpose(void);

		//Transpose this instance to a new matrix
		Matrix4 GetTranspose(void) const;

		float GetDeterminant();

		float FindSubDeterminant(std::vector<std::vector<float>> m);

		//Matrix Multiplication
		Matrix4 operator*(const Matrix4& i_other) const;

		Matrix4 operator*(const float i_Value) const;

		Vector4D operator*(Vector4D& i_Other) const;

		//Division

		Matrix4 operator/(const float i_Value) const;

		std::array<float, 4>& operator[](int pos);

		//__m128 getRow(float pos);

		//Other Operators

		bool operator== (const Matrix4& i_Input);
		bool operator!= (const Matrix4& i_Input);

		//Multiply Vector To matrix
		Vector4D MultiplyLeft(Vector4D& i_Vector) const;
		Vector4D MultiplyRight(Vector4D& i_Vector) const;

		inline __m128 ToM128(const std::array<float, 4>& i_Input) const
		{
			__m128 returnValue = { i_Input[0], i_Input[1], i_Input[2], i_Input[3] };
			return returnValue;
		}

		//Get a Zero matrix
		static Matrix4 Zero();

		
	private:
		std::array<std::array < float, 4>,4> m_Matrix;
	};
}