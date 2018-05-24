#include <Engine/Asserts/Asserts.h>
#include"sContext.h"
//#include"VertexFormats.h"
#include"../Graphics/Direct3D/Includes.h"
#include <Engine/Logging/Logging.h>
#include <Engine/UserOutput/UserOutput.h>
#include"../Graphics/Sprite.h"

bool Sprite::Factory(Sprite *& sprite)
{
	Sprite * tempSprite = new Sprite();
	sprite = tempSprite;

	if (sprite == nullptr)
		return false;
	else
		return true;
	return false;
}

void Sprite::Draw()
{	
	// Bind a specific vertex buffer to the device as a data source
	{

		direct3dImmediateContext = eae6320::Graphics::sContext::g_context.direct3dImmediateContext;
		EAE6320_ASSERT(direct3dImmediateContext);

		EAE6320_ASSERT(s_vertexBuffer);
		constexpr unsigned int startingSlot = 0;
		constexpr unsigned int vertexBufferCount = 1;
		// The "stride" defines how large a single vertex is in the stream of data
		constexpr unsigned int bufferStride = sizeof(eae6320::Graphics::VertexFormats::sSprite);
		// It's possible to start streaming data in the middle of a vertex buffer
		constexpr unsigned int bufferOffset = 0;
		direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &s_vertexBuffer, &bufferStride, &bufferOffset);
	}

	{
		// Set the layout (which defines how to interpret a single vertex)
		{
			EAE6320_ASSERT(s_vertexInputLayout);
			direct3dImmediateContext->IASetInputLayout(s_vertexInputLayout);
		}
		// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
		// the vertex buffer was defined as a triangle list
		// (meaning that every primitive is a triangle and will be defined by three vertices)
		direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	// Render triangles from the currently-bound vertex buffer
	{
		// As of this comment only a single triangle is drawn
		// (you will have to update this code in future assignments!)
		constexpr unsigned int triangleCount = 2;;
		constexpr unsigned int vertexCountPerTriangle = 3;
		constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
		// It's possible to start rendering primitives in the middle of the stream
		constexpr unsigned int indexOfFirstVertexToRender = 0;
		direct3dImmediateContext->Draw(vertexCountToRender, indexOfFirstVertexToRender);
	}
}


void Sprite::CleanUp()
{
	if (m_referenceCount > 1)
	{
		DecrementReferenceCount();
		
	}
	else
	{
		if (s_vertexBuffer)
		{
			s_vertexBuffer->Release();
			s_vertexBuffer = nullptr;
		}
		if (s_vertexInputLayout)
		{
			s_vertexInputLayout->Release();
			s_vertexInputLayout = nullptr;
		}
		DecrementReferenceCount();
	}
}

