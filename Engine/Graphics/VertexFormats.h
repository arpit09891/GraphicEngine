/*
	A vertex format determines the layout of the geometric data
	that the CPU will send to the GPU
*/

#ifndef EAE6320_GRAPHICS_VERTEXFORMATS_H
#define EAE6320_GRAPHICS_VERTEXFORMATS_H

// Include Files
//==============

#include "Configuration.h"
#include<stdint.h>

// Vertex Formats
//===============

namespace eae6320
{
	namespace Graphics
	{
		namespace VertexFormats
		{
			struct sMesh
			{
				// POSITION
				// 3 floats == 12 bytes
				// Offset = 0
				float x, y, z;

				// TEXCOORD0
				// 2 floats == 8 bytes
				// Offset = ?
				float u, v;

				//Colors
				uint8_t r, g, b, a;
			};

			struct sSprite {
				// POSITION
				// 2 floats == 8 bytes
				// Offset = 0
				float x, y;

				// TEXCOORD0
				// 2 floats == 8 bytes
				// Offset = ?
				float u, v;
			};
		}
	}
}

#endif	// EAE6320_GRAPHICS_VERTEXFORMATS_H
