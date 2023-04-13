// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DXmodelClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Paint the main window depending of engine state screen page.
// --------------------------------------------------------------------------------------------

#include "OSengine.h"
#include <d3d11.h>
#if defined DX_ENGINE && DX_ENGINE_LEVEL >= 21
	#include "DX11Class.h"
	#include "DXmodelClass.h"
	#include "mem_leak.h"

	#if   defined DX_ENGINE
		#include "DXengine.h"
	#endif

	#include "fileLoader.h"

namespace DirectX {

	#define CALCULATE_MAX_MIN(vertice){\
	/*Get the smallest vertex */\
	minVertex.x = min(minVertex.x, vertice.x);	/* Find smallest x value in model*/\
	minVertex.y = min(minVertex.y, vertice.y);	/* Find smallest y value in model*/\
	minVertex.z = min(minVertex.z, vertice.z);	/* Find smallest z value in model*/\
	\
	/*Get the largest vertex*/\
	maxVertex.x = max(maxVertex.x, vertice.x);	/* Find largest x value in model*/\
	maxVertex.y = max(maxVertex.y, vertice.y);	/* Find largest y value in model*/\
	maxVertex.z = max(maxVertex.z, vertice.z);	/* Find largest z value in model*/\
	}

DXmodelClass::DXmodelClass(bool model3d, PRIMITIVE_TOPOLOGY primitive, bool computeNormals, bool modelHASshadow, bool modelRENDERshadow)
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567830;

	// VARS:
	// ----------------------------------------------------------------------
	
#if defined DX9sdk
	m_driver9 = NULL;
#endif
	m_driver11 = NULL;
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	m_driver = NULL;
#endif

	// SUPER: ----------------------------------------------------------------------
	m_ObjId = 0;
	ModelShaderType	= SHADER_AUTO;

	Model3D			= model3d;
	ModelHASfog		= false;
	ModelHASlight	= true; // Have to be true!
	ModelHASColorMap = false;
	PrimitiveTopology = primitive;

	PosX = PosY = PosZ = 0;

	SpriteTextureWidth	= NULL;
	SpriteTextureHeight = NULL;

	ModelHASNormals = false;
	ModelcomputeNormals = computeNormals;

	// Public ----------------------------------------------------------------------
	
	
#if defined DX9sdk
	m_Shader9 = NULL;
#endif
	m_Shader11 = NULL;
	m_Shader = NULL;

	//meshSRV
	minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	objectCenterOffset = XMFLOAT4(0, 0, 0, 0);
	boundingSphere = false;

	// Private ----------------------------------------------------------------------
	#ifdef DX11 || defined DX9
		m_vertexBuffer11	= m_indexBuffer11	= NULL;
	#endif

	#if defined DX9sdk
		indexBuffer9 = NULL;
		vertexBuffer9 = NULL;
	#endif

	m_vertexCount	= m_indexCount	= NULL;
	
	Identity();

	m_xTexture = 1.0f;

	meshMatLib = TEXT("");

	//Temp variables to store into vectors
	meshMaterialsTemp	= TEXT("");
	vertPosIndexTemp	= NULL;
    vertNormIndexTemp	= NULL;
	vertTCIndexTemp		= NULL;

	checkChar = NULL;
	face = TEXT("");
	vIndex			= NULL;	//Keep track of our vertex index count
    triangleCount	= NULL;	//Total Triangles
	meshTriangles	= NULL;
	// ----------------------------------------------------------------------
	fileNameOnly = TEXT("");

	//Make sure we have a default if no tex coords or normals are defined
	hasTexCoord = false;	//ch07
	hasNorm = false;		//ch12
	hasRenderShadow = false;
	hasNormMap = false;		//ch51

	hasTransparent = false;
	meshSubsets = NULL;

}

void DXmodelClass::LOADDRIVER(void* driver)
{
	switch (SystemHandle->AppSettings->DRIVER)
	{
	#if defined DX9sdk
	case DRIVER_DX9:
		m_driver9 = (DirectX::DX9Class*)driver;
		break;
	#endif
	#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX9:
		m_driver11 = (DirectX::DX11Class*)driver;
		break;
	#endif

	case DRIVER_DX11:
		m_driver11 = (DirectX::DX11Class*)driver;
		break;

	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX12:
		m_driver = (DirectX::DX12Class*)driver;
		break;
	#endif
	}
}

DXmodelClass::~DXmodelClass() {CLASSDELETE();}

// Load Model in DX Buffers after any "Format" Read:
//
// -------------------	// COLOR
bool DXmodelClass::LoadColor(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType> *model, std::vector<UINT>* indexList, UINT instanceCount)
{
	LOADDRIVER(driver);
	MODEL_NAME = objectName;

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_COLOR;
	else
		ModelShaderType = shader_type;

	ASSERT(ModelShaderType == SHADER_COLOR);

	modelColorVertex = model; //*
	indexModelList = indexList;
	return InitializeDXbuffers(objectName);
}

bool DXmodelClass::LoadTexture(	TCHAR* objectName, void* driver, SHADER_TYPE shader_type, 
								std::vector<STRING> *textureFile, std::vector<ModelTextureVertexType> *model, 
								std::vector<UINT>* indexList, UINT instanceCount)
{
	LOADDRIVER(driver);
	MODEL_NAME = objectName;

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_TEXTURE;
	else
		ModelShaderType = shader_type;

	ASSERT(ModelShaderType == SHADER_TEXTURE || ModelShaderType == SHADER_TEXTURE_FONT);

	modelTextureVertex = model; //*
	indexModelList = indexList;
	return InitializeDXbuffers(objectName, textureFile);
}

bool DXmodelClass::LoadLight(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureLightVertexType> *model, std::vector<UINT>* indexList, UINT instanceCount)
{
	LOADDRIVER(driver);
	MODEL_NAME = objectName;

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_TEXTURE_LIGHT;
	else
		ModelShaderType = shader_type;

		modelTextureLightVertex = model;

		ASSERT( (ModelShaderType == SHADER_TEXTURE_LIGHT) || 
			(ModelShaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW));
	indexModelList = indexList;
	return InitializeDXbuffers(objectName, textureFile);
}

