// --------------------------------------------------------------------------------------------
// Filename: GLmodelClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"
#pragma warning( disable : 4473 )
#if (defined OPENGL3 || defined OPENGL4) && DX_ENGINE_LEVEL >= 21
#include "OSengine.h"
#include "mem_leak.h"

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "GLmodelClass.h"
#include "GLshaderClass.h"


GLmodelClass::GLmodelClass(bool model3d) 
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;

	// SUPER: ----------------------------------------------------------------------
	m_ObjId = 0;
	ModelShaderType = SHADER_AUTO;

	Model3D = model3d;
	ModelHASfog = false;
	ModelHASlight = true; // Have to be true!
	ModelHASColorMap = false;

	PosX = PosY = PosZ = 0;

	SpriteTextureWidth = NULL;
	SpriteTextureHeight = NULL;

	ModelHASNormals = false;

	m_Shader = NULL;

	//meshSRV
	//minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	//maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	//objectCenterOffset = XMFLOAT4(0, 0, 0, 0);
	boundingSphere = false;

	m_vertexCount = m_indexCount = NULL;
	indices = NULL;

#if defined USE_VIEW2D_SPRITES
	m_xTexture = 1.0f;
#endif

	// Initialize the world/model matrix to the identity matrix:
	m_worldMatrix = m_worldMatrix.mat4identity();  //Identity();
	m_viewMatrix = m_viewMatrix.mat4identity();
/*
	glGenSamplers(1, &linearFiltering);
	glSamplerParameteri(linearFiltering, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//BEST
	glSamplerParameteri(linearFiltering, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(linearFiltering, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glSamplerParameteri(linearFiltering, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glSamplerParameteri(linearFiltering, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
*/
}

GLmodelClass::~GLmodelClass(){CLASSDELETE();}

#if defined MAIN_RENDER_LIGHT_RAY
void GLmodelClass::UpdateLightRayVertices(void* pContext, std::vector<ModelColorVertexType>* lightVertexVector)
{
	static float m_previousPosX = -10000;
	static float m_previousPosY = -10000;
	static float m_previousPosZ = -10000;

	float positionX = (*lightVertexVector)[1].x;
	float positionY = (*lightVertexVector)[1].y;
	float positionZ = (*lightVertexVector)[1].z;

	//We check if the position to render this image has changed. If it hasn't changed then we just exit since the vertex buffer doesn't need 
	//any changes for this frame. This check can save us a lot of processing.
	if (((positionX == m_previousPosX) && (positionY == m_previousPosY)) && !((WomaDriverClass*)m_Driver)->RenderfirstTime)
		return;

	//If the position to render this image has changed then we record the new location for the next time we come through this function.
	m_previousPosX = positionX;
	m_previousPosY = positionY;
	m_previousPosZ = positionZ;

	//Now that the coordinates are calculated create a temporary vertex array and fill it with the new six vertex points.
	m_vertexCount = (UINT)(*modelColorVertex).size();
	ModelColorVertexType* vertices = NEW ModelColorVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION(vertices);

	// Load the vertex array with data:
	for (UINT i = 0; i < m_vertexCount; i++)
	{
		// Load the vertex array with data:
		vertices[i].x = (*modelColorVertex)[i].x;
		vertices[i].y = (*modelColorVertex)[i].y;
		vertices[i].z = (*modelColorVertex)[i].z;

		vertices[i].r = (*modelColorVertex)[i].r;
		vertices[i].g = (*modelColorVertex)[i].g;
		vertices[i].b = (*modelColorVertex)[i].b;
		vertices[i].a = (*modelColorVertex)[i].a;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(ModelColorVertexType), vertices, GL_STATIC_DRAW);	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	//GLenum err = glGetError(); if (err != GL_NO_ERROR) { _tprintf("ERROR! glBufferData err: %04x", err); }

	SAFE_DELETE_ARRAY(vertices);
};
#endif

