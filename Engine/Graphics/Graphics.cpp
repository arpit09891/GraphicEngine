// Include Files
//==============

#include "Graphics.h"
#include"../Graphics/Effects.h"
#include"../Graphics/Sprite.h"
#include"../Graphics/View.h"

#include "../Graphics/Direct3D/Includes.h"
#include "../Graphics/cConstantBuffer.h"
#include "../Graphics/ConstantBufferFormats.h"
#include "../Graphics/cRenderState.h"
#include "../Graphics/cSamplerState.h"
#include "../Graphics/cShader.h"
#include "../Graphics/sContext.h"
#include "../Graphics/VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <utility>
#include"Engine\Graphics\Effects.h"
#include"Engine\Graphics\Sprite.h"
#include"SpriteWithEffects.h"
#include"OpaqueMesh.h"
#include"TranslucentMesh.h"


namespace eae6320
{
	namespace Graphics
	{
		struct sDataRequiredToRenderAFrame
		{
			eae6320::Graphics::ConstantBufferFormats::sPerFrame constantData_perFrame;
			float red;
			float green;
			float blue;
			Camera camera;
			std::vector<SpriteWithEffects> v_spriteWithEffects;
			std::vector<OpaqueMesh> v_meshWithEffects;
			std::vector<TranslucentMesh> g_translucentMeshes;
		};

		std::vector<SpriteWithEffects>v_graphicSpriteWithEffects;
		std::vector<OpaqueMesh>v_graphicMeshWithEffects;
		std::vector<TranslucentMesh>v_graphicTranslucentMeshes;
		bool dataLock = false;
		eae6320::Graphics::cConstantBuffer s_constantBuffer_perFrame(eae6320::Graphics::ConstantBufferTypes::PerFrame);
		eae6320::Graphics::cConstantBuffer s_constantBuffer_perDrawCall(eae6320::Graphics::ConstantBufferTypes::PerDrawCall);
		eae6320::Graphics::cSamplerState s_samplerState;	
		sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
		auto* s_dataBeingSubmittedByApplicationThread = &s_dataRequiredToRenderAFrame[0];
		auto* s_dataBeingRenderedByRenderThread = &s_dataRequiredToRenderAFrame[1];
		eae6320::Concurrency::cEvent s_whenAllDataHasBeenSubmittedFromApplicationThread;
		eae6320::Concurrency::cEvent s_whenDataForANewFrameCanBeSubmittedFromApplicationThread;	

		View view;
	}
}
void eae6320::Graphics::SubmitSpriteAndEffectData(std::vector<SpriteWithEffects>v_spriteWithEffects) {

			v_graphicSpriteWithEffects.clear();
			
			for (size_t i = 0; i < v_spriteWithEffects.size(); i++) 
			{
				const auto& spriteWithEffects = v_spriteWithEffects.at(i);
				v_graphicSpriteWithEffects.push_back(spriteWithEffects);
			}
			{
				for (size_t i = 0; i < v_spriteWithEffects.size(); i++)
				{
					SpriteWithEffects spriteWithEffect = v_spriteWithEffects.at(i);
					spriteWithEffect.IncrementRefrenceCount();
				}
			}
		
		s_dataBeingSubmittedByApplicationThread->v_spriteWithEffects = v_graphicSpriteWithEffects;
	
}

void eae6320::Graphics::SubmitMeshAndEffectData(std::vector<OpaqueMesh>v_meshWithEffects) {

	v_graphicMeshWithEffects.clear();

	for (size_t i = 0; i < v_meshWithEffects.size(); i++)
	{
		//TODO may be change it to value then reference
		const auto& meshWithEffects = v_meshWithEffects.at(i);
		v_graphicMeshWithEffects.push_back(meshWithEffects);
	}
	/*{
		for (size_t i = 0; i < v_meshWithEffects.size(); i++)
		{
			OpaqueMesh meshWithEffects = v_meshWithEffects.at(i);
			meshWithEffects.IncrementRefrenceCount();
		}
	}*/

	s_dataBeingSubmittedByApplicationThread->v_meshWithEffects = v_graphicMeshWithEffects;

}

