#pragma once
//#include "../Graphics/sContext.h"

#include"Graphics.h"
#include"../Platform/Platform.h"

#if defined( EAE6320_PLATFORM_D3D )
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Buffer;
struct ID3D11InputLayout;
#endif

class View {

	uint16_t resolutionWidth, resolutionHeight;
	

#if defined (EAE6320_PLATFORM_D3D)
	ID3D11RenderTargetView* s_renderTargetView = nullptr;
	ID3D11DepthStencilView* s_depthStencilView = nullptr;
	ID3D11Buffer* s_vertexBuffer = nullptr;
	ID3D11InputLayout* s_vertexInputLayout = nullptr;


#elif defined (EAE6320_PLATFORM_GL)
	
	// A vertex buffer holds the data for each vertex
	GLuint s_vertexBufferId = 0;
	// A vertex array encapsulates the vertex data as well as the vertex input layout
	GLuint s_vertexArrayId = 0;

#endif

public:

	eae6320::cResult InitializeViews(const eae6320::Graphics::sInitializationParameters& i_initializationParameters);
	void RenderFrame(float red, float green, float blue);
	void SwapBetweenFrontAndBackBuffer();
	void CleanUp();

};