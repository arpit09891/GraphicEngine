#include"View.h"
#include <Engine/Asserts/Asserts.h>
#include "../Graphics/sContext.h"
#include"../Graphics/OpenGL/Includes.h"
#include <Engine/Logging/Logging.h>
#include <Engine/UserOutput/UserOutput.h>


void View::RenderFrame(float red, float green, float blue)
{
	//color buffer
	{
		// Black is usually used
		{
			glClearColor(red, green, blue, 1.0f);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
			glClear(clearColor);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}		
	}	
	//Depth Buffer
	{
		{
			glDepthMask(GL_TRUE);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			glClearDepth(1);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
			glClear(clearDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}

	}
}

void View::SwapBetweenFrontAndBackBuffer()
{
	{
		const auto deviceContext = eae6320::Graphics::sContext::g_context.deviceContext;
		EAE6320_ASSERT(deviceContext != NULL);

		const auto glResult = SwapBuffers(deviceContext);
		EAE6320_ASSERT(glResult != FALSE);
	}
}

void View::CleanUp()
{
}

eae6320::cResult View::InitializeViews(const eae6320::Graphics::sInitializationParameters& i_initializationParameters)
{
	auto result = eae6320::Results::Success;
	return result;
}