void eae6320::Graphics::SubmitTranslucentMeshes(std::vector<TranslucentMesh> v_translucentMeshes)
{
	v_graphicTranslucentMeshes.clear();

	for (size_t i = 0; i < v_translucentMeshes.size(); i++)
	{
		const auto& meshWithEffects = v_translucentMeshes.at(i);
		v_graphicTranslucentMeshes.push_back(meshWithEffects);
	}
	{
		for (size_t i = 0; i < v_translucentMeshes.size(); i++)
		{
			TranslucentMesh meshWithEffects = v_translucentMeshes.at(i);
			meshWithEffects.IncrementRefrenceCount();
		}
	}

	s_dataBeingSubmittedByApplicationThread->g_translucentMeshes = v_graphicTranslucentMeshes;
}

void eae6320::Graphics::SubmitCamera(Camera camera)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constantData_perFrame = s_dataBeingSubmittedByApplicationThread->constantData_perFrame;
	constantData_perFrame.g_transform_cameraToProjected = camera.transform_CameraToProjected;
	constantData_perFrame.g_transform_worldToCamera = camera.transform_WorldToCamera;
}

void eae6320::Graphics::ChangeBackgroundColor(float red, float green, float blue) {
	s_dataBeingSubmittedByApplicationThread->red = red;
	s_dataBeingSubmittedByApplicationThread->green = green;
	s_dataBeingSubmittedByApplicationThread->blue = blue;
}

void eae6320::Graphics::SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime)
{
	EAE6320_ASSERT( s_dataBeingSubmittedByApplicationThread );
	auto& constantData_perFrame = s_dataBeingSubmittedByApplicationThread->constantData_perFrame;
	constantData_perFrame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_perFrame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
}

eae6320::cResult eae6320::Graphics::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
{
	return Concurrency::WaitForEvent(s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
}

eae6320::cResult eae6320::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}

void eae6320::Graphics::RenderFrame()
{
	// Wait for the application loop to submit data to be rendered
	{
		const auto result = Concurrency::WaitForEvent(s_whenAllDataHasBeenSubmittedFromApplicationThread);
		if (result)
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap(s_dataBeingSubmittedByApplicationThread, s_dataBeingRenderedByRenderThread);
			// Once the pointers have been swapped the application loop can submit new data
			const auto result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal();
			if (!result)
			{
				EAE6320_ASSERTF(false, "Couldn't signal that new graphics data can be submitted");
				Logging::OutputError("Failed to signal that new render data can be submitted");
				UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited");
				return;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Waiting for the graphics data to be submitted failed");
			Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
			UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited");
			return;
		}
	}

	view.RenderFrame(s_dataBeingRenderedByRenderThread->red, s_dataBeingRenderedByRenderThread->green, s_dataBeingRenderedByRenderThread->blue);

	EAE6320_ASSERT(s_dataBeingRenderedByRenderThread);

	// Update the per-frame constant buffer
	{
		//TODO Have to add camera logic here
		// Copy the data from the system memory that the application owns to GPU memory
		auto& constantData_perFrame = s_dataBeingRenderedByRenderThread->constantData_perFrame;
		s_constantBuffer_perFrame.Update(&constantData_perFrame);
	}

	//Update the per-draw-call constant buffer
	eae6320::Graphics::ConstantBufferFormats::sPerDrawCall constantData_perDrawCall;

	//sprite data 
	{
		std::vector<SpriteWithEffects> v_spriteWithEffects = s_dataBeingRenderedByRenderThread->v_spriteWithEffects;
		size_t index = v_spriteWithEffects.size();
		while (!v_spriteWithEffects.empty())
		{
			if (v_spriteWithEffects.size() == 0)
				break;
			SpriteWithEffects spriteWithEffect = v_spriteWithEffects.at(v_spriteWithEffects.size() - 1);
			spriteWithEffect.DrawSpriteWithEffect();
			spriteWithEffect.CleanUp();
			if (!v_spriteWithEffects.empty())
				v_spriteWithEffects.pop_back();
			index--;
		}

	}

	//mesh data
	{
		std::vector<OpaqueMesh> v_meshWithEffects = s_dataBeingRenderedByRenderThread->v_meshWithEffects;
		size_t index = v_meshWithEffects.size();
		while (!v_meshWithEffects.empty())
		{
			if (v_meshWithEffects.size() == 0)
				break;
			OpaqueMesh meshWithEffect = v_meshWithEffects.at(v_meshWithEffects.size() - 1);
			constantData_perDrawCall.g_transform_localToWorld = meshWithEffect.transform_localToWorld;
			s_constantBuffer_perDrawCall.Update(&constantData_perDrawCall);
			meshWithEffect.DrawMeshWithEffect();
			/*meshWithEffect.CleanUp();*/
			if (!v_meshWithEffects.empty())
				v_meshWithEffects.pop_back();
			index--;
		}
	}
	
	//translucentmeshes
	{
		std::vector<TranslucentMesh> v_translucentMeshes = s_dataBeingRenderedByRenderThread->g_translucentMeshes;
		size_t index = v_translucentMeshes.size();
		while (!v_translucentMeshes.empty())
		{
			if (v_translucentMeshes.size() == 0)
				break;
			TranslucentMesh meshWithEffect = v_translucentMeshes.at(v_translucentMeshes.size() - 1);
			constantData_perDrawCall.g_transform_localToWorld = meshWithEffect.transform_localToWorld;
			s_constantBuffer_perDrawCall.Update(&constantData_perDrawCall);
			meshWithEffect.DrawMeshWithEffect();
			meshWithEffect.CleanUp();
			if (!v_translucentMeshes.empty())
				v_translucentMeshes.pop_back();
			index--;
		}
	}

	

	view.SwapBetweenFrontAndBackBuffer();

}


eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	// Initialize the platform-specific context
	if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
	{
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	// Initialize the asset managers
	{
		if (!(result = cShader::s_manager.Initialize()))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

	// Initialize the platform-independent graphics objects
	{
		if (result = s_constantBuffer_perFrame.Initialize())
		{
			// There is only a single per-frame constant buffer that is re-used
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_perFrame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				ShaderTypes::Vertex | ShaderTypes::Fragment);
		}
		else
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		if (result = s_constantBuffer_perDrawCall.Initialize())
		{
			// There is only a single per-draw-call constant buffer that is re-used
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_perDrawCall.Bind(
				// In our class both vertex and fragment shaders use per-draw-call constant data
				ShaderTypes::Vertex | ShaderTypes::Fragment);
		}
		else
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		if (result = s_samplerState.Initialize())
		{
			// There is only a single sampler state that is re-used
			// and so it can be bound at initialization time and never unbound
			s_samplerState.Bind();
		}
		else
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}
	// Initialize the events
	{
		if (!(result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		if (!(result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}
	// Initialize the views	
	{
		if (!(result = view.InitializeViews(i_initializationParameters)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}
	// Initialize the shading data
	{
		if (!(result = InitializeShadingData()))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}
	// Initialize the geometry
	{
		if (!(result = InitializeGeometry()))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

OnExit:

	return result;
}


eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = Results::Success;

	view.CleanUp();
	//Sprites with Effects Clean up
	{
		std::vector<SpriteWithEffects> v_spriteWithEffects = s_dataBeingRenderedByRenderThread->v_spriteWithEffects;
		size_t index = v_spriteWithEffects.size();


		while (!v_spriteWithEffects.empty()) {
			SpriteWithEffects spriteWithEffect = v_spriteWithEffects.at(index - 1);
			spriteWithEffect.CleanUp();
			v_spriteWithEffects.pop_back();
			index--;
		}
	}
	//Mesh with Effects Clean Up
	{
		std::vector<OpaqueMesh> v_meshWithEffects = s_dataBeingRenderedByRenderThread->v_meshWithEffects;
		size_t index = v_meshWithEffects.size();


		while (!v_meshWithEffects.empty()) {
			OpaqueMesh meshWithEffect = v_meshWithEffects.at(index - 1);
			meshWithEffect.CleanUp();
			v_meshWithEffects.pop_back();
			index--;
		}
	}

	//translucent Meshes clean up
	{
		std::vector<TranslucentMesh> v_translucentMeshes = s_dataBeingRenderedByRenderThread->g_translucentMeshes;
		size_t index = v_translucentMeshes.size();


		while (!v_translucentMeshes.empty()) {
			TranslucentMesh meshWithEffect = v_translucentMeshes.at(index - 1);
			meshWithEffect.CleanUp();
			v_translucentMeshes.pop_back();
			index--;
		}
	}

	{
		const auto localResult = s_constantBuffer_perFrame.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}
	//per draw call cleanup
	{
		const auto localResult = s_constantBuffer_perDrawCall.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}
	{
		const auto localResult = s_samplerState.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}

	{
		const auto localResult = cShader::s_manager.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}

	{
		const auto localResult = sContext::g_context.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}

	return result;
}


eae6320::cResult eae6320::Graphics::InitializeGeometry()
{
	auto result = eae6320::Results::Success;

	return result;
}

eae6320::cResult eae6320::Graphics::InitializeShadingData()
{
	auto result = eae6320::Results::Success;

	return result;
}

