// --------------------------------------------------------------------------------------------
// Filename: DXmodelClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
#if defined DX_ENGINE
#include <d3d11.h>
#include "ModelClass.h"
#if defined DX_ENGINE && DX_ENGINE_LEVEL >= 21
#include "DX11Class.h"
#include "DXmodelClass.h"

#if   defined DX_ENGINE
#include "DXengine.h"
#endif

#include "mem_leak.h"

#include "fileLoader.h"

#include "DXrendertextureclass.h"

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
	WomaIntegrityCheck = 1234567222;

	// VARS:
	// ----------------------------------------------------------------------
	
#if defined DX9sdk
	m_driver9 = NULL;
#endif
#if defined DX11 || defined DX9
	m_driver11 = NULL;
#endif
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	m_driver = NULL;
#endif

	// SUPER: ----------------------------------------------------------------------
	m_ObjId = -1;
	ModelShaderType	= SHADER_AUTO;

	Model3D			= model3d;
	ModelHASfog		= false;
	ModelHASlight	= true; // Have to be true!
	ModelHASColorMap = false;
	PrimitiveTopology = primitive;

	PosX = PosY = PosZ = 0;

	SpriteTextureWidth	= NULL;
	SpriteTextureHeight = NULL;

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	ModelCastShadow	  = modelHASshadow;
	ModelRenderShadow = modelRENDERshadow;
#endif

	ModelHASNormals = false;
	ModelcomputeNormals = computeNormals;

	// Public ----------------------------------------------------------------------
	
	
#if defined DX9sdk
	m_Shader9 = NULL;
#endif
#if defined DX11 || defined DX9
	m_Shader11 = NULL;
#endif
#if defined DX12
	m_Shader = NULL;
#endif

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	m_ShaderShadowMap	= NULL;
#endif

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

#if defined USE_VIEW2D_SPRITES
	m_xTexture = 1.0f;
#endif

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

	#if defined DX11
	case DRIVER_DX11:
		m_driver11 = (DirectX::DX11Class*)driver;
		break;
	#endif

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

	ASSERT(	ModelShaderType == SHADER_TEXTURE || 
			ModelShaderType == SHADER_TEXTURE_FONT || 
			ModelShaderType == SHADER_TEXTURE_WATER ||
			ModelShaderType == SHADER_FIRE);

	modelTextureVertex = model;
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

		ASSERT( (ModelShaderType == SHADER_TEXTURE_LIGHT) ||						// 4
				(ModelShaderType == SHADER_TEXTURE_LIGHT_RENDERSHADOW) ||			// 6
				(ModelShaderType == SHADER_TEXTURE_LIGHT_INSTANCED) ||				// 8
				(ModelShaderType == SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED) ||	//10
			    (ModelShaderType == SHADER_FIRE) );
	indexModelList = indexList;
	return InitializeDXbuffers(objectName, textureFile);
}

bool DXmodelClass::LoadBump(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelNormalBumpVertexType> *model, std::vector<UINT>* indexList, UINT instanceCount)
{
	LOADDRIVER(driver);
	MODEL_NAME = objectName;

	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_NORMAL_BUMP;
	else
		ModelShaderType = shader_type;

	ASSERT(ModelShaderType == SHADER_NORMAL_BUMP);

	modelNormalBumpVertex = model; //*
	indexModelList = indexList;
	return InitializeDXbuffers(objectName, textureFile);
}

DXshaderClass* DXmodelClass::CreateShader(TCHAR* objectName, SHADER_TYPE ShaderType)
{
	bool			result=false;
	DXshaderClass*	shader=NULL;

	// LOAD HLSL CODE:
	switch (SystemHandle->AppSettings->DRIVER)
	{
  #if defined DX9sdk
	case DRIVER_DX9:
		// Create the SHADER object / LOAD HLSL ---> return shader as pointer!!
		shader = NEW DXshaderClass(m_driver9->ShaderVersionH, m_driver9->ShaderVersionL, Model3D);
		IF_NOT_THROW_EXCEPTION(shader);
		result = shader->Initialize(m_ObjId, objectName, ShaderType, ((DirectX::DX9Class*)m_driver9)->m_device, SystemHandle->m_hWnd, PrimitiveTopology);
	break;
  #endif
  #if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX9:
		// Create the SHADER object / LOAD HLSL ---> return shader as pointer!!
		shader = NEW DXshaderClass(m_driver11->ShaderVersionH, m_driver11->ShaderVersionL, Model3D);
		IF_NOT_THROW_EXCEPTION(shader);
		result = shader->Initialize(m_ObjId, objectName, ShaderType, ((DirectX::DX11Class*)m_driver11)->m_device11, SystemHandle->m_hWnd, PrimitiveTopology);
	break;
  #endif

  #if defined DX11 // Pure DX11
	case DRIVER_DX11:
		// Create the SHADER object / LOAD HLSL ---> return shader as pointer!!
		shader = NEW DXshaderClass(m_driver11->ShaderVersionH, m_driver11->ShaderVersionL, Model3D);
		IF_NOT_THROW_EXCEPTION(shader);
		result = shader->Initialize(m_ObjId, objectName, ShaderType, ((DirectX::DX11Class*)m_driver11)->m_device11, SystemHandle->m_hWnd, PrimitiveTopology);
	break;
  #endif

  #if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX12:
		// Create the SHADER object / LOAD HLSL ---> return shader as pointer!!
		shader = NEW DXshaderClass(m_driver->ShaderVersionH, m_driver->ShaderVersionL, Model3D);
		IF_NOT_THROW_EXCEPTION(shader);
		result = shader->Initialize(m_ObjId, objectName, ShaderType, ((DirectX::DX12Class*)m_driver)->m_device, SystemHandle->m_hWnd, PrimitiveTopology);
	break;
  #endif
	}
	if (!result)
		{ WomaFatalExceptionW(TEXT("Could not initialize the Shader, error in HLSL code!")); /*return false;*/ }

#if defined DX11 || defined DX12 || defined DX9

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
	case SHADER_TEXTURE_WATER:
	case SHADER_FIRE:
		m_vertexCount = (UINT) (*modelTextureVertex).size();	// Set the number of vertices in the vertex array.
		if (m_vertexCount == 0)									// Better check, if object is empty...
			return false;		
		break;

	// ----------------------------------------------------------------------------------------------
	case SHADER_TEXTURE_LIGHT:			    //23
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW: //36
		m_vertexCount = (UINT) (*modelTextureLightVertex).size();	// Set the number of vertices in the vertex array.
		if (m_vertexCount == 0)										// Better check, if object is empty...
			return false;	
		break;

	// ----------------------------------------------------------------------------------------------
	#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	case SHADER_TEXTURE_LIGHT_SAVESHADOW:
		shader->castShadow = true;
		break;
	#endif

	// ----------------------------------------------------------------------------------------------
	case SHADER_NORMAL_BUMP:
		m_vertexCount = (UINT) (*modelNormalBumpVertex).size();	// Set the number of vertices in the vertex array.
		if (m_vertexCount == 0)									// Better check, if object is empty...
			return false;	
		break;

	// ----------------------------------------------------------------------------------------------

		// ----------------------------------------------------------------------------------------------

	default:
		throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
	}

	if (ShaderType >= SHADER_TEXTURE_LIGHT)
		shader->hasLight	= ModelHASlight;	// COLOR AND TEXTURE = FALSE
	//if (ShaderType == SHADER_TEXTURE_FONT)
	//	shader->isFontShader = ModelIsFont;	// COLOR AND TEXTURE = FALSE
	if (ShaderType >= SHADER_TEXTURE_LIGHT)		// COLOR AND TEXTURE = FALSE
		shader->hasNormMap	= ModelHASNormals;

#endif

	return shader;
}

