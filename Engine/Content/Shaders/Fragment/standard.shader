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
	in const float4 i_color : COLOR

	)
{
	// Output solid white
	o_color = i_color;
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
	o_color = i_color;
}

#endif