bool GLmodelClass::InitializeVertexIndexBuffers(std::vector <STRING>* textureFile = NULL) 
{
bool result=false;

	// Initialize the vertex and index buffer that hold the geometry for the triangles:
// ------------------------------------------------------------------------------------------------
	switch (ModelShaderType) 
	{
	case SHADER_COLOR:
		result = InitializeColorBuffers(NULL);
		break;
	case SHADER_TEXTURE:
	case SHADER_TEXTURE_FONT:
	case SHADER_TEXTURE_WATER:
		result = InitializeTextureBuffers(NULL);
		break;
	}
	IF_NOT_RETURN_FALSE(result);

	// Create the shader to this object:
	// ------------------------------------------------------------------------------------------------
	m_Shader = NEW GLshaderClass;
	IF_NOT_THROW_EXCEPTION (m_Shader);

	result = m_Shader->Initialize(ModelShaderType);
	if(!result)
		{ WomaFatalException(("Could not initialize the shader object.")); /*return false;*/ }

	// Load Texture (manually)
	// ------------------------------------------------------------------------------------------------
	if (ModelShaderType >= SHADER_TEXTURE)
	{
		// Create the texture object for this model:
		m_Texture = NEW GLtextureClass;
		IF_NOT_THROW_EXCEPTION (m_Texture);
		meshSRV.push_back(m_Texture);

		// Initialize the texture object: ###: AddTexture(pixels, width, height, wrap);
		TCHAR* file = (TCHAR*)(*textureFile)[0].c_str();
	#if defined WINDOWS_PLATFORM
		result = m_Texture->Initialize(WOMA::LoadFile(file), 0, /*wrap*/ Model3D);
	#else
		result = m_Texture->Initialize(file, 0, /*wrap*/ Model3D);
	#endif
		if(!result) { 
			WomaMessageBox((TCHAR*)(*textureFile)[0].c_str(), TEXT("GLmodelClass: Texture File not found")); 
			return false; 
		}

		if (!Model3D)
		{
			if (ModelShaderType != SHADER_COLOR) {
				SpriteTextureWidth = m_Texture->width;
				SpriteTextureHeight = m_Texture->height;
			}
		}
	}

	return true;
}

// -------------------	// COLOR
bool GLmodelClass::LoadColor(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType>* model, std::vector<UINT>* indexList, UINT instanceCount)
{
	_tprintf(TEXT("--------------------------------------------------------------\n"));
	_tprintf(TEXT("[%d]: LOADCOLOR(): %s\n"), gettid(), objectName);

	MODEL_NAME = objectName;
	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_COLOR;
	else
		ModelShaderType = shader_type;


	modelColorVertex = model;
	indexModelList = indexList;
	return InitializeVertexIndexBuffers();
}

bool GLmodelClass::LoadTexture(void* pContext, TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING>* textureFile, std::vector<ModelTextureVertexType>* model, std::vector<UINT>* indexList, UINT instanceCount)
{
	_tprintf(TEXT("--------------------------------------------------------------\n"));
	_tprintf(TEXT("[%d]: LOADTEXTURE(): %s\n"), gettid(), objectName);

	MODEL_NAME = objectName;
	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_TEXTURE;
	else
		ModelShaderType = shader_type;

	ASSERT(ModelShaderType == SHADER_TEXTURE || ModelShaderType == SHADER_TEXTURE_FONT);

	modelTextureVertex = model;
	indexModelList = indexList;
	return InitializeVertexIndexBuffers(textureFile);
}

void GLmodelClass::Shutdown()
{
	// Release the texture used for this model.
	for (UINT i = 0; i < meshSRV.size(); i++)
		SAFE_SHUTDOWN(meshSRV[i]);

	//ShutdownBuffers

	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_vertexBufferId);

	// Release the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_indexBufferId);

	// Release the vertex array object.
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vertexArrayId);

	SAFE_SHUTDOWN (m_Shader);
}

