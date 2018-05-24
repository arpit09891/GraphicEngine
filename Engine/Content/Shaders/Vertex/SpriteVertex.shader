/*
This is an example vertex shader used to render geometry
*/

#include <Shaders/shaders.inc>

// Constant Buffers
//=================

DeclareConstantBuffer(g_constantBuffer_perFrame, 0)
//cbuffer g_constantBuffer_perFrame : register( b0 )
{
	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding;
};

DeclareConstantBuffer(g_constantBuffer_perMaterial, 1)
//cbuffer g_constantBuffer_perMaterial : register( b1 )
{
	float4 g_color;
};

DeclareConstantBuffer(g_constantBuffer_perDrawCall, 2)
//cbuffer g_constantBuffer_perDrawCall : register( b2 )
{
	// This is a placeholder to prevent an empty constant buffer declaration
	float4 DUMMY;
};

// Entry Point
//============

#if defined( EAE6320_PLATFORM_D3D )

void main(

	// Input
	//======

	// The "semantics" (the keywords in all caps after the colon) are arbitrary,
	// but must match the C call to CreateInputLayout()

	// These values come from one of the VertexFormats::sGeometry that the vertex buffer was filled with in C code
	in const float2 i_position : POSITION,
	in const float2 i_uv : TEXCOORD0,


	// Output
	//=======

	// An SV_POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 output_position								 : SV_POSITION,
	out float2 o_uv : TEXCOORD0


)

#elif defined( EAE6320_PLATFORM_GL )
/*
This is an example vertex shader used to render geometry
*/
layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_uv;
layout(location = 0) out vec2 o_uv;

void main()
#endif

{
	// Calculate the position of this vertex on screen
	{
		// This example shader sets the "out" position directly from the "in" position:
		output_position = float4(i_position.x /** sin(g_elapsedSecondCount_simulationTime)*/, i_position.y, 0.0, 1.0);
		// Both of the following lines are exactly equivalent to the one above
		/*o_position = float4( i_position.xy, 0.0, 1.0 );
		o_position = float4( i_position, 0.0, 1.0 );*/
		o_uv = i_uv;
	}

	// EAE6320_TODO: Change the position based on time!
	// The value g_elapsedSecondCount_simulationTime constantly changes as the simulation progresses, and so by doing something like:
	//	sin( g_elapsedSecondCount_simulationTime ) or cos( g_elapsedSecondCount_simulationTime )
	// you can get a value that will oscillate between [-1,1].
	// You should change at least one position element so that the triangle animates.
	// For example, to change X you would do something kind of like:
	//		o_position.x = ? sin( g_elapsedSecondCount_simulationTime ) ?
	// You can change .x and .y (but leave .z as 0.0 and .w as 1.0).
	// The screen dimensions are already [1,1], so you may want to do some math
	// on the result of the sinusoid function to keep the triangle mostly on screen.
}