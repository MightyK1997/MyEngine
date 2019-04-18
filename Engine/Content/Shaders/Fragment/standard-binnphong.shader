/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )


// Entry Point
//============

void main(

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET,


	//For the vertexColors
	in const float4 i_position : SV_POSITION,
	in const float3 i_normals : NORMAL,
	in const float4 i_vertex_position_world : TEXCOORD1,
	in const float4 i_color : COLOR,
	in const float2 i_textureData : TEXCOORD0

	)
{
	const float4 textureColor = SampleTexture2d(g_diffuseTexture, g_samplerState, i_textureData);
	const float4 ambientLight = float4(0.2,0.2,0.2,1);
	const float3 normalizedNormal = normalize(i_normals);


	//Directional Light

    const float3 normalizeL = -normalize(g_LightRotation);
	const float3 viewDist = g_CameraPositionInWorld - i_vertex_position_world.xyz;
    const float3 normalizeV = normalize(viewDist);
    const float3 H = normalize(normalizeL+normalizeV);
    const float4 blinnPhong = pow(saturate(dot(normalizedNormal, H)), 50);
    const float4 specularLight = saturate(blinnPhong * g_LightColor) * 10;
    const float4 diffuseLight = saturate(g_LightColor * (saturate(dot(normalize(g_LightRotation), i_normals))));

	const float4 diffuseOutput = (diffuseLight + ambientLight);

	float4 directionalLightOutput = diffuseOutput + specularLight;





	//PointLight
	float3 pointLightDir = g_PointLightPositionInWorld - i_vertex_position_world.xyz;
	float distance = length(pointLightDir);

	float attenuation = 1.0 / (distance);

	float3 pointH = normalize(normalize(pointLightDir) + normalizeV);

	const float4 binnPhongPoint = pow(saturate(dot(pointH, normalizedNormal)), 50);
	float4 diffusePoint = g_PointLightColor * saturate(dot(pointLightDir, i_normals)) * 5;
	float4 specularPoint = (g_PointLightColor) * binnPhongPoint * 5;

	float4 pointLightOutput = (diffusePoint * attenuation) + (specularPoint * attenuation) +ambientLight;

	o_color = (pointLightOutput + directionalLightOutput) * i_color * textureColor;
}

#elif defined( EAE6320_PLATFORM_GL )

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

//Vertex Colors
layout(location = 0) in vec4 i_color;

// Entry Point
//============

void main()
{
	// Output solid white
	o_color = i_color;
}

#endif
