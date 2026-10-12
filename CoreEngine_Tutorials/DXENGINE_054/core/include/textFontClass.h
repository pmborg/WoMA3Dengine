// --------------------------------------------------------------------------------------------
// Filename: textFontClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once

#include "platform.h"
#if (defined USE_RASTERTEK_TEXT_FONT || defined USE_RASTERTEK_TEXT_FONTV2)

//////////////
// INCLUDES //
//////////////
#include "main.h"
#include "fileLoader.h"


#include <fstream>
using namespace std;

#if defined DX_ENGINE
#if defined DX11
// -------------------------------------------------------------------------------------------
// Use OLD xnamath from DirectX SDK June2010 or Windows Kit 8?
// -------------------------------------------------------------------------------------------
#pragma warning( disable : 4005 )		// Disable warning C4005: '' : macro redefinition
#include <d3d11.h>
#endif

#if D3D11_SPEC_DATE_YEAR == 2009		// Use the OLD DirectX_SDK_June2010 ?
	#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
	#pragma warning( disable : 4838 )
	#include <xnamath.h>				// #include <d3dx10math.h>
#else
	#include <DirectXMath.h>			// Use the NEW DirectX11
	using namespace DirectX;
#endif

#if defined DX11
	#include "DX11Class.h"
#endif
#if defined DX12
	#include "DX12Class.h"
	#include "DX12TextureClass.h"
#endif
#endif

#include "glTextureclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: textFontClass
////////////////////////////////////////////////////////////////////////////////
class textFontClass
{
public:
	struct FontType
	{
		float left, right;
		int size;
	};

	textFontClass();
	~textFontClass();

	bool Initialize(void* pContext, void* g_driver, TCHAR*, TCHAR*);
	void Shutdown();

#if defined DX9 || defined DX11
	ID3D11ShaderResourceView* GetTexture11();
#endif
#if defined DX12
	DX12TextureClass* GetTexture();
#endif
#if (defined OPENGL3 || defined OPENGL4 || defined VULKAN)
	GLtextureClass* gl_Texture = NULL;
#endif

	void BuildVertexArray(void*, TCHAR*, float, float);
	int GetSentencePixelLength(char* sentence);
	int GetFontHeight();

private:
	bool LoadFontData(TCHAR*);
	void ReleaseFontData();

public:
	// VARS:
	// ----------------------------------------------------------------------
	int m_fontHeight  = 18;  // 18 default font height
	float m_spaceSize = 2.0; // 2.0f default space size

	FontType* m_Font = NULL;
#if defined DX_ENGINE
  #if defined DX11 || defined DX9
  	DirectX::DX11Class* m_driver11 = NULL;
  #endif
  #if defined DX12  && D3D11_SPEC_DATE_YEAR > 2009
  	DirectX::DX12Class* m_driver = NULL;
  #endif
  
  	// [PATTERN] Image loader:
  #if defined DX9 || defined DX11
  	ID3D11ShaderResourceView* m_Texture11 = NULL;
  #endif
  #if defined DX12
  	DX12TextureClass* m_Texture = NULL;
  #endif
  #if defined DX11 || defined DX9
  	std::vector<ID3D11ShaderResourceView*> meshSRV11;	// vector with all pointer(s) to textures loaded
  #endif
#endif
};

#endif