HRESULT DXmodelClass::LoadTextureImage(TCHAR* textureFilename)
{
	HRESULT hr = S_FALSE;

//#if  DX_ENGINE_LEVEL >= 22 // Texturing 

#if defined DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Texture = NEW DX12TextureClass;
		IF_NOT_THROW_EXCEPTION(m_Texture);
		bool result = m_Texture->Initialize(m_driver, textureFilename, 0, Model3D);	// Initialize the texture object:
		//
		// <SYNC> SHADER IMPUT BLOCKS: DXshaderClass.cpp
		//
		if (result)
		{
				// SHADER_TEXTURE_LIGHT
				// | Root Signature		| Shader Registers	|
				// |0| DescriptorTable  | b0				|
				// |1| DescriptorTable  | b1				|
				// |2| DescriptorTable  | t0				|<-- 2 HERE: 2+
				// ...
				// |10| DescriptorTable | t9				|
			UINT textures_start_descriptor = 1;	//SHADER_TEXTURE
			if (ModelShaderType == SHADER_TEXTURE_LIGHT)
			{
				textures_start_descriptor = 3;	//SHADER_TEXTURE_LIGHT 
			}

			//Prepare Teture to be uploaded from CPU/MEMORY ------> gpu
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle1(m_Shader->DX12mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_Shader->m_CbvSrvDescriptorSize, (UINT)(textures_start_descriptor + meshSRV.size())); // 0 is CBV, so we start textures at 1
			m_driver->m_device->CreateShaderResourceView(m_Texture->m_pTexture.Get(), &m_driver->DX12viewDesc, cbvHandle1);

			hr = S_OK;
		}

		//NEEDTO: SAFE_RELEASE(m_Texture);
	}
#endif

#if defined DX11 || defined DX9
	#if _DEBUG
	WOMA_LOGManager_DebugMSG("Load TEXTURE/MODEL: %s - %s\n", MODEL_NAME.c_str(), textureFilename);
	#endif

	switch (SystemHandle->AppSettings->DRIVER)
	{
	#if defined DX9
	case DRIVER_DX9:
	#endif
	#if defined DX11
	case DRIVER_DX11:
	#endif
		LOADTEXTURE(textureFilename, m_Texture11);
		break;
	}
#endif

#if defined DX9sdk
	hr = D3DXCreateTextureFromFile(((DX_CLASS*)m_driver)->m_device, textureFilename, &m_Texture);
#endif
	if (hr != S_OK)
	{
		WomaMessageBox(textureFilename, TEXT("Texture File not found")); return false;
	}
	else
	{
		switch (SystemHandle->AppSettings->DRIVER)
		{
#if defined DX9 && D3D11_SPEC_DATE_YEAR == 2009
		case DRIVER_DX9:
			meshSRV9.push_back(m_Texture9);		// Image Converter: *.png to *.dds: C:\WoMAengine2023\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft DDS *.png
			break;
#endif
#if defined DX9 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX9:
			meshSRV11.push_back(m_Texture11);	// Image Converter: *.png to *.dds: C:\WoMAengine2023\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft DDS *.png
			break;
#endif

#if defined DX11
		case DRIVER_DX11:
			meshSRV11.push_back(m_Texture11);	// Image Converter: *.png to *.dds: C:\WoMAengine2023\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft DDS *.png
			break;
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			meshSRV.push_back(m_Texture);		// Image Converter: *.png to *.dds: C:\WoMAengine2023\ExternalTools\Microsoft_DirectX_SDK_June_2010\Utilities\bin\x64\texconv.exe -ft DDS *.png
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

#if defined DX11 || defined DX12 || defined DX9
	
#if defined DX9sdk
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		m_Shader9 = CreateShader(objectName, ModelShaderType);
		IF_NOT_RETURN_FALSE(m_Shader9);
	}
#endif
#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		m_Shader11 = CreateShader(objectName, ModelShaderType);
		IF_NOT_RETURN_FALSE(m_Shader11);
			m_Shader11->hasAlfaColor = ModelHASAlfaColor;			//IMPORTANT LEVEL 62!
			m_Shader11->alfaColor = ModelAlfaColor;					//IMPORTANT LEVEL 62!
			m_Shader11->hasFog = ModelHASfog;						//IMPORTANT LEVEL 62!
	}
#endif
#if defined DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Shader = CreateShader(objectName, ModelShaderType);
		IF_NOT_RETURN_FALSE(m_Shader);
	}
#endif

	#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	if (ModelCastShadow && ModelShaderType >= SHADER_TEXTURE_LIGHT)
	{
		m_Shader11->castShadow = true; // Use Shadow Map Result!

		// Create Shadow Map:
			m_ShaderShadowMap = CreateShader (objectName, SHADER_TEXTURE_LIGHT_SAVESHADOW);

		IF_NOT_RETURN_FALSE (m_ShaderShadowMap);
	}
	#endif

