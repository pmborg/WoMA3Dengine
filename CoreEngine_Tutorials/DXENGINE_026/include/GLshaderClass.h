// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: GLshaderClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
#pragma once

#ifndef GLshaderClass_h_
#define GLshaderClass_h_

#include <fstream>
using namespace std;


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
	void OutputLinkerErrorMessage(UINT);
	void ShutdownShader();

	UINT m_vertexShader;
	UINT m_fragmentShader;

public:
	UINT m_shaderProgram;
};

	#include "lightClass.h"	
	//extern LightClass* m_Light;

#endif
