// --------------------------------------------------------------------------------------------
// Filename: DXrenderTextureClass.h
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

#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_

//////////////
// INCLUDES //
//////////////
#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#include < d3d11.h >

#if D3D11_SPEC_DATE_YEAR == 2009		// Use the OLD DirectX_SDK_June2010 ?
	#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
	#pragma warning( disable : 4838 )
	#include <xnamath.h>				// #include <d3dx10math.h>
#else
	#include <DirectXMath.h>			// Use the NEW DirectX11
	using namespace DirectX;
#endif
	
////////////////////////////////////////////////////////////////////////////////
// Class name: DXrendertextureclass
////////////////////////////////////////////////////////////////////////////////
class DXrendertextureclass
{
public:
	DXrendertextureclass();
	~DXrendertextureclass();

	bool Initialize(void* Driver, int, int, float screenDepth, float screenNear);
	void Shutdown();

	void SetRenderTarget(void* Driver);
	void ClearRenderTarget(void* Driver, float, float, float, float);
	//ID3D11ShaderResourceView* GetShaderResourceView();

	//VARS:
	// ---------------------------------------------------------------------
public:
	ID3D11ShaderResourceView*	m_shaderResourceView;
	ID3D11RenderTargetView*		m_renderTargetView;

private:
	ID3D11Texture2D*		m_renderTargetTexture;
	ID3D11Texture2D*		m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;

	D3D11_VIEWPORT			m_viewport;
	XMMATRIX				m_projectionMatrix;
	XMMATRIX				m_orthoMatrix;
};

#endif