#endif

	// Create TEMP INDEX buffer to copy to GRPHX. CARD
	#if defined DX11 || defined DX12 || defined DX9
		UINT*	indices = NULL;	//DX10/11/12
	#else
		WORD*	indices9 = NULL;//DX9/12
	#endif

	if ( indexModelList == NULL || indexModelList->size() == 0) // BASIC object, without index? One index per vertice?
	{
		m_indexCount = m_vertexCount;			// Set the number of indices in the index array.

		#if defined DX11 || defined DX12 || defined DX9
			indices = NEW UINT[m_indexCount];		// Create the index array: DX10/11
			IF_NOT_THROW_EXCEPTION(indices);
			for (UINT i = 0; i < m_indexCount; i++)
				indices[i] = i;						// Load the index array with data:
		#else
			indices9 = NEW WORD[m_indexCount];		// Create the index array: DX9/12
			IF_NOT_THROW_EXCEPTION(indices9);
			for (UINT i = 0; i < m_indexCount; i++)
				indices9[i] = i;						// Load the index array with data:
		#endif

	} else {
		m_indexCount = (UINT)indexModelList->size();

		#if defined DX11 || defined DX12 || defined DX9
			indices = NEW UINT[m_indexCount];		// Create the index array: DX10/11
			IF_NOT_THROW_EXCEPTION(indices);

			for (UINT i = 0; i < m_indexCount; i++)
				indices[i] = indexModelList->at(i);	// Load the index array with data:
		#else
			indices9 = NEW WORD[m_indexCount];		// Create the index array: DX9/12
			IF_NOT_THROW_EXCEPTION(indices9);

			for (UINT i = 0; i < m_indexCount; i++)
				indices9[i] = indexModelList->at(i);// Load the index array with data:
		#endif
	}

	// Convert WOMA-Vertice into DX-Vertice:
	// Send Buffers to "Grph. Card.": The vertex and index buffer that hold the geometry for the triangles
	// ----------------------------------------------------------------------------------------------
	switch (ModelShaderType)
	{
	case SHADER_COLOR: 
		switch (SystemHandle->AppSettings->DRIVER)
		{
#if defined DX9sdk
		case DRIVER_DX9:
			result = InitializeColorBuffers(((DirectX::DX9Class*)m_driver9)->m_device, indices9);
		break;
#endif
#if defined DX9// && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX9:
			result = InitializeColorBuffers(((DirectX::DX11Class*)m_driver11)->m_device11, indices);
		break;
#endif

#if defined DX11 // Pure DX11
		case DRIVER_DX11:
			result = InitializeColorBuffers(((DirectX::DX11Class*)m_driver11)->m_device11, indices);
		break;
#endif

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
		case DRIVER_DX12:
			result = InitializeColorBuffers(((DirectX::DX12Class*)m_driver)->m_device, indices);
		break;
#endif
		
		}
	break;
	
	case SHADER_TEXTURE:
	case SHADER_TEXTURE_FONT:
	case SHADER_TEXTURE_WATER:
	case SHADER_FIRE:
	#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			result = InitializeTextureBuffers(m_driver11->m_device11, indices);
		#if TUTORIAL_CHAP >= 62 // FIRE
				// Set the three scrolling speeds for the three different noise textures.
				m_Shader11->scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

				// Set the three scales which will be used to create the three different noise octave textures.
				m_Shader11->scales = XMFLOAT3(1.0f, 2.0f, 3.0f);

				// Set the three different x and y distortion factors for the three different noise textures.
				m_Shader11->distortion1 = XMFLOAT2(0.1f, 0.2f);
				m_Shader11->distortion2 = XMFLOAT2(0.1f, 0.3f);
				m_Shader11->distortion3 = XMFLOAT2(0.1f, 0.1f);

				// The the scale and bias of the texture coordinate sampling perturbation.
				m_Shader11->distortionScale = 0.8f;
				m_Shader11->distortionBias = 0.5f;
		#endif
		}
	#endif
	#if defined DX12
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
			result = InitializeTextureBuffers(m_driver->m_device, indices);
		}
	#endif
	break;

	case SHADER_TEXTURE_LIGHT:						//23: LIGHT 
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW:			//36: Draw Shadows
	case SHADER_TEXTURE_LIGHT_INSTANCED:			//40: INSTANCED like 23 light, but using Instances
	case SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED: //41
	#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			result = InitializeTextureLightBuffers(m_driver11->m_device11, indices); break;
		}
	#endif
	#if defined DX12
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
			result = InitializeTextureLightBuffers(m_driver->m_device, indices); break;
		}
	#endif

	#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	case SHADER_TEXTURE_LIGHT_SAVESHADOW:			//36: Aux. Shader (render in texture)
	case SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED: //40: Aux. Shader (render in texture), but using Instances
		result = InitializeShadowMapBuffers(m_driver11->m_device11, indices); break;
	#endif

	case SHADER_NORMAL_BUMP:			//35
	case SHADER_NORMAL_BUMP_INSTANCED:  //99
	#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
			if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
			{
				result = InitializeTextureNormalBumpBuffers(m_driver11->m_device11, indices); break;
			}
	#endif
		
	default:
		throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
	}

	IF_NOT_RETURN_FALSE(result);

	// Load Texture (Manually)
	// ----------------------------------------------------------------------------------------------
	if (ModelShaderType >= SHADER_TEXTURE)
	{
		HRESULT hr = S_FALSE;

		UINT meshSRV_size = 0;
		#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9 || SystemHandle->AppSettings->DRIVER == DRIVER_DX11)
		{
			meshSRV_size = (UINT) meshSRV11.size();
		}
		#endif
		#if defined DX12
		if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		{
			meshSRV_size = (UINT) meshSRV.size();
		}
		#endif

		//3D: Create the texture object for this model:
		if (meshSRV_size == 0)
		{
			for (UINT i = 0; i < (*textureFile).size(); i++)
			{
				TCHAR* textureFilename = NULL;
				// Get full pathname for this texture:
				STRING fileNamePath = (TCHAR*)(*textureFile)[i].c_str();
				STRING pathtoengine = TEXT("../");
				if (fileNamePath.substr(0, 3) != pathtoengine)
					textureFilename = WOMA::LoadFile((TCHAR*)fileNamePath.c_str());
				else
					textureFilename = (TCHAR*)fileNamePath.c_str();

				if (fileNamePath.find(TEXT("none")) != 0) //dont load on special cases (like billboards)
				{
				HRESULT res = LoadTextureImage(textureFilename);
				if (res != S_OK)
					return false;
				} 
#if defined DX11
				else
					meshSRV11.push_back(NULL); //on special cases (like billboards)
#endif
			}
		}

		//2D:
		#if defined USE_VIEW2D_SPRITES
			if (!Model3D)	// SPRITE? Get Size...
			{
				#if defined DX12
				if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
				{
					SpriteTextureWidth = m_Texture->width;
					SpriteTextureHeight = m_Texture->height;
				}
				#endif

				#if defined DX11 || defined DX9
				if (m_Texture11 && (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9))
				{
					// Get Sprite Size:
					ID3D11Resource* textureResource;
					m_Texture11->GetResource(&textureResource);
					ID3D11Texture2D* diffuseTexture = (ID3D11Texture2D*)textureResource;

					// Get the texture Descriptor to obtain texture info:
					D3D11_TEXTURE2D_DESC diffuseTextureDesc;
					diffuseTexture->GetDesc(&diffuseTextureDesc);
					if (SpriteTextureWidth==0)
						SpriteTextureWidth = diffuseTextureDesc.Width;
					if (SpriteTextureHeight == 0)
						SpriteTextureHeight = diffuseTextureDesc.Height;

					SAFE_RELEASE(textureResource);
				}
				#endif
			}
		#endif
	}

	SAFE_DELETE_ARRAY (indices);

	//if (Model3D) 
	{
		// Compute distance between maxVertex and minVertex
		float distX = (maxVertex.x - minVertex.x) / 2.0f;
		float distY = (maxVertex.y - minVertex.y) / 2.0f;
		float distZ = (maxVertex.z - minVertex.z) / 2.0f;	

		// Now store the distance between (0, 0, 0) in model space to the models real center
		objectCenterOffset = XMFLOAT4(maxVertex.x - distX, maxVertex.y - distY, maxVertex.z - distZ, 0.0f);

		// Compute bounding sphere (distance between min and max bounding box vertices)
		boundingSphere = XMVectorGetX(XMVector3Length(XMVectorSet(distX, distY, distZ, 0.0f)));	

		#if defined BOUNDINGVOLUMES
		//if (Model3D) 
			CreateBoundingVolumes();
		#endif
	}
	return true;
}

void DXmodelClass::Shutdown()
{
	WOMA_LOGManager_DebugMSG("DXmodelClass() DESTROYING: %s\n", MODEL_NAME.c_str());

#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		SAFE_RELEASE(m_indexBuffer11);	// Release the index buffer.
		SAFE_RELEASE(m_vertexBuffer11);	// Release the vertex buffer.
	}
#endif
#if defined DX9sdk
	SAFE_RELEASE( indexBuffer9 );
	SAFE_RELEASE( vertexBuffer9 );	//TODO: use same name as DX11??
#endif

#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
	#if defined NOTES
		//DX11 no need to clean, like in DX12!
		for (size_t i = 0; i < meshSRV11.size(); i++)
			SAFE_DELETE(meshSRV11[i]);
	#endif
		meshSRV11.clear();
	}
#endif

#if defined DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		for (UINT i = 0; i < meshSRV.size(); i++)
			SAFE_DELETE(meshSRV[i]);
		meshSRV.clear();
	}
#endif
		
#if defined DX12 || defined DX11 || defined DX9
	#if defined DX9sdk
	SAFE_SHUTDOWN(m_Shader9);
	#endif

	#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		SAFE_SHUTDOWN(m_Shader11);
	}
	#endif

	#if defined DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		SAFE_SHUTDOWN(m_Shader);
	}
	#endif
#endif

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	SAFE_SHUTDOWN (m_ShaderShadowMap);
#endif

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
#if defined DX11 || defined DX12 || defined DX9
		vertices[i].position = XMFLOAT3((*modelColorVertex)[i].x, (*modelColorVertex)[i].y, (*modelColorVertex)[i].z);
		vertices[i].color	 = XMFLOAT4((*modelColorVertex)[i].r, (*modelColorVertex)[i].g, (*modelColorVertex)[i].b, (*modelColorVertex)[i].a);
#endif
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

#if _DEBUG && false
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


