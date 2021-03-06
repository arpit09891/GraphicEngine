/*
	This class is your specific game
*/

#ifndef EAE6320_CEXAMPLEGAME_H
#define EAE6320_CEXAMPLEGAME_H

// Include Files
//==============

#include<vector>
#include"Engine\Graphics\SpriteWithEffects.h"
#include"Engine\Graphics\OpaqueMesh.h"
#include"Engine\Graphics\TranslucentMesh.h"
#include"Engine\Graphics\Camera.h"
#include <Engine/Application/cbApplication.h>
#include <Engine/Results/Results.h>
#include"Engine/Graphics/cTexture.h"
#include"GameObject.h"
#include"Collision.h"

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif
//class SpriteWithEffects;
// Class Declaration
//==================

namespace eae6320
{
	class cExampleGame : public Application::cbApplication
	{
		// Inherited Implementation
		//=========================
		std::vector<SpriteWithEffects> v_spriteWithEffects;
		std::vector<OpaqueMesh> v_meshWithEffects;
		std::vector<TranslucentMesh> v_translucentMeshes;
		std::vector<GameObject> v_gameObjects;

		Collision collision;
		Camera camera;
		float red;
		float green;
		float blue;
		std::vector<eae6320::Graphics::cTexture *> v_texture;
		std::vector<eae6320::Graphics::cTexture::Handle> h_texture;
		std::vector<Mesh::Handle> h_Mesh;
		float timer;
		float winTimer;
		int imageCounter;
		int winCounter;
		
	private:

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		virtual const char* GetMainWindowName() const override
		{
			return "Arpit's EAE6320 Example Game"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
				;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		virtual const char* GetMainWindowClassName() const override { return "Arpit's Example Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		virtual const WORD* GetLargeIconId() const override { static constexpr WORD iconId_large = IDI_EAEGAMEPAD; return &iconId_large; }
		virtual const WORD* GetSmallIconId() const override { static constexpr WORD iconId_small = IDI_EAEGAMEPAD; return &iconId_small; }
#endif

		// Run
		//----

		virtual void UpdateBasedOnInput() override;
		virtual void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;

		virtual void UpdateSimulationBasedOnInput() override;
		virtual void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;
		// Initialization / Clean Up
		//--------------------------

		virtual cResult Initialize() override;
		virtual cResult CleanUp() override;

		virtual void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate) override;
		eae6320::cResult LoadTexture(char * textureTarget, eae6320::Graphics::cTexture *& p_Texture);
		eae6320::cResult LoadMesh(char * meshTarget, Mesh *& p_Mesh);
		eae6320::cResult ReleaseTextureHandlers();
		eae6320::cResult ReleaseMeshHandlers();
		bool sortAccordingtoZ(SpriteWithEffects firstMesh, SpriteWithEffects secondMesh);
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, System::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CEXAMPLEGAME_H
