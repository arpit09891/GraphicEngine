// Include Files
//==============

#include "cExampleGame.h"
#include<algorithm>
#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include"../Engine/Graphics/Graphics.h"
#include"Engine\Graphics\SpriteWithEffects.h"
#include"Engine\Graphics\cRenderState.h"
#include"Engine\Math\Functions.h"
#include<iostream>



// Inherited Implementation
//=========================

// Run
//----

void eae6320::cExampleGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

void eae6320::cExampleGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	if (timer >= 0.15)
	{
		timer = 0;
		v_meshWithEffects[1].p_Texture = v_texture[imageCounter];
		
		if (imageCounter == 1) {
			imageCounter = 8;
		}
		else
		{
			imageCounter = 1;
		}
	}

	if (winCounter >= 12)
	{
		v_spriteWithEffects[0].p_Texture = v_texture[9];
		if (winTimer > 3)
		{
			// Exit the application
			const auto result = Exit(EXIT_SUCCESS);
			EAE6320_ASSERT(result);
		}
		winTimer += i_elapsedSecondCount_sinceLastUpdate;

	}
	timer += i_elapsedSecondCount_sinceLastUpdate;	
}

void eae6320::cExampleGame::UpdateSimulationBasedOnInput()
{
	//Moving the mesh
	{
		/*if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
		{
			v_meshWithEffects[0].RigidBody.velocity.y = 0.75f;
		}
		else
		{
			v_meshWithEffects[0].RigidBody.velocity.y = 0.f;
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
		{
			v_meshWithEffects[0].RigidBody.velocity.y = -0.75f;
		}*/
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
		{
			v_meshWithEffects[0].RigidBody.velocity.x = -1.4f;
		}
		else
		{
			v_meshWithEffects[0].RigidBody.velocity.x = 0.f;
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
		{
			v_meshWithEffects[0].RigidBody.velocity.x = 1.4f;
		}
	}
	//moving the camera
	{
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Control))
		{
			camera.RigidBody.velocity.z = 0.85f;
		}
		else
		{
			camera.RigidBody.velocity.z = 0.f;
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Alt))
		{
			camera.RigidBody.velocity.z = -0.85f;
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::F1))
		{
			camera.RigidBody.velocity.y = 0.85f;
		}
		else
		{
			camera.RigidBody.velocity.y = 0.f;
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::F2))
		{
			camera.RigidBody.velocity.y = -0.85f;
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::BackSpace))
		{
			camera.RigidBody.velocity.x = -0.85f;
		}
		else
		{
			camera.RigidBody.velocity.x = 0.f;
		}
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Delete))
		{
			camera.RigidBody.velocity.x = 0.85f;
		}
	}

	//Spawning bullet
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{ 
		v_gameObjects[1].RePosition(v_gameObjects[0].RigidBody);
		v_meshWithEffects[1].RigidBody = v_gameObjects[1].RigidBody;
	}
}