DXshaderClass* DXmodelClass::CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType)
{
	bool			result=false;
	DXshaderClass*	shader=NULL;

	//m_driver = (DX_CLASS*)m_driver;
	switch (SystemHandle->AppSettings->DRIVER)
	{
  #if defined DX9sdk
	case DRIVER_DX9:
		// Create the SHADER object.
		shader = NEW DXshaderClass(m_driver9->ShaderVersionH, m_driver9->ShaderVersionL, Model3D);
		IF_NOT_THROW_EXCEPTION(shader);
		result = shader->Initialize(objectName, ShaderType, ((DirectX::DX9Class*)m_driver9)->m_device, SystemHandle->m_hWnd, PrimitiveTopology);
	break;
  #endif
  #if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX9:
		// Create the SHADER object.
		shader = NEW DXshaderClass(m_driver11->ShaderVersionH, m_driver11->ShaderVersionL, Model3D);
		IF_NOT_THROW_EXCEPTION(shader);
		result = shader->Initialize(objectName, ShaderType, ((DirectX::DX11Class*)m_driver11)->m_device, SystemHandle->m_hWnd, PrimitiveTopology);
	break;
  #endif

	case DRIVER_DX11:
		// Create the SHADER object.
		shader = NEW DXshaderClass(m_driver11->ShaderVersionH, m_driver11->ShaderVersionL, Model3D);
		IF_NOT_THROW_EXCEPTION(shader);
		result = shader->Initialize(objectName, ShaderType, ((DirectX::DX11Class*)m_driver11)->m_device, SystemHandle->m_hWnd, PrimitiveTopology);
	break;

  #if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX12:
		// Create the SHADER object.
		shader = NEW DXshaderClass(m_driver->ShaderVersionH, m_driver->ShaderVersionL, Model3D);
		IF_NOT_THROW_EXCEPTION(shader);
		result = shader->Initialize(objectName, ShaderType, ((DirectX::DX12Class*)m_driver)->m_device, SystemHandle->m_hWnd, PrimitiveTopology);
	break;
  #endif
	}
	if (!result)
	{
		WomaFatalExceptionW(TEXT("Could not initialize the Shader")); return false;
	}

	// GET: m_vertexCount
	switch (ShaderType) 
	{
	case SHADER_COLOR:
		m_vertexCount = (UINT) (*modelColorVertex).size();	// Set the number of vertices in the vertex array.
		if (m_vertexCount == 0)								// Better check, if object is empty...
			return false;		
		break;

	// ----------------------------------------------------------------------------------------------
	case SHADER_TEXTURE:
	case SHADER_TEXTURE_FONT:
		m_vertexCount = (UINT) (*modelTextureVertex).size();	// Set the number of vertices in the vertex array.
		if (m_vertexCount == 0)									// Better check, if object is empty...
			return false;		
		break;

	// ----------------------------------------------------------------------------------------------
	case SHADER_TEXTURE_LIGHT:			    //23
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW: //45
		m_vertexCount = (UINT) (*modelTextureLightVertex).size();	// Set the number of vertices in the vertex array.
		if (m_vertexCount == 0)										// Better check, if object is empty...
			return false;	
		break;

	// ----------------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------------

	default:
		throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
	}

	if (ShaderType >= SHADER_TEXTURE_LIGHT)
		shader->hasLight	= ModelHASlight;	// COLOR AND TEXTURE = FALSE

	return shader;
}

HRESULT DXmodelClass::LoadTextureImage(TCHAR* textureFilename)
{
	HRESULT hr = S_FALSE;

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Texture = NEW DX12TextureClass;
		IF_NOT_THROW_EXCEPTION(m_Texture);
		bool result = m_Texture->Initialize(m_driver, textureFilename, 0, Model3D);	// Initialize the texture object:
		if (result)
		{
			//SHADER_TEXTURE:
			// | Root Signature		| Shader Registers	|
			// |0| DescriptorTable  | b0				|
			// |1| DescriptorTable  | t0				|<-- HERE

			// SHADER_TEXTURE_LIGHT
			// | Root Signature		| Shader Registers	|
			// |0| DescriptorTable  | b0				|
			// |1| DescriptorTable  | t0				|<-- HERE
			// |2| DescriptorTable  | b1				|

			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle1(m_Shader->DX12mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_Shader->m_CbvSrvDescriptorSize, 1); // T0 at: 1
			m_driver->m_device->CreateShaderResourceView(m_Texture->m_pTexture.Get(), &m_driver->DX12viewDesc, cbvHandle1);
			hr = S_OK;
		}
	}

	switch (SystemHandle->AppSettings->DRIVER)
	{
	case DRIVER_DX9:
	case DRIVER_DX11:
		LOADTEXTURE(textureFilename, m_Texture11); // NOTE: Populate hr in case of failor
		break;
	}

#if defined DX9sdk
	hr = D3DXCreateTextureFromFile(((DX_CLASS*)m_driver)->m_device, textureFilename, &m_Texture);
#endif
	if (hr != S_OK)
	{
		WOMA::WomaMessageBox(textureFilename, TEXT("Texture File not found")); return false;
	}
	else
	{
		switch (SystemHandle->AppSettings->DRIVER)
		{
#if defined DX9 && D3D11_SPEC_DATE_YEAR == 2009
		case DRIVER_DX9:
			meshSRV9.push_back(m_Texture9);	// Image Converter: *.png to *.dds: \WoMAengine2014\_LIBS\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft DDS *.png
			break;
#endif
#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX9:
			meshSRV11.push_back(m_Texture11);	// Image Converter: *.png to *.dds: \WoMAengine2014\_LIBS\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft DDS *.png
			break;
#endif

		case DRIVER_DX11:
			meshSRV11.push_back(m_Texture11);	// Image Converter: *.png to *.dds: \WoMAengine2014\_LIBS\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft DDS *.png
			break;

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			meshSRV.push_back(m_Texture);	// Image Converter: *.png to *.dds: \WoMAengine2014\_LIBS\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft DDS *.png
			break;
#endif
		}
	}

	return hr;
}

