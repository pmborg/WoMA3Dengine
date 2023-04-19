// --------------------------------------------------------------------------------------------
// Filename: textFontClass.h
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
#pragma once

#include "platform.h"

//////////////
// INCLUDES //
//////////////
#include "main.h"
#include "fileLoader.h"


#include <fstream>
using namespace std;

// -------------------------------------------------------------------------------------------
// Use OLD xnamath from DirectX SDK June2010 or Windows Kit 8?
// -------------------------------------------------------------------------------------------
#pragma warning( disable : 4005 )		// Disable warning C4005: '' : macro redefinition
#include <d3d11.h>

#if D3D11_SPEC_DATE_YEAR == 2009		// Use the OLD DirectX_SDK_June2010 ?
	#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
	#pragma warning( disable : 4838 )
	#include <xnamath.h>				// #include <d3dx10math.h>
#else
	#include <DirectXMath.h>			// Use the NEW DirectX11
	using namespace DirectX;
#endif

	#include "DX11Class.h"

#include "glTextureclass.h"

struct FontType
{
	float left, right;
	int size;
};

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: textFontClass
////////////////////////////////////////////////////////////////////////////////
class textFontClass
{
public:
	UINT WomaIntegrityCheck = 1234567831;
	textFontClass();
	~textFontClass();

	bool Initialize(void* Driver, TCHAR*, TCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture11();
#if defined DX9sdk
	LPDIRECT3DTEXTURE9 GetTexture();
#endif
	GLtextureClass* gl_Texture = NULL;
	void BuildVertexArray(void*, TCHAR*, float, float);

private:
	bool LoadFontData(TCHAR*);
	void ReleaseFontData();

public:
	// VARS:
	// ----------------------------------------------------------------------
	FontType* m_Font = NULL;

#if defined DX9sdk
	DirectX::DX9Class* m_driver9 = NULL;
#endif
	DirectX::DX11Class* m_driver11 = NULL;

	// [PATTERN] Image loader:
	ID3D11ShaderResourceView* m_Texture11 = NULL;
#if defined DX9sdk
	LPDIRECT3DTEXTURE9 m_Texture9 = NULL;
#endif
};

