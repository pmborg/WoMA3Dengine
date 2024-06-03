// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DxTextClass.h
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
#pragma once

//////////////
// INCLUDES //
//////////////
#include "platform.h"
#if defined USE_RASTERTEK_TEXT_FONT && defined DX_ENGINE

#include "dx11Class.h"
#include "dx12Class.h"

#include "virtualTextClass.h"
#include "DXshaderClass.h"
#include "textFontClass.h"
// ----------------------------------------

namespace DirectX {

////////////////////////////////////////////////////////////////////////////////
// Class name: DxTextClass
////////////////////////////////////////////////////////////////////////////////
class DxTextClass : public VirtualTextClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	DxTextClass();
	~DxTextClass();

	void Shutdown();
	bool Initialize(void* g_driver);
	bool InitializeTexture(void* Driver);

	bool UpdateSentence(SentenceType*, TCHAR*, int, int, float, float, float);
	bool InitializeSentence(SentenceType**, int);
	
	void ReleaseSentence(SentenceType**);
	void RenderSentence(SentenceType*);

//#if defined DX_ENGINE
	DXshaderClass* m_spriteShader = NULL;
//#endif

#if defined DX9sdk
	DirectX::DX9Class* m_driver9 = NULL;
#endif
	DirectX::DX11Class* m_driver11 = NULL;
	ID3D11DeviceContext* m_deviceContext11 = NULL;
#if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
	DirectX::DX12Class* m_driver = NULL;
#endif

private:
	// VARS:
	// ----------------------------------------------------------------------
	textFontClass*	m_Font = NULL;
	XMMATRIX*		m_baseViewMatrix = NULL;
	UINT			sizeofMODELvertex = NULL;
};

}

#endif