#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES // OLD:ENGINE_LEVEL >= 26
	bool GLmodelClass::RenderSprite(void* pContext, int positionX, int positionY, float scale, float fade)
	{
		model_fade = fade;

		#if defined LINUX_PLATFORM
		#else
		//positionY += 75;
		#endif

		if (!UpdateBuffersRotY(pContext, positionX, positionY))
			return false;

		#define _11 0
		#define _12 1
		#define _13 2
		#define _14 3

		#define _21 4
		#define _22 5
		#define _23 6
		#define _24 7

		#define _31 8
		#define _32 9
		#define _33 10
		#define _34 11

		#define _41 12
		#define _42 13
		#define _43 14
		#define _44 15

		m_worldMatrix.mat4identity();
		/*
		//scale2D = scale;
		if (scale != 1) {
			m_worldMatrix.m[_11] = m_worldMatrix.m[_22] = m_worldMatrix.m[_33] = scale;
		}
		*/
		//float Ypos = (WOMA::AppSettings->WINDOW_HEIGHT) / 2 - m_worldMatrix.m[_33] * SpriteTextureHeight/2;
		//float Ypos = - m_worldMatrix.m[_33] * SpriteTextureHeight / 2;
		//m_worldMatrix.m[_42] = Ypos;


		/*
		if (scale != 1) {
			m_worldMatrix.m[4 * 0 + 0] = scale;
			m_worldMatrix.m[4 * 1 + 1] = scale;
			m_worldMatrix.m[4 * 2 + 2] = scale;
		}
		*/

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

		Render(pContext, 0, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH);

		return true;
	}

	bool GLmodelClass::UpdateBuffersRotY(void* pContext, int positionX, int positionY)
	{
		static int m_previousPosX = -10000;
		static int m_previousPosY = -10000;
		float left, right, top, bottom;

		/*no static*/bool RenderfirstTime = ((WomaDriverClass*)m_Driver)->RenderfirstTime;
		if (((positionX == m_previousPosX) && (positionY == m_previousPosY)) && !RenderfirstTime)
			return true;

		// If it has changed then update the position it is being rendered to.
		m_previousPosX = positionX;
		m_previousPosY = positionY;

		//The four sides of the image need to be calculated. See the diagram at the top of the tutorial for a complete explanation.
		left = (float)((WOMA::AppSettings->WINDOW_WIDTH / 2) * -1) + (float)positionX;	// Calculate the screen coordinates of the left side of the bitmap.
		right = left + (float)SpriteTextureWidth;										// Calculate the screen coordinates of the right side of the bitmap.

		//LINUX & ANDROID:
		top = (float)(WOMA::AppSettings->WINDOW_HEIGHT / 2) - (float)positionY;			// Calculate the screen coordinates of the top of the bitmap.
		bottom = top - (float)SpriteTextureHeight;
#if defined WINDOWS_PLATFORM //FIX OPENGL ON WINDOWS
		#define winTopOffset 20
		top -= winTopOffset + positionY;
		bottom = (WOMA::AppSettings->WINDOW_HEIGHT / 2) - (winTopOffset + positionY+SpriteTextureHeight*2);
#endif

		//op1:
		//Now that the coordinates are calculated create a temporary vertex array and fill it with the new six vertex points.
		//ModelTextureVertexType* vertices;
		//vertices = NEW ModelTextureVertexType[m_vertexCount];
		//IF_NOT_THROW_EXCEPTION(vertices);

		static ModelTextureVertexType vertices[6];

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

		// Bind the vertex buffer.
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof (vertices) /*m_vertexCount * sizeof(ModelTextureVertexType)*/, vertices, GL_STATIC_DRAW);	// Bind the vertex

		//op1:
		//SAFE_DELETE_ARRAY(vertices);				// Release the vertex array as it is no longer needed.

		return true;
	}

	bool GLmodelClass::UpdateSpriteBuffersRotY(void* pContext, int positionX, int positionY)
	{
		static int m_previousPosX = -10000;
		static int m_previousPosY = -10000;
		static bool RenderfirstTime = ((WomaDriverClass*)m_Driver)->RenderfirstTime;

		float left, right, top, bottom;
		ModelTextureVertexType* vertices;

		if (((positionX == m_previousPosX) && (positionY == m_previousPosY)) && !RenderfirstTime)
			return true;

		// If it has changed then update the position it is being rendered to.
		m_previousPosX = positionX;
		m_previousPosY = positionY;

		//The four sides of the image need to be calculated. See the diagram at the top of the tutorial for a complete explaination.
		left = (float)((WOMA::AppSettings->WINDOW_WIDTH / 2) * -1) + (float)positionX;	// Calculate the screen coordinates of the left side of the bitmap.
		right = left + (float)SpriteTextureWidth;												// Calculate the screen coordinates of the right side of the bitmap.
		top = (float)(WOMA::AppSettings->WINDOW_HEIGHT / 2) - (float)positionY;			// Calculate the screen coordinates of the top of the bitmap.
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
	#define m_bitmapWidth	SpriteTextureWidth
	#define m_bitmapHeight	SpriteTextureHeight

	// First triangle (t1):
	// --------------------
		vertices[0].x = /*left*/(float)-m_bitmapWidth / 2.0f;
		vertices[0].y = /*top*/(float)+m_bitmapHeight / 2.0f;
		vertices[0].z = 0;
		vertices[0].tu = 0;
		vertices[0].tv = 0;

		vertices[1].x = /*right*/(float)+m_bitmapWidth / 2.0f;
		vertices[1].y = /*bottom*/(float)-m_bitmapHeight / 2.0f;
		vertices[1].z = 0;
		vertices[1].tu = m_xTexture;
		vertices[1].tv = 1;

		vertices[2].x = /*left*/(float)-m_bitmapWidth / 2.0f;
		vertices[2].y = /*bottom*/(float)-m_bitmapHeight / 2.0f;
		vertices[2].z = 0;
		vertices[2].tu = 0;
		vertices[2].tv = 1;

		// Second triangle (t2)
		// --------------------
		vertices[3].x = /*left*/(float)-m_bitmapWidth / 2.0f;
		vertices[3].y = /*top*/(float)+m_bitmapHeight / 2.0f;
		vertices[3].z = 0;
		vertices[3].tu = 0;
		vertices[3].tv = 0;

		vertices[4].x = /*right*/(float)+m_bitmapWidth / 2.0f;
		vertices[4].y = /*top*/(float)+m_bitmapHeight / 2.0f;
		vertices[4].z = 0;
		vertices[4].tu = m_xTexture;
		vertices[4].tv = 0;

		vertices[5].x = /*right*/(float)+m_bitmapWidth / 2.0f;
		vertices[5].y = /*bottom*/(float)-m_bitmapHeight / 2.0f;
		vertices[5].z = 0;
		vertices[5].tu = m_xTexture;
		vertices[5].tv = 1;

	#undef m_bitmapWidth
	#undef m_bitmapHeight

		// Bind the vertex buffer.
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(ModelTextureVertexType), vertices, GL_STATIC_DRAW);	// Bind the vertex
		/*
				// Get a pointer to the buffer's actual location in memory.
				void* dataPtr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

				// Copy the vertex data into memory.
				memcpy(dataPtr, vertices, m_vertexCount * sizeof(ModelColorVertexType));

				// Unlock the vertex buffer.
				glUnmapBuffer(GL_ARRAY_BUFFER);
		*/


		SAFE_DELETE_ARRAY(vertices);				// Release the vertex array as it is no longer needed.

		return true;
	}

