// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DX11CapabilitiesClass.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#include "platform.h"
#include <d3d11.h>
#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
#pragma warning( disable : 4838 )
#include <dxgiformat.h>
#include "dx11Class.h"

#if D3D11_SPEC_DATE_YEAR > 2009
	#include <d3d11_1.h>

	#include "DDSTextureLoader.h"
	#include "WICTextureLoader.h"

	#include "ImageLoaderClass.h"
	#include "DirectXTex.h"

static size_t GetMaximum3DTextureSize(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return D3D10_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
	}
}

static size_t GetMaximum2DTextureSize(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION;
	case D3D_FEATURE_LEVEL_9_3:  return D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION;
	}
}

static size_t GetMaximumCubeMapTextureSize(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_REQ_TEXTURECUBE_DIMENSION;
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return D3D10_REQ_TEXTURECUBE_DIMENSION;
	case D3D_FEATURE_LEVEL_9_3:  return D3D_FL9_3_REQ_TEXTURECUBE_DIMENSION;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return D3D_FL9_1_REQ_TEXTURECUBE_DIMENSION;
	}
}

static size_t GetMaximum2DTextureArraySize(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return D3D10_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return 0;
	}
}

static size_t GetMaximumDrawIndexedIndexCount(D3D_FEATURE_LEVEL featureLevel)
{
	// D3D11 allows up to 2^32 elements, but we report max signed int for convenience since that's what's
	// returned from glGetInteger
	static_assert(D3D11_REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP == 32, "Unexpected D3D11 constant value.");
	static_assert(D3D10_REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP == 32, "Unexpected D3D11 constant value.");

	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0:
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return INT_MAX;
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:  return D3D_FL9_2_IA_PRIMITIVE_MAX_COUNT;
	case D3D_FEATURE_LEVEL_9_1:  return D3D_FL9_1_IA_PRIMITIVE_MAX_COUNT;
	}
}

static size_t GetMaximumDrawVertexCount(D3D_FEATURE_LEVEL featureLevel)
{
	// D3D11 allows up to 2^32 elements, but we report max signed int for convenience since that's what's
	// returned from glGetInteger
	static_assert(D3D11_REQ_DRAW_VERTEX_COUNT_2_TO_EXP == 32, "Unexpected D3D11 constant value.");
	static_assert(D3D10_REQ_DRAW_VERTEX_COUNT_2_TO_EXP == 32, "Unexpected D3D11 constant value.");
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0:
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return INT_MAX;
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:  return D3D_FL9_2_IA_PRIMITIVE_MAX_COUNT;
	case D3D_FEATURE_LEVEL_9_1:  return D3D_FL9_1_IA_PRIMITIVE_MAX_COUNT;
	}
}

static bool GetShaderTextureLODSupport(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0:
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return true;
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return false;
	}
}

static float GetMaximumAnisotropy(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_MAX_MAXANISOTROPY;
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return D3D10_MAX_MAXANISOTROPY;
	// From http://msdn.microsoft.com/en-us/library/windows/desktop/ff476876.aspx
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:  return 16;
	case D3D_FEATURE_LEVEL_9_1:  return D3D_FL9_1_DEFAULT_MAX_ANISOTROPY;
	}
}

static bool GetOcclusionQuerySupport(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0:
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return true;
	// From http://msdn.microsoft.com/en-us/library/windows/desktop/ff476150.aspx ID3D11Device::CreateQuery
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:  return true;
	case D3D_FEATURE_LEVEL_9_1:  return false;
	}
}

static bool GetInstancingSupport(D3D_FEATURE_LEVEL featureLevel)
{
	// From http://msdn.microsoft.com/en-us/library/windows/desktop/ff476150.aspx ID3D11Device::CreateInputLayout
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0:
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return true;
	// Feature Level 9_3 supports instancing, but slot 0 in the input layout must not be instanced.
	// D3D9 has a similar restriction, where stream 0 must not be instanced.
	// This restriction can be worked around by remapping any non-instanced slot to slot 0.
	// This works because HLSL uses shader semantics to match the vertex inputs to the elements in the input layout, rather than the slots.
	// Note that we only support instancing via ANGLE_instanced_array on 9_3, since 9_3 doesn't support OpenGL ES 3.0
	case D3D_FEATURE_LEVEL_9_3:  return true;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return false;
	}
}
static bool GetFramebufferMultisampleSupport(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0:
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return true;
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return false;
	}
}

static size_t GetMaximumSimultaneousRenderTargets(D3D_FEATURE_LEVEL featureLevel)
{
	// From http://msdn.microsoft.com/en-us/library/windows/desktop/ff476150.aspx ID3D11Device::CreateInputLayout
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT;
	case D3D_FEATURE_LEVEL_9_3:  return D3D_FL9_3_SIMULTANEOUS_RENDER_TARGET_COUNT;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return D3D_FL9_1_SIMULTANEOUS_RENDER_TARGET_COUNT;
	}
}

static size_t GetMaximumViewportSize(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_VIEWPORT_BOUNDS_MAX;
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return D3D10_VIEWPORT_BOUNDS_MAX;
	// No constants for D3D11 Feature Level 9 viewport size limits, use the maximum texture sizes
	case D3D_FEATURE_LEVEL_9_3:  return D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION;
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION;
	}
}

static size_t GetMaximumVertexInputSlots(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_STANDARD_VERTEX_ELEMENT_COUNT;
	case D3D_FEATURE_LEVEL_10_1: return D3D10_1_STANDARD_VERTEX_ELEMENT_COUNT;
	case D3D_FEATURE_LEVEL_10_0: return D3D10_STANDARD_VERTEX_ELEMENT_COUNT;
	// From http://http://msdn.microsoft.com/en-us/library/windows/desktop/ff476876.aspx "Max Input Slots"
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return 16;
	}
}

