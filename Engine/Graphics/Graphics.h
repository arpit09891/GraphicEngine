/*
	This file declares the external interface for the graphics system
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Include Files
//==============

#include "Configuration.h"
#include <cstdint>
#include <Engine/Results/Results.h>
#include"cSamplerState.h"
#include"ConstantBufferFormats.h"
#include"cConstantBuffer.h"
#include <Engine/Concurrency/cEvent.h>
#include<vector>
#include"Camera.h"
//#include"SpriteWithEffects.h"
class SpriteWithEffects;
class OpaqueMesh;
class TranslucentMesh;
#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
			
		// Render
		//-------
		struct sInitializationParameters
		{
#if defined( EAE6320_PLATFORM_WINDOWS )
			HWND mainWindow = NULL;
	#if defined( EAE6320_PLATFORM_D3D )
			uint16_t resolutionWidth, resolutionHeight;
	#elif defined( EAE6320_PLATFORM_GL )
			HINSTANCE thisInstanceOfTheApplication = NULL;
	#endif
#endif
		};

		
		void RenderFrame();
		void SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime);
		void ChangeBackgroundColor(float red, float green, float blue);
		cResult Initialize( const sInitializationParameters& i_initializationParameters );
		cResult CleanUp();
		eae6320::cResult InitializeGeometry();
		eae6320::cResult InitializeShadingData();
		
		cResult WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds);
		// When the application has finished submitting data for a frame
		// it must call this function
		cResult SignalThatAllDataForAFrameHasBeenSubmitted();
		void SubmitSpriteAndEffectData(std::vector<SpriteWithEffects> v_spriteWithEffects);
		void SubmitMeshAndEffectData(std::vector<OpaqueMesh> v_meshWithEffects);
		void SubmitTranslucentMeshes(std::vector<TranslucentMesh> v_translucentMeshes);
		void SubmitCamera(Camera camera);
	}
}

#endif	// EAE6320_GRAPHICS_H
