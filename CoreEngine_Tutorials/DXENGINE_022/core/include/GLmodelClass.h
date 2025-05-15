// --------------------------------------------------------------------------------------------
// Filename: GLmodelClass.h
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
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
#pragma once

//////////////
// INCLUDES //
//////////////
#include "platform.h"

#if CORE_ENGINE_LEVEL >= 10 && (defined OPENGL3 || defined OPENGL4)
#include "virtualModelClass.h"
#include "GLshaderClass.h"
#include "GLmathClass.h"

#include "fileLoader.h"
#include "GLTextureClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: GLmodelClass
////////////////////////////////////////////////////////////////////////////////
class GLmodelClass : public VirtualModelClass
{
public:
	UINT WomaIntegrityCheck = 1234567155;
	GLmodelClass(bool model3D=true);
	~GLmodelClass();
	void Shutdown();

	void RenderWithFade(float fadeLight = 1, bool FOG = false);
	void Render(UINT camera = 0, UINT projection = 0, UINT pass = 0, void* lightViewMatrix = NULL, void* ShadowProjectionMatrix = NULL);

	bool InitializeVertexIndexBuffers(std::vector <STRING>* textureFile);

	bool LoadColor(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);
	bool LoadTexture(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureVertexType> *model, std::vector<UINT>* indexList = NULL, UINT instanceCount=0);

#if defined USE_VIEW2D_SPRITES		// Sprites
	bool RenderSprite( int positionX, int positionY, float scale=1.0f, float fade = 1.0f);
	bool UpdateBuffersRotY( int positionX, int positionY);
	bool UpdateSpriteBuffersRotY( int positionX, int positionY);
#endif

#if defined USE_LIGHT_RAY
	void UpdateDynamic( std::vector<ModelColorVertexType>* lightVertexVector);
#endif

	//GLuint linearFiltering;
	STRING MODEL_NAME;

	// Math Aux Functions //
	void Identity();
	void multiply (void* m);	//XMMATRIX* or mat4*

	void rotateX (float rad);
	void rotateY (float rad);
	void rotateZ (float rad);

	void scale(float x, float y, float z);
	void translation(float x, float y, float z);

	// ----------------------------------------------------------------------
	int GetIndexCount();
	GLshaderClass* m_Shader = NULL;

private:
	GLmathClass	mathClass;

	void SetOpenGLBuffers(UINT sizeofMODELvertex, UINT* indices);
	void GetIndices();

	std::vector<ModelColorVertexType>* modelColorVertex;
	bool InitializeColorBuffers(/*GLopenGLclass*/ void* OpenGL);
	std::vector<ModelTextureVertexType>* modelTextureVertex;
	bool InitializeTextureBuffers(/*GLopenGLclass*/ void* OpenGL);
	
	void SetBuffers(/*GLopenGLclass*/ void*);
	void RenderBuffers(/*GLopenGLclass*/ void*);

#if defined USE_VIEW2D_SPRITES
	//float scale2D;
	float			m_xTexture = 1.0f;
	float			model_fade = 1;
#endif

	std::vector<UINT>* indexModelList;
	UINT*			indices = NULL;
	bool			Model3D;
	
	UINT			m_vertexCount = NULL, m_indexCount = NULL;
	unsigned int	m_vertexArrayId = NULL, m_vertexBufferId = NULL, m_indexBufferId = NULL;
	
	mat4			m_worldMatrix;
	mat4			m_viewMatrix;
	mat4			projectionMatrix;
	
	GLtextureClass* m_Texture=NULL;			// Image loader
	std::vector<GLtextureClass*> meshSRV;	// meshSRV

};

#endif
