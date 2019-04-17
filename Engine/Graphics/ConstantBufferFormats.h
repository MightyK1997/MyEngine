/*
	This file defines the layout of the constant data
	that the CPU sends to the GPU

	These must exactly match the constant buffer definitions in shader programs.
*/

#ifndef EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H
#define EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H

// Includes
//=========

#include "Configuration.h"

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/cQuaternion.h>
#include <Engine/Math/sVector.h>

// Format Definitions
//===================

namespace eae6320
{
	namespace Graphics
	{
		struct sColor
		{
			float r;
			float g;
			float b;
			float alpha;

			sColor() = default;
		};
		namespace ConstantBufferFormats
		{
			struct sPerFrame
			{
				Math::cMatrix_transformation g_transform_worldToCamera;
				Math::cMatrix_transformation g_transform_cameraToProjected;
				Math::sVector g_LightRotation;
				float pad0;
				Math::sVector g_LightPositionInWorld;
				float pad1;
				Math::sVector g_PointLightPositionInWorld;
				float pad2;
				Math::sVector g_PointLightRotation;
				float pad3;
				sColor g_LightColor;
				sColor g_PointLightColor;

				Math::sVector g_CameraPositionInWorld;
				float padding0;

				float g_elapsedSecondCount_systemTime = 0.0f;
				float g_elapsedSecondCount_simulationTime = 0.0f;
				float padding1[2];	// For float4 alignment
			};

			struct sPerDrawCall
			{
				Math::cMatrix_transformation g_transform_localToWorld;
				Math::cMatrix_transformation g_transform_localToProjected;
			};

			struct sPerMaterialCall
			{
				sColor g_color;
			};
		}
	}
}

#endif	// EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H