#endif


void GLmodelClass::RenderWithFade(void* pContext, float fadeLight, bool FOG, UINT pass, void* lightViewMatrix, void* lightProjectionMatrix)
{
	//m_Shader->fade = fadeLight;
	Render(pContext);
}
void GLmodelClass::Render(void* pContext, UINT threadID, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
	GLopenGLclass* driver = (GLopenGLclass*)m_Driver;

	if (ModelShaderType > SHADER_COLOR) {
		// Step 3: Set shader texture resource(s)
		glActiveTexture(GL_TEXTURE0);	// Set the unique texture unit in which to store the data.
		//glBindSampler(0, linearFiltering);

		//GL_LINEAR_MIPMAP_LINEAR
		//	Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the GL_LINEAR 
		//	criterion(a weighted average of the texture elements that are closest to the specified texture coordinates) 
		//	to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
		//	As more texture elements are sampled in the minification process, fewer aliasing artifacts will be apparent.
		//	While the GL_NEAREST and GL_LINEAR minification functions can be faster than the other four, they sample only 
		//	one or multiple texture elements to determine the texture value of the pixel being rendered and can produce 
		//	moire patterns or ragged transitions.The initial value of GL_TEXTURE_MIN_FILTER is GL_NEAREST_MIPMAP_LINEAR.

		glBindTexture(GL_TEXTURE_2D, m_Texture->m_textureID);
	}
	switch (projection)
	{
		case PROJECTION_PERSPECTIVE:
			 projectionMatrix = (driver->m_projectionMatrix);
			break;

		#if defined INTRO_DEMO || defined USE_VIEW2D_SPRITES
		case PROJECTION_ORTHOGRAPH:
			projectionMatrix = driver->m_orthoMatrix;
		break;
		#endif
	}

	switch (camera)
	{
		case CAMERA_NORMAL:
			if (projection == PROJECTION_PERSPECTIVE)
			{
				//PROJECTION_PERSPECTIVE
				m_viewMatrix = ((GLcameraClass*)driver->gl_Camera)->m_viewMatrix;
			}
			else
			{	
				//PROJECTION_ORTHOGRAPH:
				m_viewMatrix = m_viewMatrix;
				m_viewMatrix.mat4identity();
				m_viewMatrix.m[14] = 1;
			}
			break;

	}

	m_Shader->SetShader(); //glUseProgram(m_Shader->m_shaderProgram); // 

#if defined USE_VIEW2D_SPRITES
	m_Shader->PSfade = model_fade;
#endif
	if (RENDER_PAGE >= 26)
		m_Shader->lightType = 2;
	else
		m_Shader->lightType = 1;

	m_Shader->SetShaderParameters(ModelShaderType, &m_worldMatrix, &m_viewMatrix, &projectionMatrix, 0 /*m_Texture->m_textureID*/);


	// Step 4: Render the model
	glBindVertexArray(m_vertexArrayId);		// SetBuffers(driver);
	RenderBuffers(driver);					// glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	//unbind everything
	//glBindVertexArray(NULL);				// Unbind our Vertex Array Object
	//glBindTexture(GL_TEXTURE_2D, NULL);
	//glUseProgram(0);						// shader->unbind(); // Unbind our shader
}