// COMMON SHADER FUNCTION: LOAD VERTEX+INDEX DATA ON GRPHX. CARD
// --------------------------------------------------------------------------------------------
bool DXmodelClass::InitializeDXbuffers(TCHAR* objectName, std::vector<STRING>* textureFile)
{
	bool result = true;

	
#if defined DX9sdk
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
	{
		m_Shader9 = CreateShader(objectName, ModelShaderType);
		ASSERT(m_Shader9);
	}
#endif
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		m_Shader11 = CreateShader(objectName, ModelShaderType);
		ASSERT(m_Shader11);
	}
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Shader = CreateShader(objectName, ModelShaderType);
		ASSERT(m_Shader);
	}

	// Create TEMP INDEX buffer to copy to GRPHX. CARD
		UINT*	indices = NULL;	//DX10/11/12

	if ( indexModelList == NULL || indexModelList->size() == 0) // BASIC object, without index? One index per vertice?
	{
		m_indexCount = m_vertexCount;			// Set the number of indices in the index array.

			indices = NEW UINT[m_indexCount];		// Create the index array: DX10/11
			IF_NOT_THROW_EXCEPTION(indices);
			// getArrayIndices()
			for (UINT i = 0; i < m_indexCount; i++)
				indices[i] = i;						// Load the index array with data:

	} else {
		m_indexCount = (UINT)indexModelList->size();

			indices = NEW UINT[m_indexCount];		// Create the index array: DX10/11
			IF_NOT_THROW_EXCEPTION(indices);
			// cloneArrayIndices()
			for (UINT i = 0; i < m_indexCount; i++)
				indices[i] = indexModelList->at(i);	// Load the index array with data:
	}

	// Send Buffers to "Grph. Card.": The vertex and index buffer that hold the geometry for the triangles
	// ----------------------------------------------------------------------------------------------
	switch (ModelShaderType)
	{
	case SHADER_COLOR: 
		//result = InitializeColorBuffers(m_driver->m_device, indices);
		switch (SystemHandle->AppSettings->DRIVER)
		{
#if defined DX9sdk
		case DRIVER_DX9:
			result = InitializeColorBuffers(((DirectX::DX9Class*)m_driver9)->m_device, indices9);
		break;
#endif
		case DRIVER_DX9:
			result = InitializeColorBuffers(((DirectX::DX11Class*)m_driver11)->m_device, indices);
		break;

		case DRIVER_DX11:
			result = InitializeColorBuffers(((DirectX::DX11Class*)m_driver11)->m_device, indices);
		break;

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			result = InitializeColorBuffers(((DirectX::DX12Class*)m_driver)->m_device, indices);
		break;
#endif
		
		}
	break;
	
	case SHADER_TEXTURE:
	case SHADER_TEXTURE_FONT:
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			result = InitializeTextureBuffers(m_driver11->m_device, indices);
		}
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
			result = InitializeTextureBuffers(m_driver->m_device, indices);
		}
	break;

	case SHADER_TEXTURE_LIGHT:				//23
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW: //45
	case SHADER_TEXTURE_LIGHT_INSTANCED:	//51
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			result = InitializeTextureLightBuffers(m_driver11->m_device, indices); break;
		}
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
			result = InitializeTextureLightBuffers(m_driver->m_device, indices); break;
		}

	default:
		result = false;
	}

	IF_NOT_RETURN_FALSE(result);

	// Load Texture (Manually)
	// ----------------------------------------------------------------------------------------------


	if (ModelShaderType >= SHADER_TEXTURE)
	{
		HRESULT hr = S_FALSE;

		UINT meshSRV_size = 0;
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			meshSRV_size = (UINT) meshSRV11.size();
		}
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
			meshSRV_size = (UINT) meshSRV.size();
		}
		if (meshSRV_size > 1)
			Sleep(1);

		// Create the texture object for this model:
		if (meshSRV_size == 0)
		{
			for (UINT i = 0; i < (*textureFile).size(); i++)
			{

			// Get full pathname for this texture:
			TCHAR* textureFilename = WOMA::LoadFile((TCHAR*)(*textureFile)[i].c_str());
			LoadTextureImage(textureFilename);
			}
		}

			if (!Model3D)	// SPRITE? Get Size...
			{
				if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
				{
					SpriteTextureWidth = m_Texture->width;
					SpriteTextureHeight = m_Texture->height;
				}

				if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
				{
					// Get Sprite Size:
					ID3D11Resource* textureResource;
					m_Texture11->GetResource(&textureResource);
					ID3D11Texture2D* diffuseTexture = (ID3D11Texture2D*)textureResource;

					// Get the texture Descriptor to obtain texture info:
					D3D11_TEXTURE2D_DESC diffuseTextureDesc;
					diffuseTexture->GetDesc(&diffuseTextureDesc);
					SpriteTextureWidth = diffuseTextureDesc.Width;
					SpriteTextureHeight = diffuseTextureDesc.Height;

					SAFE_RELEASE(textureResource);
				}
			}
	}

	SAFE_DELETE_ARRAY (indices);
	if (Model3D) 
	{
		// Compute distance between maxVertex and minVertex
		float distX = (maxVertex.x - minVertex.x) / 2.0f;
		float distY = (maxVertex.y - minVertex.y) / 2.0f;
		float distZ = (maxVertex.z - minVertex.z) / 2.0f;	

		// Now store the distance between (0, 0, 0) in model space to the models real center
		objectCenterOffset = XMFLOAT4(maxVertex.x - distX, maxVertex.y - distY, maxVertex.z - distZ, 0.0f);

		// Compute bounding sphere (distance between min and max bounding box vertices)
		boundingSphere = XMVectorGetX(XMVector3Length(XMVectorSet(distX, distY, distZ, 0.0f)));	

	}
	return true;
}


void DXmodelClass::Shutdown()
{
	WOMA_LOGManager_DebugMSG("DXmodelClass() DESTROYING: %s\n", MODEL_NAME.c_str());

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		SAFE_RELEASE(m_indexBuffer11);	// Release the index buffer.
		SAFE_RELEASE(m_vertexBuffer11);	// Release the vertex buffer.
	}
#if defined DX9sdk
	SAFE_RELEASE( indexBuffer9 );
	SAFE_RELEASE( vertexBuffer9 );	//TODO: use same name as DX11??
#endif


	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		//DX11 no need to clean, like in DX12!
		//for (UINT i = 0; i < meshSRV11.size(); i++)
		//	free(meshSRV11[i]);
		meshSRV11.clear();
	}

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		for (UINT i = 0; i < meshSRV.size(); i++)
			SAFE_DELETE(meshSRV[i]);
		meshSRV.clear();
	}
		
	
#if defined DX9sdk
	SAFE_SHUTDOWN(m_Shader9);
#endif
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		SAFE_SHUTDOWN(m_Shader11);
	}
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		SAFE_SHUTDOWN(m_Shader);
	}

}

