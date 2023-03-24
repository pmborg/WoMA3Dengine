// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DxTextClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// -------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// ********************************************************************************************
#pragma once

//////////////
// INCLUDES //
//////////////
#include "platform.h"

#if (defined DX11 || defined DX12)	// This is the DX Implementation

#include "dx11Class.h"
#include "dx12Class.h"

#include "virtualTextClass.h"
#include "DXshaderClass.h"
#include "textFontClass.h"
// ----------------------------------------

#if TUTORIAL_PRE_CHAP >= 45 //60 // BILLBOARD
#define TEXT_BILLRENDERCOUNT 7
#endif//
#if TUTORIAL_PRE_CHAP >= 45 && TUTORIAL_PRE_CHAP < 55// BILLBOARD
#define N_TEXT_MAX_SENTENCE 8
#endif//

#if TUTORIAL_PRE_CHAP >= 63 // 83
#define TEXT_XP 9
#define TEXT_GOLD 10
#endif//

// Not used yet...
// ----------------------------------------
/*
*/

namespace DirectX {

////////////////////////////////////////////////////////////////////////////////
// Class name: DxTextClass
////////////////////////////////////////////////////////////////////////////////
class DxTextClass : public VirtualTextClass
{
public:
	DxTextClass();
	~DxTextClass();

	void Shutdown();
	bool Initialize(void* g_driver);
	bool InitializeTexture(void* Driver);

	bool UpdateSentence(SentenceType*, TCHAR*, int, int, float, float, float);
	bool InitializeSentence(SentenceType**, int);
	
	void ReleaseSentence(SentenceType**);
	void RenderSentence(SentenceType*);

	DXshaderClass* m_spriteShader = NULL;

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
	textFontClass*			m_Font = NULL;
	XMMATRIX*				m_baseViewMatrix = NULL;

	UINT sizeofMODELvertex = NULL;

#if defined DX12 && D3D11_SPEC_DATE_YEAR > 2009

	PRIMITIVE_TOPOLOGY			PrimitiveTopology;
#endif

};

}

#endif