#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
//Ver: 1 + COLOR
bool DXmodelClass::InitializeShadowMapBuffers(/*ID3D11Device*/ void* device, UINT* indices)
{
	m_vertexCount = (UINT) (*modelShadowMapVertex).size();
	DXShadowMapVertexType*	vertices = NEW DXShadowMapVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION (vertices);

	// Load the "DX vertex" array with "WoMA vertex" data:
	// ----------------------------------------------------------------------------------------
	for (UINT i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3((*modelShadowMapVertex)[i].x, (*modelShadowMapVertex)[i].y, (*modelShadowMapVertex)[i].z);

		CALCULATE_MAX_MIN(vertices[i].position);
	}

	IF_NOT_RETURN_FALSE (CreateDXbuffers(sizeof (DXShadowMapVertexType), device, indices, vertices));

	// Release the arrays now from RAM, that the vertex and index buffers have been created and loaded to GPU
	// ----------------------------------------------------------------------------------------
	SAFE_DELETE_ARRAY (vertices);

	return true;
}
#endif


//Ver: 3 + LIGHT + BUMP
bool DXmodelClass::InitializeTextureNormalBumpBuffers(/*ID3D11Device*/ void* device, UINT* indices)
{
	m_vertexCount = (UINT) (*modelNormalBumpVertex).size();
	DXNormalBumpVertexType* vertices = NEW DXNormalBumpVertexType[m_vertexCount];	// Create the vertex array.
	IF_NOT_THROW_EXCEPTION (vertices);

	// Load the "DX vertex" array with "WoMA vertex" data:
	// ----------------------------------------------------------------------------------------
	for (UINT i = 0; i < m_vertexCount; i++)
	{
		// Load the vertex array with data.
		vertices[i].position	= XMFLOAT3((*modelNormalBumpVertex)[i].x, (*modelNormalBumpVertex)[i].y, (*modelNormalBumpVertex)[i].z);
		vertices[i].texCoord	= XMFLOAT2((*modelNormalBumpVertex)[i].tu, (*modelNormalBumpVertex)[i].tv);
		vertices[i].normal		= XMFLOAT3((*modelNormalBumpVertex)[i].nx, (*modelNormalBumpVertex)[i].ny, (*modelNormalBumpVertex)[i].nz);
		vertices[i].tangent		= XMFLOAT3((*modelNormalBumpVertex)[i].tx, (*modelNormalBumpVertex)[i].ty, (*modelNormalBumpVertex)[i].tz);
		vertices[i].binormal	= XMFLOAT3((*modelNormalBumpVertex)[i].bx, (*modelNormalBumpVertex)[i].by, (*modelNormalBumpVertex)[i].bz);

		CALCULATE_MAX_MIN(vertices[i].position);
	}

	IF_NOT_RETURN_FALSE (CreateDXbuffers(sizeof (DXNormalBumpVertexType), device, indices, vertices));

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
		bool UPLOAD = true;
		if (Model3D) {
			// [*] CreateCommittedResource - VERTEX - ALOCATE SPACE ON GPU RAM
#if defined USE_LIGHT_RAY
			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
				nullptr,
				IID_PPV_ARGS(&m_vertexBuffer)));
			m_vertexBuffer->SetName(L"Vertex Buffer Resource");
#else
		// [*] CreateCommittedResource - VERTEX - ALOCATE SPACE ON GPU RAM
			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_vertexBuffer)));
			m_vertexBuffer->SetName(L"Vertex Buffer Resource");
			UPLOAD = false;
#endif
#if defined USE_LIGHT_RAY
			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //D3D12_HEAP_TYPE_UPLOAD
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, //!!!AQUI!!!
				nullptr,
				IID_PPV_ARGS(&vertexBufferUpload)));
			vertexBufferUpload->SetName(L"Vertex Buffer Upload Resource");
			UPLOAD = true;
#endif
		}
		else
		{
			// [*] CreateCommittedResource - VERTEX - ALOCATE SPACE ON GPU RAM
			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&m_vertexBuffer)));
			m_vertexBuffer->SetName(L"Vertex Buffer Resource");

			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //D3D12_HEAP_TYPE_UPLOAD
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ, //!!!AQUI!!!
				nullptr,
				IID_PPV_ARGS(&vertexBufferUpload)));
			vertexBufferUpload->SetName(L"Vertex Buffer Upload Resource");
			UPLOAD = true;
		}

		if (!UPLOAD) {
			// Copy the triangle data to the vertex buffer.
			UINT8* pVertexDataBegin;
			ThrowIfFailed(m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin)));
			memcpy(pVertexDataBegin, vertices, vertexBufferSize);  //sizeof(triangleVertices)
			m_vertexBuffer->Unmap(0, nullptr);
		}
		else
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
		UPLOAD = true;
		if (Model3D) {
#if defined USE_LIGHT_RAY
			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
				D3D12_RESOURCE_STATE_INDEX_BUFFER,
				nullptr,
				IID_PPV_ARGS(&m_indexBuffer)));
			m_indexBuffer->SetName(L"Index Buffer Resource");
#else
			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_indexBuffer)));
			m_indexBuffer->SetName(L"Index Buffer Resource");
			UPLOAD = false;
#endif
#if defined USE_LIGHT_RAY
			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //D3D12_HEAP_TYPE_UPLOAD
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
				D3D12_RESOURCE_STATE_INDEX_BUFFER, //!!!AQUI!!!
				nullptr,
				IID_PPV_ARGS(&indexBufferUpload)));
			indexBufferUpload->SetName(L"Index Buffer Upload Resource");
			UPLOAD = true;
#endif
		}
		else
		{
			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&m_indexBuffer)));
			m_indexBuffer->SetName(L"Index Buffer Resource");

			ThrowIfFailed(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //D3D12_HEAP_TYPE_UPLOAD
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ, //!!!AQUI!!!
				nullptr,
				IID_PPV_ARGS(&indexBufferUpload)));
			indexBufferUpload->SetName(L"Index Buffer Upload Resource");
			UPLOAD = true;
		}

	if (!UPLOAD) {
		// Copy data
		// Copy the triangle data to the vertex buffer.
		UINT8* pIndexDataBegin;
		ThrowIfFailed(m_indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pIndexDataBegin)));
		memcpy(pIndexDataBegin, indices, IndexBufferSize); // sizeof(triangleVertices)
		m_indexBuffer->Unmap(0, nullptr);
		}
		else
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

#if defined DX11 || defined DX9
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
#elif defined DX9 && D3D11_SPEC_DATE_YEAR == 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
#define device ((IDirect3DDevice9*)Driver_Device)

		// DX9
		//
		// Copy Vertex Data: Create a vertex buffer interface called vertexBuffer
		//
		DWORD FVF = NULL;

		switch (ModelShaderType)
		{
		case SHADER_COLOR:
			FVF = CUSTOMFVF_XYZ_DIFFUSE_DX9;
			break;

		case SHADER_TEXTURE:
		case SHADER_TEXTURE_WATER:
			FVF = CUSTOMFVF_XYZ_TEXTURE_DX9;
			break;

		case SHADER_TEXTURE_LIGHT:
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
			FVF = CUSTOMFVF_XYZ_LIGHT_DX9;
			break;

		default:
			throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
		}

		//
		// Create an vertex buffer:
		//
		device->CreateVertexBuffer(m_vertexCount * sizeofMODELvertex,
			D3DUSAGE_WRITEONLY,
			FVF,
			D3DPOOL_MANAGED,
			&vertexBuffer9,
			NULL);	// parameter "Reserved"

		// Lock vertexBuffer and load the [vertices] into it:
		VOID* pVoid;    // a void pointer
		vertexBuffer9->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, vertices, sizeofMODELvertex * m_vertexCount);
		vertexBuffer9->Unlock();

		if (m_indexCount > 0)
		{
			//
			// Create an index buffer to use with our [indexBuffer] into it:
			//
			((IDirect3DDevice9*)device)->CreateIndexBuffer(m_indexCount * sizeof(WORD),
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_DEFAULT,
				&indexBuffer9,
				NULL);
			WORD* pIndices = NULL;
			indexBuffer9->Lock(0, sizeof(WORD) * m_indexCount, (void**)&pIndices, 0);
			memcpy(pIndices, indices, sizeof(WORD) * m_indexCount);
			indexBuffer9->Unlock();
		}
	}