//Ver: 1 + COLOR
bool DXmodelClass::InitializeColorBuffers(/*ID3D11Device*/ void* device, void* indices)
{
	m_vertexCount = (UINT) (*modelColorVertex).size();
	DXcolorVertexType*	vertices = NEW DXcolorVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION (vertices);

	// Load the "DX vertex" array with "WoMA vertex" data:
	// ----------------------------------------------------------------------------------------
	for (UINT i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3((*modelColorVertex)[i].x, (*modelColorVertex)[i].y, (*modelColorVertex)[i].z);
		vertices[i].color	 = XMFLOAT4((*modelColorVertex)[i].r, (*modelColorVertex)[i].g, (*modelColorVertex)[i].b, (*modelColorVertex)[i].a);
#if defined DX9sdk
		vertices[i].position = D3DXVECTOR3((*modelColorVertex)[i].x, (*modelColorVertex)[i].y, (*modelColorVertex)[i].z); 
		vertices[i].color	 = D3DCOLOR_ARGB(	(BYTE)((*modelColorVertex)[i].a*255.0f), 
												(BYTE)((*modelColorVertex)[i].r*255.0f), (BYTE)((*modelColorVertex)[i].g*255.0f), (BYTE)((*modelColorVertex)[i].b*255.0f));
#endif

		CALCULATE_MAX_MIN(vertices[i].position);
	}

	IF_NOT_RETURN_FALSE (CreateDXbuffers(sizeof (DXcolorVertexType), device, indices, vertices));

	// Release the arrays now from RAM, that the vertex and index buffers have been created and loaded to GPU
	// ----------------------------------------------------------------------------------------
	SAFE_DELETE_ARRAY (vertices);

	return true;
}

//Ver: 2: + TEXTURE
bool DXmodelClass::InitializeTextureBuffers(/*ID3D11Device*/ void* device, void* indices)
{
	m_vertexCount = (UINT) (*modelTextureVertex).size();
	DXtextureVertexType* vertices = NEW DXtextureVertexType[m_vertexCount];	// Create the vertex array.
	IF_NOT_THROW_EXCEPTION (vertices);

	// Load the "DX vertex" array with "WoMA vertex" data:
	// ----------------------------------------------------------------------------------------
	if (Model3D) 
	{
		for (UINT i = 0; i < m_vertexCount; i++)
		{
			// Load the vertex array with data.
		#ifndef DX9sdk
			vertices[i].position = XMFLOAT3((*modelTextureVertex)[i].x, (*modelTextureVertex)[i].y, (*modelTextureVertex)[i].z);
			vertices[i].texCoord = XMFLOAT2((*modelTextureVertex)[i].tu, (*modelTextureVertex)[i].tv);
		#else
			vertices[i].position = D3DXVECTOR3((*modelTextureVertex)[i].x, (*modelTextureVertex)[i].y, (*modelTextureVertex)[i].z); 
			vertices[i].texCoord = D3DXVECTOR2((*modelTextureVertex)[i].tu, (*modelTextureVertex)[i].tv);
		#endif

#if _DEBUG
		WOMA_LOGManager_DebugMSG("vertices: %d %d %d - %f %f \n", 
			vertices[i].position.x, vertices[i].position.y, vertices[i].position.z, vertices[i].texCoord.x, vertices[i].texCoord.y);
#endif

			CALCULATE_MAX_MIN(vertices[i].position);
		}
	}

	IF_NOT_RETURN_FALSE (CreateDXbuffers(sizeof (DXtextureVertexType), device, indices, vertices));

	// Release the arrays now from RAM, that the vertex and index buffers have been created and loaded to GPU
	// ----------------------------------------------------------------------------------------
	SAFE_DELETE_ARRAY (vertices);

	return true;
}

//Ver: 3 + LIGHT
bool DXmodelClass::InitializeTextureLightBuffers(/*ID3D11Device*/ void* device, void* indices)
{
	m_vertexCount = (UINT) (*modelTextureLightVertex).size();
	DXtextureLightVertexType* vertices = NEW DXtextureLightVertexType[m_vertexCount];	// Create the vertex array.
	IF_NOT_THROW_EXCEPTION (vertices);

	// Load the "DX vertex" array with "WoMA vertex" data:
	// ----------------------------------------------------------------------------------------
	for (UINT i = 0; i < m_vertexCount; i++)
	{
		// Load the vertex array with data.
	#ifndef DX9sdk
		vertices[i].position = XMFLOAT3((*modelTextureLightVertex)[i].x, (*modelTextureLightVertex)[i].y, (*modelTextureLightVertex)[i].z);
		vertices[i].texCoord = XMFLOAT2((*modelTextureLightVertex)[i].tu, (*modelTextureLightVertex)[i].tv);
		vertices[i].normal = XMFLOAT3((*modelTextureLightVertex)[i].nx, (*modelTextureLightVertex)[i].ny, (*modelTextureLightVertex)[i].nz);
	#else
		vertices[i].position = D3DXVECTOR3((*modelTextureLightVertex)[i].x, (*modelTextureLightVertex)[i].y, (*modelTextureLightVertex)[i].z); 
		vertices[i].texCoord = D3DXVECTOR2((*modelTextureLightVertex)[i].tu, (*modelTextureLightVertex)[i].tv);
		vertices[i].normal = D3DXVECTOR3((*modelTextureLightVertex)[i].nx, (*modelTextureLightVertex)[i].ny, (*modelTextureLightVertex)[i].nz);
	#endif
		CALCULATE_MAX_MIN(vertices[i].position);
	}
	
	IF_NOT_RETURN_FALSE (CreateDXbuffers(sizeof (DXtextureLightVertexType), device, indices, vertices));

	// Release the arrays now from RAM, that the vertex and index buffers have been created and loaded to GPU
	// ----------------------------------------------------------------------------------------
	SAFE_DELETE_ARRAY (vertices);

	return true;
}