// TODO: ALSO SHARED FROM DX do it on model class!?
void GLmodelClass::GetIndices()
{
    int j = 0;
    //womalog("Indices: \n");
    if (indexModelList == NULL || indexModelList->size() == 0) // BASIC object, without index? One index per vertice?
    {
        m_indexCount = m_vertexCount;			// Set the number of indices in the index array.
        indices = NEW UINT[m_indexCount];		// Create the index array.
        IF_NOT_THROW_EXCEPTION(indices);

        // getArrayIndices()
        for (UINT i = 0; i < m_indexCount; i++) {
            indices[i] = i;						// Load the index array with data:
#if (defined _DEBUG || defined  DEBUG) && DX_ENGINE_LEVEL < 29
        //   womalog("%u ", indices[i]);
        //   if (j++ >= 2) {
        //       j = 0;
        //       womalog("\n");
        //   }
#endif
        }

    }
    else {
        m_indexCount = (UINT)indexModelList->size();
        indices = NEW UINT[m_indexCount];		// Create the index array.
        IF_NOT_THROW_EXCEPTION(indices);

        // cloneArrayIndices()
        for (UINT i = 0; i < m_indexCount; i++) {
            indices[i] = indexModelList->at(i);	// Load the index array with data:
#if (defined _DEBUG || defined  DEBUG) && DX_ENGINE_LEVEL < 29
     //       womalog("%u ", indices[i]);
     //       if (j++ >= 2) {
     //           j = 0;
     //           womalog("\n");
     //       }
#endif
        }
    }
}


