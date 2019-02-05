/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )

// Entry Point
//============

	struct VS_OUTPUT
	{
		float4 o_vertexPosition_projected : SV_POSITION;
		float4 o_vertexPosition_local : TEXCOORD1;
		float4 o_vertexColor_projected : COLOR;
		float4 o_vertexColor_local : TEXCOORD2;
	};

void main(

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET,

	//For the vertexColors
	// in const float4 i_position : SV_POSITION,
	// in const float4 i_color : COLOR

	in const VS_OUTPUT i_VSInput

	)
{
	// Output solid white
	o_color = float4(
		floor(sin(i_VSInput.o_vertexPosition_local.x) / cos(i_VSInput.o_vertexPosition_local.x)),
		floor(sin(i_VSInput.o_vertexPosition_local.y) / cos(i_VSInput.o_vertexPosition_local.y)),
		floor(sin(i_VSInput.o_vertexPosition_local.z) / cos(i_VSInput.o_vertexPosition_local.z)), 1.0)
		* i_VSInput.o_vertexPosition_local;
}

#elif defined( EAE6320_PLATFORM_GL )

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

//Vertex Colors
layout(location = 1) in vec4 i_color;

// Entry Point
//============

void main()
{
	// Output solid white
	o_color = vec4(
		// RGB (color)
		abs(sin(g_elapsedSecondCount_simulationTime)), abs(cos(g_elapsedSecondCount_simulationTime)), abs(sin(g_elapsedSecondCount_simulationTime) / cos(g_elapsedSecondCount_simulationTime)),
		// Alpha (transparency)
		1.0 ) * i_color;
}

#endif