// ----------------------------------------------------------------------------------------
bool DXmodelClass::CreateDXbuffers(UINT sizeofMODELvertex_, /*ID3D11Device*/ void* Driver_Device, void* indices, void* vertices)
// ----------------------------------------------------------------------------------------
{
	sizeofMODELvertex = sizeofMODELvertex_;

	ASSERT(m_vertexCount && m_indexCount && vertices && indices && sizeofMODELvertex > 0);

	//DX12
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		ID3D12Device* device = ((ID3D12Device*)Driver_Device);

		//
		// VERTEX:
		//
		const UINT vertexBufferSize = sizeofMODELvertex * m_vertexCount; // sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer)));
		m_vertexBuffer->SetName(L"Vertex Buffer Resource");

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertexBufferUpload)));
		vertexBufferUpload->SetName(L"Vertex Buffer Upload Resource");
		// EQUIVALENT (upper code): Upload the vertex buffer to the GPU.
		{
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = vertices;
			vertexData.RowPitch = vertexBufferSize;
			vertexData.SlicePitch = vertexData.RowPitch;

			UpdateSubresources(m_driver->m_commandList.Get(), m_vertexBuffer.Get(), vertexBufferUpload.Get(), 0, 0, 1, &vertexData);
			m_driver->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
		}

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeofMODELvertex; // sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;

		//
		// INDEX:
		//
		const UINT IndexBufferSize = sizeof(UINT) * m_indexCount; // sizeof(triangleVertices);

		// Create the index buffer resource in the GPU's default heap and copy index data into it using the upload heap.
		// The upload resource must not be released until after the GPU has finished using it.

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_indexBuffer)));
		m_indexBuffer->SetName(L"Index Buffer Resource");

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBufferUpload)));
		indexBufferUpload->SetName(L"Index Buffer Upload Resource");
		// EQUIVALENT (upper code): Upload the index buffer to the GPU.
		{
			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = reinterpret_cast<BYTE*>(indices);
			indexData.RowPitch = IndexBufferSize;
			indexData.SlicePitch = indexData.RowPitch;

			UpdateSubresources(m_driver->m_commandList.Get(), m_indexBuffer.Get(), indexBufferUpload.Get(), 0, 0, 1, &indexData);
			m_driver->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
		}

		// Initialize the index buffer view.
		m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.SizeInBytes = IndexBufferSize;
		m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	}
#endif

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
#define device ((ID3D11Device*)Driver_Device)
		HRESULT result;
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };

		// ----------------------------------------------------------------------------------------
		// Set up the description of the static vertex buffer:
		// ----------------------------------------------------------------------------------------
		if (Model3D)
		{
			// Normal 3D MODEL       
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // Store in "Video Card" Memory (NOTE: D3D11_USAGE_DEFAULT = Decided by Driver...)
			vertexBufferDesc.CPUAccessFlags = 0;
		}
		else {
			// SPRITE 2D Model
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;		// Store in "Shared RAM" Memory (once we need to update)
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU Need to re-write after creation.
		}
		vertexBufferDesc.ByteWidth = sizeofMODELvertex * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//vertexBufferDesc.MiscFlags = 0;
		//vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = { 0 };
		vertexData.pSysMem = vertices;	// Give the subresource structure a pointer to the vertex data.
		//vertexData.SysMemPitch = 0;
		//vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = ((ID3D11Device*)device)->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer11);
		IF_FAILED_RETURN_FALSE(result);

		// ----------------------------------------------------------------------------------------
		// Set up the description of the static index buffer:
		// ----------------------------------------------------------------------------------------
		D3D11_BUFFER_DESC indexBufferDesc = { 0 };
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount; // DWORD = 32 bits of Indexes
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//indexBufferDesc.CPUAccessFlags = 0;
		//indexBufferDesc.MiscFlags = 0;
		//indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		D3D11_SUBRESOURCE_DATA indexData = { 0 };
		indexData.pSysMem = indices;
		//indexData.SysMemPitch = 0;
		//indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = ((ID3D11Device*)device)->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer11);
		IF_FAILED_RETURN_FALSE(result);
	}

	return true;
}


// SetStreamSource
// ----------------------------------------------------------------------------------------
void DXmodelClass::SetBuffers(void* deviceContext)
// ----------------------------------------------------------------------------------------
{
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		ID3D11DeviceContext* context = ((ID3D11DeviceContext*)deviceContext);
		UINT				stride[2];
		UINT				offset[2] = { 0 };
		ID3D11Buffer*		bufferPointer[2];
		UINT				numBuffers = 1;	//Can't be static

		// [1st Buffer]
		bufferPointer[0] =	m_vertexBuffer11;

		switch (ModelShaderType)
		{
		case SHADER_COLOR:
			stride[0] = sizeof(DXcolorVertexType); break;
		case SHADER_TEXTURE:
		case SHADER_TEXTURE_FONT:
			stride[0] = sizeof(DXtextureVertexType); break;

		case SHADER_TEXTURE_LIGHT:					//23
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:		//45
		case SHADER_TEXTURE_LIGHT_INSTANCED:		//51
			stride[0] = sizeof(DXtextureLightVertexType); break;

		case SHADER_TEXTURE_LIGHT_CASTSHADOW:			//45
		case SHADER_TEXTURE_LIGHT_CASTSHADOW_INSTANCED: //51
			stride[0] = sizeof(DXShadowMapVertexType); break;

		case SHADER_NORMAL_BUMP:
		case SHADER_NORMAL_BUMP_INSTANCED:			//51
			stride[0] = sizeof(DXNormalBumpVertexType); break;

		default:
			throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
		}

		// [2nd Buffer]
		/*
		D3D_PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST = 1,
		D3D_PRIMITIVE_TOPOLOGY_LINELIST = 2,
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
		*/
		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)(/*D3D_PRIMITIVE_TOPOLOGY_LINELIST*/PrimitiveTopology));

		// Set the Geometric Data to be sent to Vertex Shader:
		context->IASetVertexBuffers(0, numBuffers, bufferPointer, stride, offset);			// Set the vertex buffer to active in the input assembler so it can be rendered.
		context->IASetIndexBuffer(m_indexBuffer11, DXGI_FORMAT_R32_UINT, 0);					// Set the index buffer to active in the input assembler so it can be rendered.
	}

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles:
		((DirectX::DX12Class*)m_driver)->m_commandList->IASetPrimitiveTopology((D3D12_PRIMITIVE_TOPOLOGY)(PrimitiveTopology)); //D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		((DirectX::DX12Class*)m_driver)->m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		((DirectX::DX12Class*)m_driver)->m_commandList->IASetIndexBuffer(&m_indexBufferView);
	}
#endif
}