bool GLmodelClass::InitializeColorBuffers(/*GLopenGLclass*/ void* OpenGL)
{
	ModelColorVertexType* vertices;
	//UINT*	indices = NULL;

	m_vertexCount = (UINT) (*modelColorVertex).size();	// Set the number of vertices in the vertex array.

	// Create the vertex array.
	vertices = NEW ModelColorVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION(vertices);

	// Load the vertex array with data:
	for (UINT i = 0; i < m_vertexCount; i++)
	{
		// Load the vertex array with data:
		vertices[i].x = (*modelColorVertex)[i].x;
		vertices[i].y = (*modelColorVertex)[i].y;
		vertices[i].z = (*modelColorVertex)[i].z;

		vertices[i].r = (*modelColorVertex)[i].r;
		vertices[i].g = (*modelColorVertex)[i].g;
		vertices[i].b = (*modelColorVertex)[i].b;
		vertices[i].a = (*modelColorVertex)[i].a;

	#if (defined _DEBUG || defined  DEBUG) && DX_ENGINE_LEVEL < 29
		womalog("vertices[i].x=%f vertices[i].y=%f vertices[i].z=%f\n", vertices[i].x, vertices[i].y, vertices[i].z);
	#endif
	}

    GetIndices();


#if (defined _DEBUG || defined  DEBUG) //&& defined ANDROID_PLATFORM
	_tprintf(TEXT("[%d]: InitializeColorBuffers::glGenVertexArrays()\n"), gettid());
#endif
	glGenVertexArrays(1, &m_vertexArrayId);	// Allocate an OpenGL vertex array object.
	glBindVertexArray(m_vertexArrayId);		// Bind the vertex array object to store all the buffers and vertex attributes we create here.

	glGenBuffers(1, &m_vertexBufferId);		// Generate an ID for the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(ModelColorVertexType), vertices, GL_STATIC_DRAW);	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.

	SetOpenGLBuffers(sizeof (ModelColorVertexType), indices);

	// Now that the buffers have been loaded we can release the array data.
	SAFE_DELETE_ARRAY (vertices);
	SAFE_DELETE_ARRAY (indices);

	return true;
}

bool GLmodelClass::InitializeTextureBuffers(/*GLopenGLclass*/ void* OpenGL)
{
	ModelTextureVertexType* vertices;
	GLenum  err;

	m_vertexCount = (UINT) (*modelTextureVertex).size();	// Set the number of vertices in the vertex array.

	// Create the vertex array.
	vertices = NEW ModelTextureVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION(vertices);

	// Load the vertex array with data:
	for (UINT i = 0; i <m_vertexCount; i++)
	{
		// Load the vertex array with data:
		vertices[i].x = (*modelTextureVertex)[i].x;
		vertices[i].y = (*modelTextureVertex)[i].y;
		vertices[i].z = (*modelTextureVertex)[i].z;

		vertices[i].tu = (*modelTextureVertex)[i].tu;
		vertices[i].tv = (*modelTextureVertex)[i].tv;

		#if (defined _DEBUG || defined  DEBUG)
		womalog("vertices: %f %f %f # %f %f \n", vertices[i].x, vertices[i].y, vertices[i].z, vertices[i].tu, vertices[i].tv);
		#endif
	}

    GetIndices();

#if (defined _DEBUG || defined  DEBUG) //&& defined ANDROID_PLATFORM
	_tprintf(TEXT("[%d]: InitializeTextureBuffers::glGenVertexArrays()\n"), gettid());
#endif
	glGenVertexArrays(1, &m_vertexArrayId);	// Allocate an OpenGL vertex array object.
	err = glGetError(); if (err != GL_NO_ERROR) { _tprintf(TEXT("ERROR! glGenVertexArrays err: %04x"), err); }

	glBindVertexArray(m_vertexArrayId);		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	err = glGetError(); if (err != GL_NO_ERROR) { _tprintf(TEXT("ERROR! glBindVertexArray err: %04x"), err); }

	glGenBuffers(1, &m_vertexBufferId);		// Generate an ID for the vertex buffer.
	err = glGetError(); if (err != GL_NO_ERROR) { _tprintf(TEXT("ERROR! glGenBuffers err: %04x"), err); }

	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(ModelTextureVertexType), vertices, GL_STATIC_DRAW);

	SetOpenGLBuffers(sizeof (ModelTextureVertexType), indices);
	err = glGetError(); if (err != GL_NO_ERROR) { _tprintf(TEXT("ERROR! SetOpenGLBuffers err: %04x"), err); }

	// Now that the buffers have been loaded we can release the array data.
	SAFE_DELETE_ARRAY (vertices);
	SAFE_DELETE_ARRAY (indices);

	return true;
}

