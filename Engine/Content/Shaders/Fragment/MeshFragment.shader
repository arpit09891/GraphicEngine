/*
This is an example fragment shader used to render geometry
*/
#include <Shaders/shaders.inc>
// Constant Buffers
//=================

DeclareConstantBuffer(g_constantBuffer_perFrame, 0)
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	float2 g_padding; // For float4 alignment
};

DeclareConstantBuffer(g_constantBuffer_perMaterial, 1)
{
	float4 g_color;
};

// Entry Point
//============
DeclareTexture2D(g_color_texture, 0);

DeclareSampleState(g_color_samplerState, 0);

#if defined( EAE6320_PLATFORM_D3D )

void main(
	//Input
	//=======
	in const float4 i_position : SV_POSITION,
	in const float2 i_uv : TEXCOORD0,
	in const float4 i_color : COLOR0,
	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET

	//Textures
	//======


)

#elif defined( EAE6320_PLATFORM_GL )
layout(location = 0) in vec2 i_uv;
layout(location = 1) in vec4 i_color;
layout(location = 0) out vec4 o_color;

void main()

#endif
{

	// Output solid white
	//o_color = float4(
	//	// RGB
	//	1.0, 1.0, 1.0,
	//	// Alpha
	//	1.0 );

	//Output Textures

	const float4 sampledColor = DeclareSample(g_color_texture, g_color_samplerState, i_uv);
	//g_color_texture.Sample(g_color_samplerState, i_uv);
	//float4 combinedColor = color1 * color2;
	o_color = sampledColor;

	// EAE6320_TODO: Change the color based on time!
	// The value g_elapsedSecondCount_simulationTime constantly changes as the simulation progresses, and so by doing something like:
	//	sin( g_elapsedSecondCount_simulationTime ) or cos( g_elapsedSecondCount_simulationTime )
	// you can get a value that will oscillate between [-1,1].
	// You should change at least one "channel" so that the color animates.
	// For example, to change red ("r") you would do something kind of like:
	//		o_color.r = ? sin( g_elapsedSecondCount_simulationTime ) ?
	// You can change .r, .g, and .b (but leave .a as 1.0).
	// Remember that your final values should be [0,1], so you will have to do some math.
}