void DXmodelClass::UpdateDynamic(void* Driver, std::vector<ModelColorVertexType>* lightVertexVector)
{
	ID3D11DeviceContext* deviceContext11 = NULL;
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11) {
		deviceContext11 = m_driver11->m_deviceContext;
	}

	static float m_previousPosX = -10000;
	static float m_previousPosY = -10000;
	static float m_previousPosZ = -10000;

	//float left, right, top, bottom;
	DXcolorVertexType*			verticesPtr;
	D3D11_MAPPED_SUBRESOURCE	mappedResource;
	HRESULT result;

	float positionX = (*lightVertexVector)[1].x;
	float positionY = (*lightVertexVector)[1].y;
	float positionZ = (*lightVertexVector)[1].z;

	//We check if the position to render this image has changed. If it hasn't changed then we just exit since the vertex buffer doesn't need 
	//any changes for this frame. This check can save us a lot of processing.
	if(((positionX == m_previousPosX) && (positionY == m_previousPosY) && (positionZ == m_previousPosZ)))
		return; //true;

	//If the position to render this image has changed then we record the new location for the next time we come through this function.
	m_previousPosX = positionX;
	m_previousPosY = positionY;
	m_previousPosZ = positionZ;

	//Now that the coordinates are calculated create a temporary vertex array and fill it with the new six vertex points.
	m_vertexCount = (UINT) (*modelColorVertex).size();
	DXcolorVertexType*	vertices = NEW DXcolorVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION (vertices);

	for (UINT i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3((*modelColorVertex)[i].x, (*modelColorVertex)[i].y, (*modelColorVertex)[i].z);
		vertices[i].color	 = XMFLOAT4((*modelColorVertex)[i].r, (*modelColorVertex)[i].g, (*modelColorVertex)[i].b, (*modelColorVertex)[i].a);
	}

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11) {
		//Now copy the contents of the vertex array into the vertex buffer using the Map and memcpy functions:
		result = deviceContext11->Map(m_vertexBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);// Lock the vertex buffer so it can be written to.
		if (FAILED(result)) throw woma_exception("deviceContext->Map!", __FILE__, __FUNCTION__, __LINE__);

		verticesPtr = (DXcolorVertexType*)mappedResource.pData;								// Get a pointer to the data in the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, (sizeof(DXcolorVertexType) * m_vertexCount));		// Copy the data into the vertex buffer.
		deviceContext11->Unmap(m_vertexBuffer11, 0);	// Unlock the vertex buffer.
	}

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12) {
		// VERTEX:
		const UINT vertexBufferSize = sizeofMODELvertex * m_vertexCount; // sizeof(triangleVertices);

		// EQUIVALENT (upper code): Upload the vertex buffer to the GPU.
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = vertices;
		vertexData.RowPitch = vertexBufferSize;
		vertexData.SlicePitch = vertexData.RowPitch;

		UpdateSubresources(m_driver->m_commandList.Get(), m_vertexBuffer.Get(), vertexBufferUpload.Get(), 0, 0, 1, &vertexData);
		m_driver->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
	}

	SAFE_DELETE_ARRAY (vertices);				// Release the vertex array as it is no longer needed.
}


//The UpdateBuffers function is called each frame to update the contents of the dynamic vertex buffer to re-position the 2D bitmap image on the screen if need be.
bool DXmodelClass::UpdateBuffersRotY(void* Driver, int positionX, int positionY)
// ----------------------------------------------------------------------------------------
{
static int m_previousPosX = -10000;
static int m_previousPosY = -10000;

float left, right, top, bottom;
ModelTextureVertexType*		vertices;
ModelTextureVertexType*		verticesPtr;
D3D11_MAPPED_SUBRESOURCE	mappedResource;
HRESULT result;

//We check if the position to render this image has changed. If it hasn't changed then we just exit since the vertex buffer doesn't need 
//any changes for this frame. This check can save us a lot of processing.

	// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.

	static bool RenderfirstTime=true;
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		RenderfirstTime = m_driver11->RenderfirstTime;
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		RenderfirstTime = m_driver->RenderfirstTime;
	if (((positionX == m_previousPosX) && (positionY == m_previousPosY)) && !RenderfirstTime)
		return true;

//If the position to render this image has changed then we record the new location for the next time we come through this function.

	// If it has changed then update the position it is being rendered to.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	//The four sides of the image need to be calculated. See the diagram at the top of the tutorial for a complete explaination.
	left = (float)((SystemHandle->AppSettings->WINDOW_WIDTH / 2) * -1) + (float)positionX;	// Calculate the screen coordinates of the left side of the bitmap.
	right = left + (float)SpriteTextureWidth;												// Calculate the screen coordinates of the right side of the bitmap.
	top = (float)(SystemHandle->AppSettings->WINDOW_HEIGHT / 2) - (float)positionY;			// Calculate the screen coordinates of the top of the bitmap.
	bottom = top - (float)SpriteTextureHeight;												// Calculate the screen coordinates of the bottom of the bitmap.

	//Now that the coordinates are calculated create a temporary vertex array and fill it with the new six vertex points.
	vertices = NEW ModelTextureVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION (vertices);

	// Load the vertex array with data:
/*
	------
	|t1 / |
	|  /  |
	| / t2|
	|/----|
*/
	// First triangle (t1):
	// --------------------
	vertices[0].x = left;
	vertices[0].y = top;
	vertices[0].z = 0;	
	vertices[0].tu=0;
	vertices[0].tv=0;

	vertices[1].x = right;
	vertices[1].y = bottom;
	vertices[1].z = 0;	
	vertices[1].tu= m_xTexture;
	vertices[1].tv= 1;

	vertices[2].x = left;
	vertices[2].y = bottom;
	vertices[2].z = 0;	
	vertices[2].tu= 0;
	vertices[2].tv= 1;

	// Second triangle (t2)
	// --------------------
	vertices[3].x = left;
	vertices[3].y = top;
	vertices[3].z = 0;	
	vertices[3].tu= 0;
	vertices[3].tv= 0;

	vertices[4].x = right;
	vertices[4].y = top;
	vertices[4].z = 0;	
	vertices[4].tu= m_xTexture;
	vertices[4].tv= 0;

	vertices[5].x = right;
	vertices[5].y = bottom;
	vertices[5].z = 0;	
	vertices[5].tu= m_xTexture;
	vertices[5].tv= 1;

	UINT vertexBufferSize = sizeofMODELvertex * m_vertexCount; // sizeof(triangleVertices);

	//Now copy the contents of the vertex array into the vertex buffer using the Map and memcpy functions:
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		result = m_driver11->m_deviceContext->Map(m_vertexBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);// Lock the vertex buffer so it can be written to.
		if(FAILED(result))return false;

		verticesPtr = (ModelTextureVertexType*)mappedResource.pData;	// Get a pointer to the data in the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, vertexBufferSize);			// (sizeof(ModelTextureVertexType) * m_vertexCount)
		m_driver11->m_deviceContext->Unmap(m_vertexBuffer11, 0);		// Unlock the vertex buffer.
	}

	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		// EQUIVALENT (upper code): Upload the vertex buffer to the GPU.
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = vertices;
		vertexData.RowPitch = vertexBufferSize;
		vertexData.SlicePitch = vertexData.RowPitch;

		UpdateSubresources(m_driver->m_commandList.Get(), m_vertexBuffer.Get(), vertexBufferUpload.Get(), 0, 0, 1, &vertexData);
		m_driver->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
	}

	SAFE_DELETE_ARRAY (vertices);				// Release the vertex array as it is no longer needed.

	return true;
}