void eae6320::cExampleGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	for (int i = 0; i < v_gameObjects.size(); i++)
	{
		v_meshWithEffects[i].RigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
		//v_translucentMeshes[i].RigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
		if (v_gameObjects[i].IsAlive())
		{
			v_gameObjects[i].RigidBody = v_meshWithEffects[i].RigidBody;
		}
	}
	/*v_meshWithEffects[0].RigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
	v_meshWithEffects[1].RigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);*/
	camera.RigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
	bool isCollided;
	for (size_t i = 2; i < v_gameObjects.size(); i++)
	{
		if (v_gameObjects[1].IsAlive() && v_gameObjects[i].IsAlive())
		{
			isCollided = collision.checkCollision(v_gameObjects[i], v_gameObjects[1]);
			if (isCollided)
			{
				std::cout << "YEs";
			}
		}
	}
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cExampleGame::Initialize()
{
	//Initialize Collision Object
	collision = Collision();

	//Initialising camera
	camera = Camera();
	camera.RigidBody.position.z = 10.f;
	eae6320::Math::cQuaternion isometricAngle = eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(180.0f), eae6320::Math::sVector(0.0f, 1.0f, 0.0f)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(-10.0f), eae6320::Math::sVector(0.0f, 1.0f, 0.0f));
	//camera.RigidBody.orientation = isometricAngle;
	
	//Initialising game objects
	
	auto result = eae6320::Results::Success;
	timer = 0;
	winTimer = 0;
	imageCounter = 1;
	winCounter = 0;

	SpriteWithEffects spriteWithEffects = SpriteWithEffects();
	OpaqueMesh opaqueMesh = OpaqueMesh();
	TranslucentMesh translucentMesh = TranslucentMesh();
	GameObject gameObject = GameObject();
	//Initialising Textures
	eae6320::Graphics::cTexture *tempHandle;
	result = LoadTexture("Red", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("Yellow", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("hull", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("eae", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("Silver", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("Green", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("Blue", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("Space", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("Pink", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);

	result = LoadTexture("YouWin", tempHandle);
	if (!result) {
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	v_texture.push_back(tempHandle);
	
	//Background Color
	{
		red = 1.0f;
		green = 0.5f;
		blue = 0.25f;
	}

	//Initialising sprite with effect
	float width = 0.5f;
	float height = 2.0f;

	//Player Mesh
	{	
		Mesh * mesh1;
		result = LoadMesh("Player", mesh1);
		if (!result) {
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		char* vertexTarget = "MeshVertex";
		char* fragmentTarget = "MeshFragment";
		uint8_t renderState = eae6320::Graphics::RenderStates::eRenderState::DepthBuffering;
		Effects * effect1 = opaqueMesh.EffectData(vertexTarget,fragmentTarget, renderState);
		
		if (effect1 == nullptr)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		
		opaqueMesh.p_Texture = v_texture[2];

		opaqueMesh.BindMeshAndEffect(mesh1, effect1);
		opaqueMesh.RigidBody.position.x = 0.f;
		opaqueMesh.RigidBody.position.y = -6.8f;
		opaqueMesh.RigidBody.position.z = 0.f;
		v_meshWithEffects.push_back(opaqueMesh);
		
		//adding player to gameobjects

		gameObject.maxHealth = 3;
		gameObject.currentHealth = gameObject.maxHealth;
		gameObject.height = 0.1f;
		gameObject.width = 0.1f;
		//gameObject.opaqueMesh = &v_meshWithEffects;
		v_gameObjects.push_back(gameObject);
	}
	//Bullet
	{
		Mesh * mesh1;
		result = LoadMesh("Bullet", mesh1);
		if (!result) {
			EAE6320_ASSERT(false);
			goto OnExit;
		}

		char* vertexTarget = "MeshVertex";
		char* fragmentTarget = "MeshFragment";
		uint8_t renderState = eae6320::Graphics::RenderStates::eRenderState::DepthBuffering;
		Effects * effect1 = opaqueMesh.EffectData(vertexTarget, fragmentTarget, renderState);

		if (effect1 == nullptr)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERT(false);
			goto OnExit;
		}

		opaqueMesh.p_Texture = v_texture[1];
		opaqueMesh.BindMeshAndEffect(mesh1, effect1);
		opaqueMesh.RigidBody.position.x = -0.1f;
		opaqueMesh.RigidBody.position.y = -2.6f;
		opaqueMesh.RigidBody.position.z = 0.f;
		opaqueMesh.RigidBody.velocity.y = 3.0f;
		v_meshWithEffects.push_back(opaqueMesh);

		//adding bullet to gameobject

		gameObject.maxHealth = 1;
		gameObject.currentHealth = 0;/*gameObject.maxHealth;*/
		gameObject.RigidBody = opaqueMesh.RigidBody;
		gameObject.width = 0.15f;
		gameObject.height = 0.15f;
		//auto opaqueGameObject = v_meshWithEffects.crbegin();
		//gameObject.opaqueMesh = &(*v_meshWithEffects.crbegin());
		v_gameObjects.push_back(gameObject);
	}
	//Enemy Meshes
	float xPost = 0.0f;;
	float yPost = 0.0f;
	for (int i = 0; i < 12; i++)
	{
		//Enemy Mesh
		{
			Mesh * mesh1;
			result = LoadMesh("Enemy", mesh1);
			if (!result) {
				EAE6320_ASSERT(false);
				goto OnExit;
			}

			char* vertexTarget = "MeshVertex";
			char* fragmentTarget = "MeshFragment";
			uint8_t renderState = eae6320::Graphics::RenderStates::eRenderState::DepthBuffering;
			Effects * effect1 = opaqueMesh.EffectData(vertexTarget, fragmentTarget, renderState);

			if (effect1 == nullptr)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERT(false);
				goto OnExit;
			}

			opaqueMesh.p_Texture = v_texture[0];
			opaqueMesh.BindMeshAndEffect(mesh1, effect1);
			opaqueMesh.RigidBody.position.x = -3.5f + xPost +(xPost* 0.5f);
			opaqueMesh.RigidBody.position.y = 3.5f - yPost;
			opaqueMesh.RigidBody.position.z = 0.f;
			opaqueMesh.RigidBody.velocity.y = 0.f;
			v_meshWithEffects.push_back(opaqueMesh);
			
			//adding enemies to gameobject

			gameObject.maxHealth = 3;
			gameObject.currentHealth =/* 1;*/ gameObject.maxHealth;
			gameObject.RigidBody = opaqueMesh.RigidBody;
			gameObject.width = 1.5f;
			gameObject.height = 1.5f;
			//gameObject.opaqueMesh = &v_meshWithEffects;
			v_gameObjects.push_back(gameObject);
		}
		xPost++;
		
		if (i == 5)
		{
			xPost = 0.f;
			yPost = 1.f;
		}
	}
	//Roof
	{
		Mesh * mesh1;
		result = LoadMesh("Roof", mesh1);
		if (!result) {
			EAE6320_ASSERT(false);
			goto OnExit;
		}

		char* vertexTarget = "MeshVertex";
		char* fragmentTarget = "MeshFragment";
		uint8_t renderState = eae6320::Graphics::RenderStates::eRenderState::DepthBuffering;
		Effects * effect1 = opaqueMesh.EffectData(vertexTarget, fragmentTarget, renderState);

		if (effect1 == nullptr)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERT(false);
			goto OnExit;
		}

		opaqueMesh.p_Texture = v_texture[3];
		opaqueMesh.BindMeshAndEffect(mesh1, effect1);
		opaqueMesh.RigidBody.position.x = 0.f;
		opaqueMesh.RigidBody.position.y = 6.5f;
		opaqueMesh.RigidBody.position.z = 0.f;
		opaqueMesh.RigidBody.velocity.y = 0.f;
		v_meshWithEffects.push_back(opaqueMesh);

		//adding bullet to gameobject

		gameObject.maxHealth = 10000;
		gameObject.currentHealth = gameObject.maxHealth;
		gameObject.width = 100.0f;
		gameObject.height = 5.1f;
		v_gameObjects.push_back(gameObject);
	}
	////////

	//Translucent Enemy Meshes
	xPost = 0.0f;;
	yPost = 0.0f;
	for (int i = 0; i < 12; i++)
	{
		//Enemy Mesh
		{
			Mesh * mesh1;
			result = LoadMesh("Enemy", mesh1);
			if (!result) {
				EAE6320_ASSERT(false);
				goto OnExit;
			}

			char* vertexTarget = "MeshVertex";
				char* fragmentTarget = "MeshFragmentTranslucent";
				uint8_t renderState = 0/* = eae6320::Graphics::RenderStates::eRenderState::DepthBuffering*/;
				eae6320::Graphics::RenderStates::EnableAlphaTransparency(renderState);
				eae6320::Graphics::RenderStates::EnableDepthBuffering(renderState);
				Effects * effect1 = translucentMesh.EffectData(vertexTarget, fragmentTarget, renderState);

			if (effect1 == nullptr)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERT(false);
				goto OnExit;
			}

			translucentMesh.p_Texture = v_texture[0];
			translucentMesh.BindMeshAndEffect(mesh1, effect1);
			translucentMesh.RigidBody.position.x = -3.5f + xPost + (xPost* 0.5f);
			translucentMesh.RigidBody.position.y = 3.5f - yPost;
			translucentMesh.RigidBody.position.z = 0.f;
			v_translucentMeshes.push_back(translucentMesh);
		}
		xPost++;

		if (i == 5)
		{
			xPost = 0.f;
			yPost = 1.f;
		}
	}


	eae6320::Graphics::VertexFormats::sSprite center;
	//Sprite1 with sprite effect
	{
		width = 4.f;
		height = 4.f;
		center.x = 0.f;
		center.y = 0.f;

		Sprite * sprite1 = spriteWithEffects.SubmitSpriteData(width, height, center);
		if (sprite1 == nullptr)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		char* vertexTarget = "SpriteVertex";
		char* fragmentTarget = "SpriteFragment";
		uint8_t renderState = 0;
		Effects * effect1 = spriteWithEffects.EffectData(vertexTarget, fragmentTarget, renderState);

		if (effect1 == nullptr)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERT(false);
			goto OnExit;
		}

		spriteWithEffects.p_Texture = v_texture[7];
		spriteWithEffects.BindSpriteAndEffect(sprite1, effect1);
		v_spriteWithEffects.push_back(spriteWithEffects);
	}


	for (size_t i = 0; i < v_gameObjects.size(); i++ )
	{
		v_gameObjects[i].opaqueMesh = &v_meshWithEffects[i];
		if (i > 1 && i < v_gameObjects.size() - 1)
			v_gameObjects[i].translucentMesh = &v_translucentMeshes[i - 2];		
	}
OnExit:

	return result;
}


void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	std::vector<OpaqueMesh> simulation_meshWithEffects;
	std::vector<TranslucentMesh> simulation_translucentMeshes;
	int translucentCount = 0;
	int opaqueCount = 0;
	winCounter = 0;
	for(size_t i = 0; i < (v_gameObjects.size()); i++)
	{
		if (v_gameObjects[i].IsAlive())
		{
			if (v_gameObjects[i].maxHealth - v_gameObjects[i].currentHealth == 2 && v_gameObjects[i].currentHealth < 3)
			{
				TranslucentMesh* tempTranslucentMesh;
				tempTranslucentMesh = (TranslucentMesh*)v_gameObjects[i].translucentMesh;
				tempTranslucentMesh->transform_localToWorld = eae6320::Math::cMatrix_transformation(v_gameObjects[i].translucentMesh->RigidBody.orientation, v_gameObjects[i].translucentMesh->RigidBody.position);
				TranslucentMesh tempMesh = *v_gameObjects[i].translucentMesh;
				tempMesh.RigidBody.position = tempMesh.RigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
				//changing Texture
				tempMesh.p_Texture = v_texture[5];
				simulation_translucentMeshes.push_back(tempMesh);
				translucentCount++;
			}
			else if (v_gameObjects[i].maxHealth - v_gameObjects[i].currentHealth == 1)
			{
				OpaqueMesh* tempOpaqueMesh;
				tempOpaqueMesh = (OpaqueMesh*)v_gameObjects[i].opaqueMesh;
				tempOpaqueMesh->transform_localToWorld = eae6320::Math::cMatrix_transformation(v_gameObjects[i].opaqueMesh->RigidBody.orientation, v_gameObjects[i].opaqueMesh->RigidBody.position);
				OpaqueMesh tempMesh = *v_gameObjects[i].opaqueMesh;
				tempMesh.RigidBody.position = tempMesh.RigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
				//changing Texture
				tempMesh.p_Texture = v_texture[4];
				simulation_meshWithEffects.push_back(tempMesh);
				opaqueCount++;
			}
			else
			{
				OpaqueMesh* tempOpaqueMesh;
				tempOpaqueMesh = (OpaqueMesh*)v_gameObjects[i].opaqueMesh;
				tempOpaqueMesh->transform_localToWorld = eae6320::Math::cMatrix_transformation(v_gameObjects[i].opaqueMesh->RigidBody.orientation, v_gameObjects[i].opaqueMesh->RigidBody.position);
				OpaqueMesh tempMesh = *v_gameObjects[i].opaqueMesh;
				tempMesh.RigidBody.position = tempMesh.RigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
				simulation_meshWithEffects.push_back(tempMesh);
				opaqueCount++;
			}
		}
		else
		{
			if (i > 1)
			{
				winCounter++;
			}
		}
	}	

	camera.UpdateTransformWorldToCamera();
	camera.UpdateTransformCameraToProjected();
	//simulation camera
	//transform_worldToCamera * transform_localToWorld.GetTranslation();
	Camera tempCamera = camera;
	tempCamera.RigidBody.position = tempCamera.RigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);	
	tempCamera.UpdateTransformWorldToCamera();
	//for sorting translucent objects
	for (size_t i = 0; i < simulation_translucentMeshes.size(); i++)
	{
		simulation_translucentMeshes[i].objectWRTOCamera = tempCamera.transform_WorldToCamera * simulation_translucentMeshes[i].transform_localToWorld.GetTranslation();
	}
	std::sort(simulation_translucentMeshes.begin(), simulation_translucentMeshes.end(),
		[](TranslucentMesh const &mesh1, TranslucentMesh const &mesh2) {
		return(mesh1.objectWRTOCamera.z > mesh2.objectWRTOCamera.z);
	});

	tempCamera.UpdateTransformCameraToProjected();
	
	//Submitting all gameobjects to graphics to render
	Graphics::SubmitCamera(tempCamera);
	Graphics::ChangeBackgroundColor(red, green, blue);
	Graphics::SubmitSpriteAndEffectData(v_spriteWithEffects);
	Graphics::SubmitMeshAndEffectData(simulation_meshWithEffects);
	Graphics::SubmitTranslucentMeshes(simulation_translucentMeshes);

}


bool eae6320::cExampleGame::sortAccordingtoZ(SpriteWithEffects firstMesh, SpriteWithEffects secondMesh)
{
	return (firstMesh.RigidBody.position.z > secondMesh.RigidBody.position.z);
}


eae6320::cResult eae6320::cExampleGame::LoadTexture(char * textureTarget, eae6320::Graphics::cTexture *& p_Texture)
{
	auto result = eae6320::Results::Success;
	eae6320::Graphics::cTexture::Handle s_Texture;
	char filepathTexture[100];
	sprintf(filepathTexture, "data/Textures/%s.texture", textureTarget);
	if (!(result = eae6320::Graphics::cTexture::s_manager.Load(filepathTexture,
		s_Texture)))
	{
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	h_texture.push_back(s_Texture);
	p_Texture = eae6320::Graphics::cTexture::s_manager.Get(s_Texture);
	//p_Texture->IncrementReferenceCount();
OnExit:

	return result;
}

eae6320::cResult eae6320::cExampleGame::LoadMesh(char * meshTarget, Mesh *& p_Mesh)
{
	auto result = eae6320::Results::Success;
	Mesh::Handle s_Mesh;
	char filepathMesh[100];
	sprintf(filepathMesh, "data/Meshes/%s.binary", meshTarget);

	if (!(result = Mesh::s_manager.Load(filepathMesh,
		s_Mesh)))
	{
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	h_Mesh.push_back(s_Mesh);
	p_Mesh = Mesh::s_manager.Get(s_Mesh);
	//p_Mesh->IncrementReferenceCount();
OnExit:

	return result;
}

eae6320::cResult eae6320::cExampleGame::CleanUp()
{
	size_t index = v_spriteWithEffects.size();
	
	while (!v_spriteWithEffects.empty()) {
		SpriteWithEffects spriteWithEffect = v_spriteWithEffects.at(index - 1);
		spriteWithEffect.CleanUp();
		v_spriteWithEffects.pop_back();
		index--;
	}

	//TODO  Find clean up is require or not
	/*index = v_gameObjects.size();
	while (!v_gameObjects.empty()) {
		GameObject gameobject = v_gameObjects.at(index - 1);
		gameobject.CleanUp();
		v_gameObjects.pop_back();
		index--;
	}*/

	index = v_meshWithEffects.size();
	while (!v_meshWithEffects.empty()) {
		OpaqueMesh meshWithEffect = v_meshWithEffects.at(index - 1);
		meshWithEffect.mesh->CleanUp();
		meshWithEffect.effect->CleanUp();
		meshWithEffect.p_Texture->DecrementReferenceCount();
		meshWithEffect.CleanUp();
		v_meshWithEffects.pop_back();
		index--;
	}

	index = v_translucentMeshes.size();
	while (!v_translucentMeshes.empty()) {
		TranslucentMesh meshWithEffect = v_translucentMeshes.at(index - 1);
		meshWithEffect.CleanUp();
		v_translucentMeshes.pop_back();
		index--;
	}

	index = v_texture.size();

	while (!v_texture.empty()) {

		v_texture.at(index - 1)->DecrementReferenceCount();
		v_texture.pop_back();
		index--;
	}

	ReleaseTextureHandlers();
	ReleaseMeshHandlers();
	return Results::Success;
}


eae6320::cResult eae6320::cExampleGame::ReleaseTextureHandlers()
{
	auto result = eae6320::Results::Success;
	size_t index = h_texture.size();
	
	while (!h_texture.empty()) {
		eae6320::Graphics::cTexture::s_manager.Release(h_texture.at(index - 1));
		
		h_texture.pop_back();
		index--;
	}
	return result;
}

eae6320::cResult eae6320::cExampleGame::ReleaseMeshHandlers()
{
	auto result = eae6320::Results::Success;
	size_t index = h_Mesh.size();

	while (!h_Mesh.empty()) {
		Mesh::s_manager.Release(h_Mesh.at(index - 1));

		h_Mesh.pop_back();
		index--;
	}
	return result;
}
