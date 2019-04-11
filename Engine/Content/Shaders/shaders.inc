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

	#define DeclareTexture2d( i_name, i_id ) Texture2D i_name : register( t##i_id )
	#define DeclareSamplerState( i_name, i_id ) SamplerState i_name : register( s##i_id )

	#define SampleTexture2d( i_texture, i_samplerState, i_uv ) i_texture.Sample( i_samplerState, i_uv )
	float4 ConvertNormalizedsRGBToLinear(in const float4 i_Color)
	{
		float4 returnColor;
		returnColor.x = i_Color.x < 0.04045f ? i_Color.x / 12.92f : pow(((i_Color.x + 0.055f) / 1.055f), 2.4f);
		returnColor.y = i_Color.y < 0.04045f ? i_Color.y / 12.92f : pow(((i_Color.y + 0.055f) / 1.055f), 2.4f);
		returnColor.z = i_Color.z < 0.04045f ? i_Color.z / 12.92f : pow(((i_Color.z + 0.055f) / 1.055f), 2.4f);
		returnColor.w = i_Color.w;
		return returnColor;
	}
	
	//Adding constant buffers here to not redeclare them in every shader.
	
cbuffer g_constantBuffer_perFrame : register( b0 )
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;
	float3 g_LightRotation;
	float g_padding2;
	float4 g_LightColor;

	float3 g_CameraPositionInWorld;
	float g_padding0;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding1;
};

Texture2D g_diffuseTexture : register(t0);
SamplerState g_samplerState : register(s0);

cbuffer g_constantBuffer_perMaterial : register(b1)
{
	float4 g_material_color;
};

cbuffer g_constantBuffer_perDrawCall : register( b2 )
{
  float4x4 g_transform_localToWorld;
  float4x4 g_transform_localToProjected;
};


#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define DeclareTexture2d( i_name, i_id ) layout( binding = i_id ) uniform sampler2D i_name
	#define DeclareSamplerState( i_name, i_id )	// GLSL requires the sampler state to be set in C code

	#define SampleTexture2d( i_texture, i_samplerState, i_uv ) texture2D( i_texture, i_uv )
	
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