bool DXmodelClass::RenderSprite(void* Driver, int positionX, int positionY, float scale, float fade)
// ----------------------------------------------------------------------------------------
{	
	model_fade = fade;

	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	switch (SystemHandle->AppSettings->DRIVER)
	{
	case DRIVER_DX9:
	case DRIVER_DX11:
		if (!UpdateBuffersRotY(m_driver11, positionX, positionY))
			return false;
	break;
	#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX12:
		if (!UpdateBuffersRotY(m_driver, positionX, positionY))
			return false;
	break;
	#endif
	}

	//PROJECTION_ORTHOGRAPH:
	m_worldMatrix = XMMatrixIdentity();
	m_worldMatrix._11 = m_worldMatrix._22 = m_worldMatrix._33 = scale;
	m_worldMatrix._42 = +SystemHandle->AppSettings->WINDOW_HEIGHT/2 -m_worldMatrix._33 * SpriteTextureHeight/2;

	// CAMERA:
	//	CAMERA_NORMAL
	//	CAMERA_SKY

	// PROJECTION:
	//	PROJECTION_PERSPECTIVE	//3D
	//	PROJECTION_ORTHOGRAPH	//2D

	switch (SystemHandle->AppSettings->DRIVER)
	{
	case DRIVER_DX9:
	case DRIVER_DX11:
		Render(m_driver11, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH);
	break;
	#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX12:
		Render(m_driver, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH);
	break;
	#endif
	}

	return true;
}


void DXmodelClass::RenderWithFade(WomaDriverClass* Driver, float fadeLight)
{
			if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
				m_Shader11->PSfade = fadeLight;
			if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
			{
				m_Shader->PSfade = fadeLight;
			}
		Render(Driver);
}

void DXmodelClass::RenderSky(WomaDriverClass* driver, UINT camera, float fadeLight)
{
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Shader->PSfade = fadeLight;
		m_Shader->isSky = true;
		Render(m_driver, CAMERA_SKY, PROJECTION_PERSPECTIVE);
	}
#endif
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		m_Shader11->PSfade = fadeLight;
		m_Shader11->isSky = true;
		Render(m_driver11, CAMERA_SKY, PROJECTION_PERSPECTIVE);
	}
#if defined DX9sdk
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		m_Shader->PSfade = fadeLight;
		m_Shader->isSky = true;
		Render(m_driver9, CAMERA_SKY, PROJECTION_PERSPECTIVE);
	}
#endif
	
}

// ----------------------------------------------------------------------------------------
void DXmodelClass::Render(WomaDriverClass* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
// ----------------------------------------------------------------------------------------
{
#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	DX12Class* driver = NULL;
#endif
	DX11Class* driver11 = NULL;
#if defined DX9sdk
	DX9Class* driver9 = NULL;
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		driver = (DX12Class*)Driver;
	}
#endif
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		driver11 = (DX11Class*)Driver;
	}
#if defined DX9sdk
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		driver9 = (DX9Class*)Driver;
#endif

	//	--------------------------------------------------------------------------------------------------------------
	// Turn on/off transparency (depending on the case)
	//	--------------------------------------------------------------------------------------------------------------
#ifdef NOTUSED
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		// Step 1 - Put the "vertex", "index" and "instances"(if exist) buffers on the graphics pipeline to prepare them for drawing:
		// ----------------------------------------------------------------------------------------
		SetBuffers(driver->m_device);

		// Step 2 - Get "view" and "projection" matrices from the "driver" and "camera" objects
		// ----------------------------------------------------------------------------------------
		XMMATRIX* projectionMatrix = driver->GetProjectionMatrix(Driver, camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);
		XMMATRIX* viewMatrix = driver->GetViewMatrix(Driver, camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);

		m_Shader->PSfade = model_fade;
			m_Shader->Render(driver->m_device, m_indexCount, &m_worldMatrix, viewMatrix, projectionMatrix);	// Single Material (Optimized)
	}
#endif
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		 ID3D11DeviceContext* pContext = driver11->m_deviceContext;

		// Step 1 - Put the "vertex", "index" and "instances"(if exist) buffers on the graphics pipeline to prepare them for drawing:
		// ----------------------------------------------------------------------------------------
		SetBuffers(driver11->m_deviceContext);

		// Step 2 - Get "view" and "projection" matrices from the "driver" and "camera" objects
		// ----------------------------------------------------------------------------------------
		XMMATRIX* projectionMatrix = driver11->GetProjectionMatrix(Driver, camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);
		XMMATRIX* viewMatrix = driver11->GetViewMatrix(Driver, camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);

		{
			// Step 3: Render Simple Mesh:
			// ----------------------------------------------------------------------------------------
			if (ModelShaderType >= SHADER_TEXTURE)
				for (UINT i = 0; i < meshSRV11.size(); i++)
					pContext->PSSetShaderResources(i, 1, &meshSRV11[i]);	// Set shader texture resource(s) in the "Pixel Shader", only!
			// 21/22
			m_Shader11->PSfade = model_fade;
			m_Shader11->Render(pContext, m_indexCount, &m_worldMatrix, viewMatrix, projectionMatrix);	// Single Material (Optimized)
		}
	}
