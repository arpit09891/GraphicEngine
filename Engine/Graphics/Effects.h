#pragma once

//#include "Graphics.h"

//#include "../Graphics/Direct3D/Includes.h"
//#include "cConstantBuffer.h"
#include "cRenderState.h"
#include"../Assets/ReferenceCountedAssets.h"
#include "cShader.h"

class Effects {

	EAE6320_ASSETS_DECLAREREFERENCECOUNT();
	public: 
		static bool Factory(Effects*& effect);
		void Bind();

		EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();
		eae6320::cResult InitializeShadingData(char * vertexTarget, char * fragmentTarget, uint8_t defaultRenderState);
		eae6320::cResult CleanUp();

		eae6320::Graphics::cShader::Handle s_vertexShader;
		eae6320::Graphics::cShader::Handle s_fragmentShader;
		eae6320::Graphics::cRenderState s_renderState;
		EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(Effects);
#if defined (EAE6320_PLATFORM_D3D)
	
#elif defined (EAE6320_PLATFORM_GL)
	GLuint s_programId = 0;
	
#endif
private:
		Effects() = default;
		~Effects()
		{
			volatile int z = 0;
		}
};