#endif

	return true;
}


// SetStreamSource
// ----------------------------------------------------------------------------------------
void DXmodelClass::SetGeometryBuffers(void* deviceContext)
// ----------------------------------------------------------------------------------------
{
#if defined DX11 || defined DX9
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
		case SHADER_TEXTURE_WATER:
		case SHADER_FIRE:
			stride[0] = sizeof(DXtextureVertexType); break;

		case SHADER_TEXTURE_LIGHT:						//23
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:			//36
		case SHADER_TEXTURE_LIGHT_INSTANCED:			//40
		case SHADER_TEXTURE_LIGHT_DRAWSHADOW_INSTANCED: //41
			stride[0] = sizeof(DXtextureLightVertexType); break;

		case SHADER_TEXTURE_LIGHT_SAVESHADOW:			//36
		case SHADER_TEXTURE_LIGHT_SAVESHADOW_INSTANCED: //40
			stride[0] = sizeof(DXShadowMapVertexType); break;

		case SHADER_NORMAL_BUMP:						//35
		case SHADER_NORMAL_BUMP_INSTANCED:				//99
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
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,	1 Triang. = 3 Vert.--> DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,	4 Triang. = 6 Vert.--> DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
		*/
		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)(PrimitiveTopology));

		// Set the Geometric Data to be sent to Vertex Shader:
		context->IASetVertexBuffers(0, numBuffers, bufferPointer, stride, offset);			// Set the vertex buffer to active in the input assembler so it can be rendered.
		context->IASetIndexBuffer(m_indexBuffer11, DXGI_FORMAT_R32_UINT, 0);					// Set the index buffer to active in the input assembler so it can be rendered.
	}
#endif

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

#if defined USE_LIGHT_RAY
void DXmodelClass::UpdateDynamic(std::vector<ModelColorVertexType>* lightVertexVector)
{
	ID3D11DeviceContext* deviceContext11 = NULL;
	#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		deviceContext11 = m_driver11->m_deviceContext;
	#endif

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

	#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		//Now copy the contents of the vertex array into the vertex buffer using the Map and memcpy functions:
		result = deviceContext11->Map(m_vertexBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);// Lock the vertex buffer so it can be written to.
		if (FAILED(result)) throw woma_exception("deviceContext->Map!", __FILE__, __FUNCTION__, __LINE__);

		verticesPtr = (DXcolorVertexType*)mappedResource.pData;								// Get a pointer to the data in the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, (sizeof(DXcolorVertexType) * m_vertexCount));		// Copy the data into the vertex buffer.
		deviceContext11->Unmap(m_vertexBuffer11, 0);	// Unlock the vertex buffer.
	}
	#endif

	#ifdef DX12	
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
	#endif	

	SAFE_DELETE_ARRAY (vertices);				// Release the vertex array as it is no longer needed.
}
#endif


#if defined USE_VIEW2D_SPRITES

//The UpdateBuffers function is called each frame to update the contents of the dynamic vertex buffer to re-position the 2D bitmap image on the screen if need be.
bool DXmodelClass::UpdateBuffersRotY(int positionX, int positionY)
// ----------------------------------------------------------------------------------------
{
	static int m_previousPosX = -10000;
	static int m_previousPosY = -10000;

	float left, right, top, bottom;
	ModelTextureVertexType* vertices;
	ModelTextureVertexType* verticesPtr;
	D3D11_MAPPED_SUBRESOURCE	mappedResource;
	HRESULT result;

	//We check if the position to render this image has changed. If it hasn't changed then we just exit since the vertex buffer doesn't need 
	//any changes for this frame. This check can save us a lot of processing.

		// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
		// currently has the correct parameters.

		static bool RenderfirstTime=true;
#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
		RenderfirstTime = m_driver11->RenderfirstTime;
#endif
#if defined DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
		RenderfirstTime = m_driver->RenderfirstTime;
#endif
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
	IF_NOT_THROW_EXCEPTION(vertices);

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
	vertices[0].tu = 0;
	vertices[0].tv = 0;

	vertices[1].x = right;
	vertices[1].y = bottom;
	vertices[1].z = 0;
	vertices[1].tu = m_xTexture;
	vertices[1].tv = 1;

	vertices[2].x = left;
	vertices[2].y = bottom;
	vertices[2].z = 0;
	vertices[2].tu = 0;
	vertices[2].tv = 1;

	// Second triangle (t2)
	// --------------------
	vertices[3].x = left;
	vertices[3].y = top;
	vertices[3].z = 0;
	vertices[3].tu = 0;
	vertices[3].tv = 0;

	vertices[4].x = right;
	vertices[4].y = top;
	vertices[4].z = 0;
	vertices[4].tu = m_xTexture;
	vertices[4].tv = 0;

	vertices[5].x = right;
	vertices[5].y = bottom;
	vertices[5].z = 0;
	vertices[5].tu = m_xTexture;
	vertices[5].tv = 1;

	UINT vertexBufferSize = sizeofMODELvertex * m_vertexCount; // sizeof(triangleVertices);

	//Now copy the contents of the vertex array into the vertex buffer using the Map and memcpy functions:
#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		result = m_driver11->m_deviceContext->Map(m_vertexBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);// Lock the vertex buffer so it can be written to.
		if (FAILED(result))return false;

		verticesPtr = (ModelTextureVertexType*)mappedResource.pData;	// Get a pointer to the data in the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, vertexBufferSize);			// (sizeof(ModelTextureVertexType) * m_vertexCount)
		m_driver11->m_deviceContext->Unmap(m_vertexBuffer11, 0);		// Unlock the vertex buffer.
	}
#endif

#ifdef DX12	
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
#endif	

	SAFE_DELETE_ARRAY(vertices);				// Release the vertex array as it is no longer needed.

	return true;
}


bool DXmodelClass::UpdateSpriteBuffersRotY(int positionX, int positionY)
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

	//static bool RenderfirstTime=true;
#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	//if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	//	RenderfirstTime = m_driver11->RenderfirstTime;
#endif
#if defined DX12
	//if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	//	RenderfirstTime = m_driver->RenderfirstTime;
#endif
	//if (((positionX == m_previousPosX) && (positionY == m_previousPosY)) && !RenderfirstTime)
	//	return true;

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
	#define m_bitmapWidth	SpriteTextureWidth
	#define m_bitmapHeight	SpriteTextureHeight

	// First triangle (t1):
	// --------------------
	vertices[0].x = /*left*/(float)-m_bitmapWidth / 2.0f;
	vertices[0].y = /*top*/(float)+m_bitmapHeight / 2.0f;
	vertices[0].z = 0;	
	vertices[0].tu=0;
	vertices[0].tv=0;

	vertices[1].x = /*right*/(float)+m_bitmapWidth / 2.0f;
	vertices[1].y = /*bottom*/(float)-m_bitmapHeight / 2.0f;
	vertices[1].z = 0;	
	vertices[1].tu= m_xTexture;
	vertices[1].tv= 1;

	vertices[2].x = /*left*/(float)-m_bitmapWidth / 2.0f;
	vertices[2].y = /*bottom*/(float)-m_bitmapHeight / 2.0f;
	vertices[2].z = 0;	
	vertices[2].tu= 0;
	vertices[2].tv= 1;

	// Second triangle (t2)
	// --------------------
	vertices[3].x = /*left*/(float)-m_bitmapWidth / 2.0f;
	vertices[3].y = /*top*/(float)+m_bitmapHeight / 2.0f;
	vertices[3].z = 0;	
	vertices[3].tu= 0;
	vertices[3].tv= 0;

	vertices[4].x = /*right*/(float)+m_bitmapWidth / 2.0f;
	vertices[4].y = /*top*/(float)+m_bitmapHeight / 2.0f;
	vertices[4].z = 0;	
	vertices[4].tu= m_xTexture;
	vertices[4].tv= 0;

	vertices[5].x = /*right*/(float)+m_bitmapWidth / 2.0f;
	vertices[5].y = /*bottom*/(float)-m_bitmapHeight / 2.0f;
	vertices[5].z = 0;	
	vertices[5].tu= m_xTexture;
	vertices[5].tv= 1;

	#undef m_bitmapWidth
	#undef m_bitmapHeight

	UINT vertexBufferSize = sizeofMODELvertex * m_vertexCount; // sizeof(triangleVertices);

	//Now copy the contents of the vertex array into the vertex buffer using the Map and memcpy functions:
#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		result = m_driver11->m_deviceContext->Map(m_vertexBuffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);// Lock the vertex buffer so it can be written to.
		if(FAILED(result))return false;

		verticesPtr = (ModelTextureVertexType*)mappedResource.pData;	// Get a pointer to the data in the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, vertexBufferSize);			// (sizeof(ModelTextureVertexType) * m_vertexCount)
		m_driver11->m_deviceContext->Unmap(m_vertexBuffer11, 0);		// Unlock the vertex buffer.
	}
#endif

#ifdef DX12	
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
#endif	

	SAFE_DELETE_ARRAY (vertices);				// Release the vertex array as it is no longer needed.

	return true;
}


bool DXmodelClass::RenderSprite( int positionX, int positionY, float scale, float fade)
// ----------------------------------------------------------------------------------------
{	
	model_fade = fade;

	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	switch (SystemHandle->AppSettings->DRIVER)
	{
	#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	case DRIVER_DX9:
	case DRIVER_DX11:
		if (!UpdateBuffersRotY(positionX, positionY))
			return false;
	break;
	#endif
	#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX12:
		if (!UpdateBuffersRotY(positionX, positionY))
			return false;
	break;
	#endif
	}
	
	//SPECIAL NUMBER: Means world pre-computed 
	m_worldMatrix = XMMatrixIdentity();

	if (fade == -1000)
	{
		Render(/*m_driver11,*/ CAMERA_NORMAL, PROJECTION_MINIMAP);
		return true;
	}

	//PROJECTION_ORTHOGRAPH:
	// 
	// CAMERA:
	// - CAMERA_NORMAL
	// - CAMERA_SKY

	// PROJECTION:
	// - PROJECTION_PERSPECTIVE	//3D
	// - PROJECTION_ORTHOGRAPH	//2D

	switch (SystemHandle->AppSettings->DRIVER)
	{
	#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
	case DRIVER_DX9:
	case DRIVER_DX11:
		Render(/*m_driver11,*/ CAMERA_NORMAL, PROJECTION_ORTHOGRAPH);
	break;
	#endif
	#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	case DRIVER_DX12:
		Render( CAMERA_NORMAL, PROJECTION_ORTHOGRAPH);
	break;
	#endif
	}

	return true;
}
#endif


void DXmodelClass::RenderSubMesh(WomaDriverClass* driver, XMMATRIX* m_world, XMMATRIX* viewRender, XMMATRIX* projectionMatrix,
	UINT pass, XMMATRIX* lightViewMatrix, XMMATRIX* ShadowProjectionMatrix)
{
	DXshaderClass* useShader = NULL;

#if defined DX11
	ID3D11DeviceContext* pContext = NULL;
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		pContext = m_driver11->m_deviceContext;
		useShader = m_Shader11;
	}
#else
	void* pContext = NULL;
#endif
#if defined DX12
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		useShader = m_Shader;
	}
#endif

	// Get shader to Use:
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
	if (pass == PASS_SHADOWS && m_ShaderShadowMap)
		useShader = m_ShaderShadowMap;
#endif

	if (!useShader)
		return;//!

	useShader->hasTexture = false;

	//DX12:
	UINT texture_index=0;

    for(UINT i = 0; i < obj3d.meshSubsets; ++i)
    {
		//ALFA-COLOR:
		if ((UINT)obj3d.material[obj3d.subsetMaterialArray[i]].transparent == pass || pass == PASS_SHADOWS)
	    {
			useShader->hasTexture = obj3d.material[obj3d.subsetMaterialArray[i]].hasTexture;

		    if (!useShader->hasTexture) 
			{
				// Material WITH COLOR ONLY
				// ------------------------
				useShader->pixelColor = obj3d.material[obj3d.subsetMaterialArray[i]].diffuseColor;		// Set the new Pixel Color!)
			    useShader->SetShaderParameters(pass, pContext, m_world, viewRender, projectionMatrix);	// UPDATE CONST. BUFFER for COLOR
		    } 
			else 
			{
				// Material - WITH TEXTURE:
				// ---------------------------------
				if (pass < PASS_SHADOWS) // if shadow pass skip this
				{
					useShader->diffuseColor = obj3d.material[obj3d.subsetMaterialArray[i]].diffuseColor;
					useShader->ambientColor = obj3d.material[obj3d.subsetMaterialArray[i]].ambientColor;
					useShader->emissiveColor = obj3d.material[obj3d.subsetMaterialArray[i]].emissiveColor;

					// Alfa Map:
					#if defined RENDER_OBJ_WITH_ALFA //ALFA_MAP
						#if defined DX11 ||	defined DX9
						if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
							useShader->hasAlfaMap = (obj3d.material[obj3d.subsetMaterialArray[i]].alfaMap11) ? true : false;
						#endif
						#if defined DX12
						if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
							useShader->hasAlfaMap = (obj3d.material[obj3d.subsetMaterialArray[i]].alfaMap) ? true : false;
						#endif
					#endif

					// Specular Light:
					#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
					useShader->hasSpecular = obj3d.material[obj3d.subsetMaterialArray[i]].bSpecular;
					if (useShader->hasSpecular) {
						useShader->specularColor = obj3d.material[obj3d.subsetMaterialArray[i]].specularColor;	// NEW Specular
						useShader->nShininess = (float)obj3d.material[obj3d.subsetMaterialArray[i]].nShininess;	// NEW Shininess
					}
					#endif
					
					texture_index = obj3d.material[obj3d.subsetMaterialArray[i]].texArrayIndex;

					// DX11
					#if defined DX11 || defined DX9
					if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
					{
						// [0] Set: shaderTexture
						pContext->PSSetShaderResources(0, 1, &meshSRV11[texture_index]);// set current texture

						// [1] Set: AlfaMapTexture
						// below...

						// [2] Set: ShadowMapTextureTexture
					#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
					if (ModelRenderShadow) 
						pContext->PSSetShaderResources(2, 1, &SystemHandle->m_Application->m_RenderShadowTexture->m_shaderTextureResourceView);			// set current alfaMap
					#endif

						// [3] Set: TangentMapTexture
					useShader->hasNormMap = obj3d.material[obj3d.subsetMaterialArray[i]].hasNormMap;
					if (useShader->hasNormMap)
						pContext->PSSetShaderResources(3, 1, &meshSRV11[obj3d.material[obj3d.subsetMaterialArray[i]].normMapTexArrayIndex]);
					}
					#endif

					#if defined RENDER_OBJ_WITH_ALFA
					// [1] Set: AlfaMapTexture
					if (useShader->hasAlfaMap)
						pContext->PSSetShaderResources(1, 1, &obj3d.material[obj3d.subsetMaterialArray[i]].alfaMap11);		// set current alfaMap
					#endif
				}
					
				useShader->SetShaderParameters(pass, pContext, m_world, viewRender, projectionMatrix, lightViewMatrix, ShadowProjectionMatrix);// UPDATE CONST. BUFFER for TEXTURE
		    }

			// Render:
		    int indexStart = obj3d.meshSubsetIndexStart[i];
		    int indexDrawAmount = obj3d.meshSubsetIndexStart[i+1] - obj3d.meshSubsetIndexStart[i];
		    useShader->RenderShader(pass, pContext, texture_index, indexDrawAmount, indexStart);				// Now render the prepared buffers with the shader
	    } 
    }
}