#if defined DX9sdk
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
	#define d3ddev m_driver9->m_device

		/*
			static float index = 0.0f;
			index+= 0.01f;    // an ever-increasing float value

			// Frame Move
			//------------------------------------------------------------------------
			D3DXMATRIX matRotateY;    // a matrix to store the rotation information

			// build a matrix to rotate the model based on the increasing float value:
			// [cos# 0 -sin# 0]
			// [   0 1     0 0]
			// [sin# 0 cos#  0]
			// [   0 0     0 1]
			D3DXMatrixRotationY(&matRotateY, index);

			// tell Direct3D about our matrix
			d3ddev->SetTransform(D3DTS_WORLD, &matRotateY);
		*/

		/*
		// States
		//------------------------------------------------------------------------
		if(g_isWireFrame)
			d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		else
			d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
		*/

		// DX9: Samples:
		// http://www.directxtutorial.com/Lesson.aspx?lessonid=9-4-5
		// http://www.codesampler.com/dx9src.htm

		// Step 3: Render Simple Mesh:
		// ----------------------------------------------------------------------------------------
		D3DXMATRIX viewMatrix;
		m_driver9->m_Camera->GetViewMatrix(viewMatrix);

		d3ddev->SetTransform(D3DTS_WORLD, &m_worldMatrix9);
		d3ddev->SetTransform(D3DTS_VIEW, &viewMatrix);    // set the view transform to matView

		if (m_indexCount > 0)
			d3ddev->SetIndices(indexBuffer9);

		switch (ModelShaderType)
		{
		case SHADER_COLOR:		// COLOR
			d3ddev->SetStreamSource(0, vertexBuffer9, 0, sizeof(CUSTOMVERTEX_XYZ_DIFFUSE_DX9));
			d3ddev->SetFVF(CUSTOMFVF_XYZ_DIFFUSE_DX9);	// Type of Vertice: COLOR

			((DirectX::DX9Class*)SystemHandle->m_Driver)->m_device->LightEnable(0, FALSE);
			((DirectX::DX9Class*)SystemHandle->m_Driver)->m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
			break;

		case SHADER_TEXTURE:
			d3ddev->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX_XYZ_TEXTURE_DX9));
			d3ddev->SetFVF(CUSTOMFVF_XYZ_TEXTURE_DX9);	// Type of Vertice: TEXTURE

			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->LightEnable(0, FALSE);
			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
			break;

		case SHADER_TEXTURE_LIGHT:
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
			d3ddev->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX_XYZ_LIGHT_DX9));
			d3ddev->SetFVF(CUSTOMFVF_XYZ_LIGHT_DX9);	// Type of Vertice: LIGHT

			/*
						// Set up a material. The material here just has the diffuse and ambient
						// colors set to yellow. Note that only one material can be used at a time.
						D3DMATERIAL9 mtrl;
						ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
						mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
						mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
						mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
						mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
						((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetMaterial( &mtrl );
			*/

			//((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetLight( 0, &SystemHandle->m_Application->m_Light->light );
			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->LightEnable(0, TRUE);
			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
			/*
			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetRenderState( D3DRS_AMBIENT, 0x00202020 );

		((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		*/
			break;
		}
		if (ModelShaderType >= SHADER_TEXTURE)
			for (UINT i = 0; i < meshSRV.size(); i++)
				d3ddev->SetTexture(i, meshSRV[i]);

#define PrimType D3DPT_TRIANGLESTRIP //D3DPT_TRIANGLELIST
		d3ddev->DrawIndexedPrimitive(PrimType, 0/*VertexStart*/, 0, m_vertexCount, 0/*IndexStart*/, m_indexCount);
	}
#endif
}


// ----------------------------------------------------------------------------------------
// Basic MATH:
// ----------------------------------------------------------------------------------------
void DXmodelClass::Identity()
{
		m_worldMatrix = XMMatrixIdentity();
	#if defined DX9sdk
		D3DXMatrixIdentity(&m_worldMatrix9);
	#endif
}

void DXmodelClass::multiply (void* m) // in radians!!
{
	m_worldMatrix *= *(XMMATRIX*)m;
#if defined DX9sdk
	m_worldMatrix9 *= *(D3DXMATRIX*)m;
#endif
}

void DXmodelClass::rotateX (float rZrad) // in radians!!
{
    XMMATRIX m = XMMatrixRotationX (rZrad);
#if defined DX9sdk
	D3DXMATRIX m;
	D3DXMatrixRotationX(&m, rZrad);
#endif
	m_worldMatrix *= m;
}

void DXmodelClass::rotateY (float rZrad) // in radians!!
{
    XMMATRIX m = XMMatrixRotationY (rZrad);
    m_worldMatrix *= m;
#if defined DX9sdk
	D3DXMATRIX m;
	D3DXMatrixRotationY(&m, rZrad);
#endif
	m_worldMatrix *= m;
}

void DXmodelClass::rotateZ (float rZrad) // in radians!!
{
    XMMATRIX m = XMMatrixRotationZ (rZrad);
    m_worldMatrix *= m;
#if defined DX9sdk
	D3DXMATRIX m;
	D3DXMatrixRotationZ(&m, rZrad);
#endif
	m_worldMatrix *= m;
}

void DXmodelClass::scale(float x, float y, float z)
{
	#if defined _XM_NO_INTRINSICS_
		#if X64
		m_worldMatrix.vector4_f32[0] = x;
		m_worldMatrix.vector4_f32[1] = y;
		m_worldMatrix.vector4_f32[2] = z;
		#else
		m_worldMatrix._41 = x;
		m_worldMatrix._42 = y;
		m_worldMatrix._43 = z;
		#endif
	#else
		//NEED: DEFINE: "_XM_SSE_INTRINSICS_" for fast code
		//#if D3D11_SPEC_DATE_YEAR == 2009
		//	m_worldMatrix._11 = x;
		//	m_worldMatrix._22 = y;
		//	m_worldMatrix._33 = z;
		//#else
			m_worldMatrix.r[0].m128_f32[0] = x;
			m_worldMatrix.r[1].m128_f32[1] = y;
			m_worldMatrix.r[2].m128_f32[2] = z;
		//#endif
	#endif
}

void DXmodelClass::translation(float x, float y, float z)
{
	PosX = x;
	PosY = y;
	PosZ = z;

	#if defined _XM_NO_INTRINSICS_
		#if x64
		m_worldMatrix.vector4_f32[0] = x;
		m_worldMatrix.vector4_f32[1] = y;
		m_worldMatrix.vector4_f32[2] = z;
		#else
		m_worldMatrix._41 = x;
		m_worldMatrix._42 = y;
		m_worldMatrix._43 = z;
		#endif
	#else
		//NEED: DEFINE: "_XM_SSE_INTRINSICS_" for fast code
		//#if D3D11_SPEC_DATE_YEAR == 2009
		//	m_worldMatrix._41 = x;
		//	m_worldMatrix._42 = y;
		//	m_worldMatrix._43 = z;
		//#else
			m_worldMatrix.r[3].m128_f32[0] = x;
			m_worldMatrix.r[3].m128_f32[1] = y;
			m_worldMatrix.r[3].m128_f32[2] = z;
		//#endif
	#endif
}

}

#endif