static size_t GetMaximumConstantBufferSize(D3D_FEATURE_LEVEL featureLevel)
{
	// Returns a size_t despite the limit being a GLuint64 because size_t is the maximum size of
	// any buffer that could be allocated.
	const size_t bytesPerComponent = 4 * sizeof(float);
	switch (featureLevel)
	{
	default:
#if D3D11_SPEC_DATE_YEAR > 2009
	case D3D_FEATURE_LEVEL_11_1:
#endif
	case D3D_FEATURE_LEVEL_11_0: return D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * bytesPerComponent;
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0: return D3D10_REQ_CONSTANT_BUFFER_ELEMENT_COUNT * bytesPerComponent;
	// Limits from http://msdn.microsoft.com/en-us/library/windows/desktop/ff476501.aspx remarks section
	case D3D_FEATURE_LEVEL_9_3:
	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:  return 4096 * bytesPerComponent;
	}
}

#endif

namespace DirectX {

void DX11Class::setDeviceCapabilities(D3D_FEATURE_LEVEL featureLevel)
{
	HRESULT result = S_OK;

	// Query DXGI1.1:
	m_sCapabilities.DXGI11 = false;
	IDXGIDevice1 *dxgiDevice1 = NULL;
	result = m_device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice1);
	if (!FAILED(result))
	{
		m_sCapabilities.DXGI11 = true;
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DXGI1.1: Available\n"));
	}
	SAFE_RELEASE(dxgiDevice1);

#if D3D11_SPEC_DATE_YEAR > 2009
	// Query DXGI1.2:
	m_sCapabilities.DXGI12 = false;
	IDXGIDevice2 *dxgiDevice2 = NULL;
	result = m_device->QueryInterface(__uuidof(IDXGIDevice2), (void**)&dxgiDevice2);
	if (!FAILED(result))
	{
		m_sCapabilities.DXGI12 = true;
		WOMA_LOGManager_DebugMSGAUTO(TEXT("DXGI1.2: Available\n"));
	}
	SAFE_RELEASE(dxgiDevice2);

	WOMA_LOGManager_DebugMSGAUTO(TEXT("Device Capabilities:\n"));

	m_sCapabilities.maxTextureAnisotropy = GetMaximumAnisotropy(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("maxTextureAnisotropy: %f\n"), m_sCapabilities.maxTextureAnisotropy);

	m_sCapabilities.occlusionQueryBoolean = GetOcclusionQuerySupport(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("occlusionQueryBoolean: %s\n"), m_sCapabilities.occlusionQueryBoolean?TEXT("TRUE"): TEXT("FALSE"));
	m_sCapabilities.instancedArraysBoolean = GetInstancingSupport(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("instancedArraysBoolean: %s\n"), m_sCapabilities.instancedArraysBoolean ? TEXT("TRUE") : TEXT("FALSE"));
	m_sCapabilities.framebufferMultisampleBoolean = GetFramebufferMultisampleSupport(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("framebufferMultisampleBoolean: %s\n"), m_sCapabilities.framebufferMultisampleBoolean ? TEXT("TRUE") : TEXT("FALSE"));
	m_sCapabilities.shaderTextureLODBoolean = GetShaderTextureLODSupport(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("shaderTextureLODBoolean: %s\n"), m_sCapabilities.shaderTextureLODBoolean ? TEXT("TRUE") : TEXT("FALSE"));

	m_sCapabilities.MaximumSimultaneousRenderTargets = GetMaximumSimultaneousRenderTargets(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("MaximumSimultaneousRenderTargets: %d\n"), m_sCapabilities.MaximumSimultaneousRenderTargets);

	m_sCapabilities.max2DTextureSize = GetMaximum2DTextureSize(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("max2DTextureSize: %d\n"), m_sCapabilities.max2DTextureSize);
	m_sCapabilities.maxArrayTextureLayers = GetMaximum2DTextureArraySize(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("maxArrayTextureLayers: %d\n"), m_sCapabilities.maxArrayTextureLayers);

	m_sCapabilities.max3DTextureSize = GetMaximum3DTextureSize(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("max3DTextureSize: %d\n"), m_sCapabilities.max3DTextureSize);
	m_sCapabilities.maxCubeMapTextureSize = GetMaximumCubeMapTextureSize(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("maxCubeMapTextureSize: %d\n"), m_sCapabilities.maxCubeMapTextureSize);

	m_sCapabilities.maxViewportWidth = GetMaximumViewportSize(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("maxViewportWidth: %d\n"), m_sCapabilities.maxViewportWidth);
	m_sCapabilities.maxViewportHeight = m_sCapabilities.maxViewportWidth;
	WOMA_LOGManager_DebugMSGAUTO(TEXT("maxViewportHeight: %d\n"), m_sCapabilities.maxViewportHeight);

	m_sCapabilities.maxElementsIndices = GetMaximumDrawIndexedIndexCount(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("maxElementsIndices: %d\n"), m_sCapabilities.maxElementsIndices);
	m_sCapabilities.maxElementsVertices = GetMaximumDrawVertexCount(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("maxElementsVertices: %d\n"), m_sCapabilities.maxElementsVertices);
	
	m_sCapabilities.maxVertexAttributes = GetMaximumVertexInputSlots(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("maxVertexAttributes: %d\n"), m_sCapabilities.maxVertexAttributes);
	m_sCapabilities.MaximumConstantBufferSize = GetMaximumConstantBufferSize(featureLevel);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("MaximumConstantBufferSize: %d\n"), m_sCapabilities.MaximumConstantBufferSize);
#endif
}

}

