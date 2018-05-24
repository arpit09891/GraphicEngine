#pragma once

#include "../Graphics/VertexFormats.h"
#include"../Platform/Platform.h"
#include"../Assets/ReferenceCountedAssets.h"

#if defined( EAE6320_PLATFORM_D3D )
struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11DeviceContext;

#elif EAE6320_PLATFORM_GL
#include"OpenGL\Includes.h"
#endif

class Sprite{

	EAE6320_ASSETS_DECLAREREFERENCECOUNT();
public:	
	static bool Factory(Sprite*& sprite);
	void Draw();
	EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();

#if EAE6320_PLATFORM_D3D
	ID3D11Buffer* s_vertexBuffer = nullptr;
	ID3D11InputLayout* s_vertexInputLayout = nullptr;
	ID3D11DeviceContext* direct3dImmediateContext;
	void CleanUp();
#elif EAE6320_PLATFORM_GL
	GLuint s_vertexArrayId = 0;
	GLuint s_vertexBufferId = 0;
	eae6320::cResult CleanUp();
#endif
	eae6320::cResult InitializeGeometry(float width, float height, eae6320::Graphics::VertexFormats::sSprite center);
	EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(Sprite);
private:

	Sprite() = default;
	~Sprite() = default;
};