void DXmodelClass::RenderWithFade(float fadeLight, bool FOG)
{
#if defined DX11 || defined DX12 || defined OPENGL3
		#if defined DX11 || (defined DX9 && D3D11_SPEC_DATE_YEAR > 2009)
			if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
				m_Shader11->PSfade = fadeLight;

		#endif

		#if defined DX12
			if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
			{
				m_Shader->PSfade = fadeLight;
			}
		#endif

		Render();
#endif
}

#if defined USE_SKY_CAMERA_DOME && DX_ENGINE_LEVEL >= 28 && defined USE_SKYSPHERE
void DXmodelClass::RenderSky(UINT camera, float fadeLight)
{
	#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		m_Shader->PSfade = fadeLight;
		m_Shader->isSky = true;
		Render( CAMERA_SKY, PROJECTION_PERSPECTIVE);
	}
#endif
#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		m_Shader11->PSfade = fadeLight;
		if (m_Driver->RenderfirstTime) 
		{
			m_Shader11->isSky = true;
		}
		Render(CAMERA_SKY, PROJECTION_PERSPECTIVE);
	}
#endif
#if defined DX9sdk
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		m_Shader->PSfade = fadeLight;
		m_Shader->isSky = true;
		Render(m_driver9, CAMERA_SKY, PROJECTION_PERSPECTIVE);
	}
#endif
	
}
#endif

// ----------------------------------------------------------------------------------------
void DXmodelClass::Render(UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
// ----------------------------------------------------------------------------------------
{
	if (m_Driver->RenderfirstTime) 
		{ LOADDRIVER(m_Driver); }

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX12)
	{
		// Step 1 - Put the "vertex", "index" and "instances"(if exist) buffers on the graphics pipeline to prepare them for drawing:
		// ----------------------------------------------------------------------------------------
		SetGeometryBuffers(m_driver->m_device);

		// Step 2 - Get "view" and "projection" matrices from the "driver" and "camera" objects
		// ----------------------------------------------------------------------------------------
		XMMATRIX* projectionMatrix = m_driver->GetProjectionMatrix(camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);
		XMMATRIX* viewMatrix = m_driver->GetViewMatrix(camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);
#if defined USE_VIEW2D_SPRITES
		m_Shader->PSfade = model_fade;
#endif
		// Step 3: Render Complex Mesh (OBJ/W3D):
		// ----------------------------------------------------------------------------------------
		if (obj3d.material.size() > 0)
			RenderSubMesh(m_driver, &m_worldMatrix, viewMatrix, projectionMatrix, pass, (XMMATRIX*)lightViewMatrix, (XMMATRIX*)ShadowProjectionMatrix);			// Multiple Material
		else
			m_Shader->Render(pass, m_driver->m_device, m_indexCount, &m_worldMatrix, viewMatrix, projectionMatrix);	// Single Material (Optimized)
	}
#endif
#if defined DX11 || defined DX9
	if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9)
	{
		 ID3D11DeviceContext* pContext = m_driver11->m_deviceContext;

		// Step 1 - Put the "vertex", "index" and "instances"(if exist) buffers on the graphics pipeline to prepare them for drawing:
		// ----------------------------------------------------------------------------------------
#if defined USE_TERRAIN_QUAD_TREE
		if (m_Shader11->m_shaderType == SHADER_Slope_Texture_Terrain) {
			// Render the terrain cells (and cell lines if needed).
			for (UINT i = 0; i < SystemHandle->m_Application->m_Terrain.GetCellCount(); i++)
			{
				SystemHandle->m_Application->m_Terrain.RenderCell(Driver, i);	// Put the terrain cell buffers on the pipeline.
			}
		}
		else
#endif
		{
			//IASetPrimitiveTopology()
			//IASetVertexBuffers()
			//IASetIndexBuffer()
			SetGeometryBuffers(m_driver11->m_deviceContext);
		}

		// Step 2 - Get "view" and "projection" matrices from the "driver" and "camera" objects
		// ----------------------------------------------------------------------------------------
		XMMATRIX* projectionMatrix; 
		XMMATRIX* viewMatrix;

		if (projection == PROJECTION_MINIMAP) {
			projectionMatrix = (XMMATRIX*)ShadowProjectionMatrix;	//Use provided projection
			viewMatrix = (XMMATRIX*)lightViewMatrix;				//Use provided view
		} else
		{
		if (camera != CAMERA_MINIMAP)
			projectionMatrix = m_driver11->GetProjectionMatrix(camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);
		else
			projectionMatrix = (XMMATRIX*)ShadowProjectionMatrix;	//Use provided projection: MINI-MAP
		
			viewMatrix = m_driver11->GetViewMatrix(camera, projection, pass, lightViewMatrix, ShadowProjectionMatrix);
		}

		// Step 3: Render Complex Mesh (OBJ/W3D):
		// ----------------------------------------------------------------------------------------
		if ((obj3d.material.size() > 0) && (ModelShaderType != SHADER_FIRE))
			RenderSubMesh(m_driver11, &m_worldMatrix, viewMatrix, projectionMatrix, pass, (XMMATRIX*)lightViewMatrix, (XMMATRIX*)ShadowProjectionMatrix); // Multiple Material
		else
		{
			// Step 3: Render Simple Mesh: (Basic Tutorials) include: SKY and MAIN Terrain
			// ----------------------------------------------------------------------------------------
			if (ModelShaderType >= SHADER_TEXTURE)
				for (UINT i = 0; i < meshSRV11.size(); i++)
					pContext->PSSetShaderResources(i, 1, &meshSRV11[i]);	// Set shader texture resource(s) in the "Pixel Shader", only!

			#if defined USE_VIEW2D_SPRITES
			m_Shader11->PSfade = model_fade;
			#endif
		#if TUTORIAL_CHAP >= 62 // FIRE
			if (ModelShaderType == SHADER_FIRE) {
				static float frameTime = 0.0f;

				// Increment the frame time counter.
				frameTime += 0.01f;
				if (frameTime > 1000.0f)
					frameTime = 0.0f;

				m_Shader11->frameTime = frameTime;
			}
		#endif
			m_Shader11->shaderTypeParameter = (float)shaderTypeParameter;
			m_Shader11->Render(pass, pContext, m_indexCount, &m_worldMatrix, viewMatrix, projectionMatrix);	// Single Material (Optimized)
		}
	}
#endif
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
		case SHADER_TEXTURE_WATER:
			d3ddev->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX_XYZ_TEXTURE_DX9));
			d3ddev->SetFVF(CUSTOMFVF_XYZ_TEXTURE_DX9);	// Type of Vertice: TEXTURE

			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->LightEnable(0, FALSE);
			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
			break;

		case SHADER_TEXTURE_LIGHT:
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
			d3ddev->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX_XYZ_LIGHT_DX9));
			d3ddev->SetFVF(CUSTOMFVF_XYZ_LIGHT_DX9);	// Type of Vertice: LIGHT

			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->LightEnable(0, TRUE);
			((DX_CLASS*)SystemHandle->m_Application->m_Driver)->m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
			break;
		default:
			throw woma_exception("WRONG SHADER!", __FILE__, __FUNCTION__, __LINE__);
		}

		if (ModelShaderType >= SHADER_TEXTURE)
			for (UINT i = 0; i < meshSRV.size(); i++)
				d3ddev->SetTexture(i, meshSRV[i]);