void GLmodelClass::SetOpenGLBuffers(UINT sizeofMODELvertex, UINT* indices)
{
	// Specify the location and format of the position portion of the vertex buffer:
	// ----------------------------------------------------------------------------------------
	// void glVertexAttribPointer( 	
	// GLuint index,
  	// GLint size,
  	// GLenum type,
  	// GLboolean normalized, !!! Check this ONE !!!
  	// GLsizei stride,
  	// const GLvoid * pointer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeofMODELvertex, 0);			// vec3 inputPosition;

	switch (ModelShaderType)
	{
		case SHADER_COLOR:														
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeofMODELvertex, (void*)(3 * sizeof(float)));// vec3 inputColor;
		break;

		case SHADER_TEXTURE:		// TEXTURE
		case SHADER_TEXTURE_FONT:	// TEXTURE
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeofMODELvertex, (void*)(3 * sizeof(float)));// vec2 inputTexCoord;
		break;

	}

	glEnableVertexAttribArray(0);			// 0: Vertex position.

	switch (ModelShaderType)
	{
		case SHADER_COLOR:					// 0: Vertex position.
			glEnableVertexAttribArray(1);	// 1: Vertex color.	
		break;

		case SHADER_TEXTURE:				// 0: Vertex position.
		case SHADER_TEXTURE_FONT:				
			glEnableVertexAttribArray(1);	// 1: Texture coordinates.
		break;

	}

	// Generate an ID for the INDEX BUFFER:
	glGenBuffers(1, &m_indexBufferId);
	// Bind the INDEX BUFFER and load the index data into it:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount*sizeof(UINT), indices, GL_STATIC_DRAW);
}


void GLmodelClass::SetBuffers(/*GLopenGLclass*/void* OpenGL)
{
	// Bind the vertex array object that stored all the information about the vertex and index buffers.
	glBindVertexArray(m_vertexArrayId);
}

void GLmodelClass::RenderBuffers(/*GLopenGLclass*/void* OpenGL)
{
	ASSERT (m_indexCount > 0);

	// Render the vertex buffer using the index buffer:
	if (PrimitiveTopology == LINELIST)
		glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, 0);
	else
	if (PrimitiveTopology == TRIANGLESTRIP)
		glDrawElements(GL_TRIANGLE_STRIP, m_indexCount, GL_UNSIGNED_INT, 0);
	else
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	SystemHandle->TotalVertexCounter += m_indexCount;
}


void GLmodelClass::Identity()
{
	m_worldMatrix =	m_worldMatrix.mat4identity();
}

void GLmodelClass::multiply (void* m) // in radians!!
{
	m_worldMatrix = m_worldMatrix * *(mat4*)m;;
}

void GLmodelClass::rotateX (float rZrad) // in radians!!
{
    mat4 m = mathClass.rotateX (rZrad);
    m_worldMatrix = m_worldMatrix * m;
}
void GLmodelClass::rotateY (float rZrad) // in radians!!
{
    mat4 m = mathClass.rotateY (-rZrad);	//OPEN GL ROTATE Z in oposite direction of DX
    m_worldMatrix = m_worldMatrix * m;
}
void GLmodelClass::rotateZ (float rZrad) // in radians!!
{
    mat4 m = mathClass.rotateZ (rZrad);
    m_worldMatrix = m_worldMatrix * m;
}
void GLmodelClass::scale(float x, float y, float z)
{
	m_worldMatrix.m[4*0+0] = x;
	m_worldMatrix.m[4*1+1] = y;
	m_worldMatrix.m[4*2+2] = z;
}
void GLmodelClass::translation(float x, float y, float z)
{
	m_worldMatrix.m[4*3+0] = x;
	m_worldMatrix.m[4*3+1] = y;
	m_worldMatrix.m[4*3+2] = z;
}
#endif