eae6320::cResult Sprite::InitializeGeometry(float width, float height, eae6320::Graphics::VertexFormats::sSprite center)
{
	auto result = eae6320::Results::Success;

	auto* const direct3dDevice = eae6320::Graphics::sContext::g_context.direct3dDevice;
	EAE6320_ASSERT(direct3dDevice);

	// Initialize vertex format
	{
		// Load the compiled binary vertex shader for the input layout
		eae6320::Platform::sDataFromFile vertexShaderDataFromFile;
		std::string errorMessage;
		if (result = eae6320::Platform::LoadBinaryFile("data/Shaders/Vertex/vertexInputLayout_Sprite.shd", vertexShaderDataFromFile, &errorMessage))
		{
			// Create the vertex layout

			// These elements must match the VertexFormats::sGeometry layout struct exactly.
			// They instruct Direct3D how to match the binary data in the vertex buffer
			// to the input elements in a vertex shader
			// (by using so-called "semantic" names so that, for example,
			// "POSITION" here matches with "POSITION" in shader code).
			// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
			constexpr unsigned int vertexElementCount = 2;
			D3D11_INPUT_ELEMENT_DESC layoutDescription[vertexElementCount] = {};
			{
				// Slot 0

				// POSITION
				// 2 floats == 8 bytes
				// Offset = 0
				{
					auto& positionElement = layoutDescription[0];

					positionElement.SemanticName = "POSITION";
					positionElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
					positionElement.Format = DXGI_FORMAT_R32G32_FLOAT;
					positionElement.InputSlot = 0;
					positionElement.AlignedByteOffset = offsetof(eae6320::Graphics::VertexFormats::sSprite, x);
					positionElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					positionElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
				}
			}

			{
				// Slot 1

				// TEXCOORD
				// 2 floats == 8 bytes
				// Offset = 1
				{
					auto& texcoordElement = layoutDescription[1];

					texcoordElement.SemanticName = "TEXCOORD";
					texcoordElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
					texcoordElement.Format = DXGI_FORMAT_R32G32_FLOAT;
					texcoordElement.InputSlot = 0;
					texcoordElement.AlignedByteOffset = offsetof(eae6320::Graphics::VertexFormats::sSprite, u);
					texcoordElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					texcoordElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
				}
			}

			const auto d3dResult = direct3dDevice->CreateInputLayout(layoutDescription, vertexElementCount,
				vertexShaderDataFromFile.data, vertexShaderDataFromFile.size, &s_vertexInputLayout);

			if (FAILED(result))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Geometry vertex input layout creation failed (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to create the geometry vertex input layout (HRESULT %#010x)", d3dResult);
			}

			/*const auto d3dResult2 = direct3dDevice->CreateInputLayout(layoutDescription, vertexElementCount,
				vertexShaderDataFromFile.data, vertexShaderDataFromFile.size, &s_vertexInputLayout);
			if (FAILED(result))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Geometry vertex input layout creation failed (HRESULT %#010x)", d3dResult2);
				eae6320::Logging::OutputError("Direct3D failed to create the geometry vertex input layout (HRESULT %#010x)", d3dResult2);
			}*/

			vertexShaderDataFromFile.Free();
		}
		else
		{
			EAE6320_ASSERTF(false, errorMessage.c_str());
			eae6320::Logging::OutputError("The geometry vertex input layout shader couldn't be loaded: %s", errorMessage.c_str());
			goto OnExit;
		}
	}
	// Vertex Buffer
	{
		constexpr unsigned int triangleCount = 2;
		constexpr unsigned int vertexCountPerTriangle = 3;
		const auto vertexCount = triangleCount * vertexCountPerTriangle;
		eae6320::Graphics::VertexFormats::sSprite vertexData[vertexCount];		
		{
			vertexData[0].x = center.x -(width/2);
			vertexData[0].y = center.y - (height/2);
			vertexData[0].u = 0;
			vertexData[0].v = 1;

			vertexData[1].x = center.x + (width / 2); 
			vertexData[1].y = center.y + (height / 2);
			vertexData[1].u = 1;
			vertexData[1].v = 0;

			vertexData[2].x = center.x + (width / 2);  
			vertexData[2].y = center.y - (height / 2); 
			vertexData[2].u = 1;
			vertexData[2].v = 1;

			vertexData[3].x = center.x - (width / 2);
			vertexData[3].y = center.y - (height / 2);
			vertexData[3].u = 0;
			vertexData[3].v = 1;

			vertexData[4].x = center.x - (width / 2);
			vertexData[4].y = center.y + (height / 2);
			vertexData[4].u = 0;
			vertexData[4].v = 0;

			vertexData[5].x = center.x + (width / 2);
			vertexData[5].y = center.y + (height / 2);
			vertexData[5].u = 1;
			vertexData[5].v = 0;

		}
		D3D11_BUFFER_DESC bufferDescription{};
		{
			const auto bufferSize = vertexCount * sizeof(eae6320::Graphics::VertexFormats::sSprite);
			EAE6320_ASSERT(bufferSize < (uint64_t(1u) << (sizeof(bufferDescription.ByteWidth) * 8)));
			bufferDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used
		}
		D3D11_SUBRESOURCE_DATA initialData{};
		{
			initialData.pSysMem = vertexData;
			// (The other data members are ignored for non-texture buffers)
		}

		const auto d3dResult = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &s_vertexBuffer);
		if (FAILED(d3dResult))
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, "Geometry vertex buffer creation failed (HRESULT %#010x)", d3dResult);
			eae6320::Logging::OutputError("Direct3D failed to create a geometry vertex buffer (HRESULT %#010x)", d3dResult);
			goto OnExit;
		}
	}

OnExit:

	return result;
	
}
