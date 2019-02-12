/*
	This file should be #included by all shaders
*/

// Version Information
//====================

#if defined( EAE6320_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

#endif

// Constant Buffers
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )
	
	//Adding constant buffers here to not redeclare them in every shader.
	
	cbuffer g_constantBuffer_perFrame : register( b0 )
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;

	float3 g_CameraPositionInWorld;
	float g_padding0;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding1;
};

cbuffer g_constantBuffer_perDrawCall : register( b2 )
{
  float4x4 g_transform_localToWorld;
  float4x4 g_transform_localToProjected;
};

cbuffer g_constantBuffer_perMaterial : register(b1)
{
float4 g_material_color;
};


#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	
	//Adding constant buffers here to not redeclare them in every shader.
	
	layout( std140, binding = 0 ) uniform g_constantBuffer_perFrame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For vec4 alignment
	vec2 g_padding;
};

layout( std140, binding = 2 ) uniform g_constantBuffer_perdrawCall
{
  mat4 g_transform_localToWorld;
};

#endif