#define PrimType D3DPT_TRIANGLESTRIP //D3DPT_TRIANGLELIST
		d3ddev->DrawIndexedPrimitive(PrimType, 0/*VertexStart*/, 0, m_vertexCount, 0/*IndexStart*/, m_indexCount);
		SystemHandle->TotalVertexCounter += indexCount;
	}
#endif
}


// ----------------------------------------------------------------------------------------
// Basic MATH:
// ----------------------------------------------------------------------------------------
void DXmodelClass::Identity()
{
	#if defined DX11 || defined DX12 || defined DX9
		m_worldMatrix = XMMatrixIdentity();
	#endif
	#if defined DX9sdk
		D3DXMatrixIdentity(&m_worldMatrix9);
	#endif
}

void DXmodelClass::multiply (void* m) // in radians!!
{
#if defined DX11 || defined DX12 || defined DX9
	m_worldMatrix *= *(XMMATRIX*)m;
#endif
#if defined DX9sdk
	m_worldMatrix9 *= *(D3DXMATRIX*)m;
#endif
}

void DXmodelClass::rotateX (float rZrad) // in radians!!
{
#if defined DX11 || defined DX12 || defined DX9
    XMMATRIX m = XMMatrixRotationX (rZrad);
#endif
#if defined DX9sdk
	D3DXMATRIX m;
	D3DXMatrixRotationX(&m, rZrad);
#endif
	m_worldMatrix *= m;
}

void DXmodelClass::rotateY (float rZrad) // in radians!!
{
#if defined DX11 || defined DX12 || defined DX9
    XMMATRIX m = XMMatrixRotationY (rZrad);
#endif
#if defined DX9sdk
	D3DXMATRIX m;
	D3DXMatrixRotationY(&m, rZrad);
#endif
	m_worldMatrix *= m;
}

void DXmodelClass::rotateZ (float rZrad) // in radians!!
{
#if defined DX11 || defined DX12 || defined DX9
    XMMATRIX m = XMMatrixRotationZ (rZrad);
#endif
#if defined DX9sdk
	D3DXMATRIX m;
	D3DXMatrixRotationZ(&m, rZrad);
#endif
	m_worldMatrix *= m;
}

#define _11 r[0].m128_f32[0]
#define _12 r[0].m128_f32[1]
#define _13 r[0].m128_f32[2]
#define _14 r[0].m128_f32[3]

#define _21 r[1].m128_f32[0]
#define _22 r[1].m128_f32[1]
#define _23 r[1].m128_f32[2]
#define _24 r[1].m128_f32[3]

#define _31 r[2].m128_f32[0]
#define _32 r[2].m128_f32[1]
#define _33 r[2].m128_f32[2]
#define _34 r[2].m128_f32[3]

#define _41 r[3].m128_f32[0]
#define _42 r[3].m128_f32[1]
#define _43 r[3].m128_f32[2]
#define _44 r[3].m128_f32[3]

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
			m_worldMatrix._11 = x;
			m_worldMatrix._22 = y;
			m_worldMatrix._33 = z;
		//#else
		//	m_worldMatrix.r[0].m128_f32[0] = x;
		//	m_worldMatrix.r[1].m128_f32[1] = y;
		//	m_worldMatrix.r[2].m128_f32[2] = z;
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
			m_worldMatrix._41 = x;
			m_worldMatrix._42 = y;
			m_worldMatrix._43 = z;
		//#else
		//	m_worldMatrix.r[3].m128_f32[0] = x;
		//	m_worldMatrix.r[3].m128_f32[1] = y;
		//	m_worldMatrix.r[3].m128_f32[2] = z;
		//#endif
	#endif
}

#undef _11
#undef _12
#undef _13
#undef _14

#undef _21
#undef _22
#undef _23
#undef _24

#undef _31
#undef _32
#undef _33
#undef _34

#undef _41
#undef _42
#undef _43
#undef _44


bool DXmodelClass::LoadModel(TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow, bool renderShadow, UINT instanceCount)
{

	const TCHAR* extension = _tcsrchr(filename.c_str(), '.');

	if (_tcsicmp(extension, TEXT(".obj")) == 0 || _tcsicmp(extension, TEXT(".OBJ")) == 0)
	{
		bool b = modelClass.LoadOBJ(this, shader_type, g_driver, filename, castShadow, renderShadow, instanceCount);
		if (b)
			modelClass.CreateObject(this, (TCHAR*)filename.c_str(), g_driver, shader_type /*SHADER_AUTO*/, filename, castShadow, renderShadow); // Auto Detect Shader Type
	}
#if defined LOADW3D
	else if (_tcsicmp(extension, TEXT(".w3d")) == 0 || _tcsicmp(extension, TEXT(".W3D")) == 0)
		return LoadW3D(shader_type, g_driver, filename, castShadow, renderShadow, instanceCount);
#endif

	return true;
}

// Create: Bounding Box 
// --------------------------------------------------------------------------------------------
#if defined BOUNDINGVOLUMES
void DXmodelClass::CreateBoundingVolumes(std::vector<XMFLOAT3>& vertPosArray)
// --------------------------------------------------------------------------------------------
{
	XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for(UINT i = 0; i < vertPosArray.size(); i++)
	{
		// The minVertex and maxVertex will most likely not be actual vertices in the model, but vertices
		// that use the smallest and largest x, y, and z values from the model to be sure ALL vertices are
		// covered by the bounding volume

		//Get the smallest vertex
		minVertex.x = min(minVertex.x, vertPosArray[i].x);	// Find smallest x value in model
		minVertex.y = min(minVertex.y, vertPosArray[i].y);	// Find smallest y value in model
		minVertex.z = min(minVertex.z, vertPosArray[i].z);	// Find smallest z value in model

		//Get the largest vertex
		maxVertex.x = max(maxVertex.x, vertPosArray[i].x);	// Find largest x value in model
		maxVertex.y = max(maxVertex.y, vertPosArray[i].y);	// Find largest y value in model
		maxVertex.z = max(maxVertex.z, vertPosArray[i].z);	// Find largest z value in model
	}

	// Create bounding box	
	// Front Vertices:
	boundingBoxVerts.push_back(XMFLOAT3(minVertex.x, minVertex.y, minVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(minVertex.x, maxVertex.y, minVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(maxVertex.x, maxVertex.y, minVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(maxVertex.x, minVertex.y, minVertex.z));

	// Back Vertices:
	boundingBoxVerts.push_back(XMFLOAT3(minVertex.x, minVertex.y, maxVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(maxVertex.x, minVertex.y, maxVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(maxVertex.x, maxVertex.y, maxVertex.z));
	boundingBoxVerts.push_back(XMFLOAT3(minVertex.x, maxVertex.y, maxVertex.z));

	DWORD i[36];	//DWORD* i = NEW DWORD[36];

	// Front Face
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Back Face
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Top Face
	i[12] = 1; i[13] = 7; i[14] = 6;
	i[15] = 1; i[16] = 6; i[17] = 2;

	// Bottom Face
	i[18] = 0; i[19] = 4; i[20] = 5;
	i[21] = 0; i[22] = 5; i[23] = 3;

	// Left Face
	i[24] = 4; i[25] = 7; i[26] = 1;
	i[27] = 4; i[28] = 1; i[29] = 0;

	// Right Face
	i[30] = 3; i[31] = 2; i[32] = 6;
	i[33] = 3; i[34] = 6; i[35] = 5;

	for (int j = 0; j < 36; j++)
		boundingBoxIndex.push_back(i[j]);
}
#endif
}

#endif
#endif
