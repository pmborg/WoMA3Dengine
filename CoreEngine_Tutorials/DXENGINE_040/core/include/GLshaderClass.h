// --------------------------------------------------------------------------------------------
// Filename: GLshaderClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234526920;

#pragma once

#ifndef GLshaderClass_h_
#define GLshaderClass_h_

#include "platform.h"
#include <fstream>
using namespace std;

#include "GLmathClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GLshaderClass
////////////////////////////////////////////////////////////////////////////////
class GLshaderClass
{
public:
	GLshaderClass();
	~GLshaderClass();

	bool Initialize(SHADER_TYPE shaderType);
	void Shutdown();
	void SetShader();
	bool SetShaderParameters(SHADER_TYPE shaderType, mat4*, mat4*, mat4*, int textureUnit=0);

private:
	bool InitializeShader(SHADER_TYPE shaderType, TCHAR*, TCHAR*);
	char* LoadShaderSourceFile(char*);
	void OutputShaderErrorMessage(UINT, char*);
#if defined UNICODE
	void OutputShaderErrorMessage(UINT, WCHAR*);
#endif
	void OutputLinkerErrorMessage(UINT);
	void ShutdownShader();

	UINT m_vertexShader;
	UINT m_fragmentShader;

public:
	UINT m_shaderProgram;

	// --------------------------------------------------------------------------------------------
	// Internal Shader VARs to Copy to Buffers: VS/PS
	// --------------------------------------------------------------------------------------------
	// BLOCK1:
	float		pixelColor[4];

	// BLOCK2:
	BOOL		hasTexture;
	BOOL		hasLight;
	BOOL		hasSpecular;
	int			isFont;

	// BLOCK3:
	float		ambientColor[4];	// LIGHT: Ka
	float		diffuseColor[4];	// LIGHT: Kd
	float		emissiveColor[4];	// LIGHT: Ke
	//			lightDirection (AUTO)

	// BLOCK4:
	bool		hasColorMap = false;	// 66
	float		lightType = 0;			// Light Type
	bool		isDay = 0;				// level98
	float		shaderTypeParameter = 0;// level62

	// BLOCK5:
	bool		hasAlfaColor;
	float		alfaColor;
	float		PSfade=1;			// Fade from 0 to 1
#if defined INTRO_DEMO
	float		frameTime;			// For animations
#endif

	// BLOCK6:
	BOOL		hasFog;
	BOOL		isSky=false;
	BOOL		hasAlfaMap;	// 43
	BOOL		hasNormMap;

	// BLOCK7:
	// cameraPosition (AUTO)
	BOOL		castShadow;
	float		specularColor[3];	// 44:
	float		nShininess;		// 44:

#if DX_ENGINE_LEVEL >= 40 && defined USE_INSTANCES // Normal Bump + Instancing 
	UINT		m_instanceCount = 0;
#endif
};

	#include "lightClass.h"	

#endif
