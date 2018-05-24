/*
	This is an example vertex shader used to render geometry
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

DeclareConstantBuffer(g_constantBuffer_perDrawCall, 2)
{	
	float4x4 g_transform_localToWorld;
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
	in const float3 i_vertexPosition_local : POSITION,
	in const float2 i_uv : TEXCOORD0,
	in const float4 i_color: COLOR0,
 

	// Output
	//=======

	// An SV_POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 output_position : SV_POSITION,
	out float2 o_uv : TEXCOORD0,
	out float4 o_color : COLOR0

	)

#elif defined( EAE6320_PLATFORM_GL )
/*
	This is an example vertex shader used to render geometry
*/
layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec2 i_uv;
layout( location = 2 ) in vec4 i_color;
layout( location = 0 ) out vec2 o_uv;
layout( location = 1 ) out vec4 o_color;

void main()
#endif

{
	// Calculate the position of this vertex on screen
	{
		float4 vertexPosition_local = float4(i_vertexPosition_local, 1.0);
		float4 vertexPosition_world = MulMatNVect(g_transform_localToWorld, vertexPosition_local);
		float4 vertexPosition_camera = MulMatNVect(g_transform_worldToCamera, vertexPosition_world);
		float4 vertexPosition_projected = MulMatNVect(g_transform_cameraToProjected, vertexPosition_camera);
		// This example shader sets the "out" position directly from the "in" position:
		//output_position = float4(vertexPosition_world.x, vertexPosition_world.y, vertexPosition_world.z, 1.0);
		output_position = vertexPosition_projected;

		//float4(i_vertexPosition_local.x + g_transform_localToWorld[3][0], i_vertexPosition_local.y + g_transform_localToWorld[3][1], 0.0, 1.0);
		// Both of the following lines are exactly equivalent to the one above
		/*o_position = float4( i_position.xy, 0.0, 1.0 );
		o_position = float4( i_position, 0.0, 1.0 );*/
		o_uv = i_uv;
		o_color = i_color;
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