// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: GLmodelClass.cpp
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

#include "platform.h"

#include "ModelClass.h"
#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "GLmodelClass.h"
#include "GLshaderClass.h"
#include "mem_leak.h"
#include "winSystemClass.h"

GLmodelClass::GLmodelClass(bool model3d) 
{
	CLASSLOADER();
	WomaIntegrityCheck = 1234567831;

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

	m_xTexture = 1.0f;

	// Initialize the world/model matrix to the identity matrix:
	m_worldMatrix = m_worldMatrix.mat4identity();  //Identity();
	m_viewMatrix = m_viewMatrix.mat4identity();
}

GLmodelClass::~GLmodelClass(){
	CLASSDELETE();
}

void GLmodelClass::UpdateDynamic(void* Driver, std::vector<ModelColorVertexType>* lightVertexVector)
{
	static float m_previousPosX = -10000;
	static float m_previousPosY = -10000;
	static float m_previousPosZ = -10000;

	float positionX = (*lightVertexVector)[1].x;
	float positionY = (*lightVertexVector)[1].y;
	float positionZ = (*lightVertexVector)[1].z;

	//We check if the position to render this image has changed. If it hasn't changed then we just exit since the vertex buffer doesn't need 
	//any changes for this frame. This check can save us a lot of processing.
	if (((positionX == m_previousPosX) && (positionY == m_previousPosY)) && !((WomaDriverClass*)Driver)->RenderfirstTime)
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
	SAFE_DELETE_ARRAY(vertices);
};

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
		result = InitializeTextureBuffers(NULL);
		break;
	case SHADER_TEXTURE_LIGHT:
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
		result = InitializeTextureLightBuffers(NULL);
		break;
	}
	IF_NOT_RETURN_FALSE(result);

	// Create the shader to this object:
	// ------------------------------------------------------------------------------------------------
	m_Shader = NEW GLshaderClass;
	IF_NOT_THROW_EXCEPTION (m_Shader);

	result = m_Shader->Initialize(ModelShaderType);
	if(!result)
		{ WomaFatalExceptionW(TEXT("Could not initialize the shader object.")); return false; }

	// Load Texture (manually)
	// ------------------------------------------------------------------------------------------------
	if (ModelShaderType >= SHADER_TEXTURE)
	{
		// Create the texture object for this model:
		m_Texture = NEW GLtextureClass;
		IF_NOT_THROW_EXCEPTION (m_Texture);
		meshSRV.push_back(m_Texture);

		// Initialize the texture object:
		result = m_Texture->Initialize(WOMA::LoadFile((TCHAR*)(*textureFile)[0].c_str()), 0, /*wrap*/ Model3D);
		if(!result)
			{ WOMA::WomaMessageBox((TCHAR*)(*textureFile)[0].c_str(), TEXT("Texture File not found")); return false; }

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
bool GLmodelClass::LoadColor(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<ModelColorVertexType> *model, std::vector<UINT>* indexList, UINT instanceCount)
{
	MODEL_NAME = objectName;
	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_COLOR;
	else
		ModelShaderType = shader_type;

	modelColorVertex = model;
	indexModelList = indexList;
	return InitializeVertexIndexBuffers();
}

bool GLmodelClass::LoadTexture(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureVertexType> *model, std::vector<UINT>* indexList, UINT instanceCount)
{
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

bool GLmodelClass::LoadLight(TCHAR* objectName, void* driver, SHADER_TYPE shader_type, std::vector<STRING> *textureFile, std::vector<ModelTextureLightVertexType> *model, std::vector<UINT>* indexList, UINT instanceCount)
{
	MODEL_NAME = objectName;
	if (shader_type == SHADER_AUTO)
		ModelShaderType = SHADER_TEXTURE_LIGHT;
	else
		ModelShaderType = shader_type;

	modelTextureLightVertex = model;
	indexModelList = indexList;
	return InitializeVertexIndexBuffers(textureFile);
}

bool GLmodelClass::LoadModel(TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow, bool renderShadow, UINT instanceCount)
{

	const TCHAR* extension = _tcsrchr(filename.c_str(), '.');

	if (_tcsicmp(extension, TEXT(".obj")) == 0)
	{
		bool b = modelClass.LoadOBJ(this, shader_type, g_driver, filename, castShadow, renderShadow, instanceCount);
		if (b)
			modelClass.CreateObject(this, (TCHAR*)filename.c_str(), g_driver, shader_type /*SHADER_AUTO*/, filename, castShadow, renderShadow); // Auto Detect Shader Type
	}

	return true;
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

	glDisableVertexAttribArray(2);

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

	bool GLmodelClass::RenderSprite(void* Driver , int positionX, int positionY, float scale, float fade)
	{
		model_fade = fade;

		if (!UpdateBuffersRotY(Driver, positionX, positionY))
			return false;

		m_worldMatrix.mat4identity();

		//scale2D = scale;
		if (scale != 1) {
			m_worldMatrix.m[4 * 0 + 0] = scale;
			m_worldMatrix.m[4 * 1 + 1] = scale;
			m_worldMatrix.m[4 * 2 + 2] = scale;
		}

		float Ypos = (SystemHandle->AppSettings->WINDOW_HEIGHT-45-32) / 2 - m_worldMatrix.m[10] * SpriteTextureHeight / 2;
		m_worldMatrix.m[13] = Ypos;

		Render((WomaDriverClass*)Driver, CAMERA_NORMAL, PROJECTION_ORTHOGRAPH);

		return true;
	}

	bool GLmodelClass::UpdateBuffersRotY(void* Driver, int positionX, int positionY)
	{
		static int m_previousPosX = -10000;
		static int m_previousPosY = -10000;
		static bool RenderfirstTime = ((WomaDriverClass*)Driver)->RenderfirstTime;

		float left, right, top, bottom;
		ModelTextureVertexType* vertices;

		if (((positionX == m_previousPosX) && (positionY == m_previousPosY)) && !RenderfirstTime)
			return true;

		// If it has changed then update the position it is being rendered to.
		m_previousPosX = positionX;
		m_previousPosY = positionY;

		//The four sides of the image need to be calculated. See the diagram at the top of the tutorial for a complete explaination.
		left = (float)((SystemHandle->AppSettings->WINDOW_WIDTH / 2) * -1) + (float)positionX;	// Calculate the screen coordinates of the left side of the bitmap.
		right = left + (float)SpriteTextureWidth;												// Calculate the screen coordinates of the right side of the bitmap.
		top = (float)(SystemHandle->AppSettings->WINDOW_HEIGHT / 2) - (float)positionY;			// Calculate the screen coordinates of the top of the bitmap.
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

		// Get a pointer to the buffer's actual location in memory.
		void* dataPtr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		// Copy the vertex data into memory.
		memcpy(dataPtr, vertices, m_vertexCount * sizeof(ModelColorVertexType));

		// Unlock the vertex buffer.
		glUnmapBuffer(GL_ARRAY_BUFFER);


		SAFE_DELETE_ARRAY(vertices);				// Release the vertex array as it is no longer needed.

		return true;
	}


void GLmodelClass::RenderWithFade(WomaDriverClass* driver, float fadeLight)
{
	//m_Shader->fade = fadeLight;
	Render(driver);
}
void GLmodelClass::RenderSky(WomaDriverClass* driver, UINT camera, float fadeLight)
{
	m_Shader->PSfade = fadeLight;
	m_Shader->isSky = true;
	RenderWithFade(driver, fadeLight);
}
void GLmodelClass::Render(/*GLopenGLclass*/WomaDriverClass* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
	GLopenGLclass* driver = (GLopenGLclass*)Driver;
	
	switch (projection)
	{
		case PROJECTION_PERSPECTIVE:
			 projectionMatrix = (driver->m_projectionMatrix);
			break;

		case PROJECTION_ORTHOGRAPH:
			projectionMatrix = driver->m_orthoMatrix;
		break;
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

		case CAMERA_SKY:
			m_viewMatrix = ((GLcameraClass*)driver->gl_CameraSKY)->m_viewMatrix;
			break;
	}

	m_Shader->SetShader(); //glUseProgram(m_Shader->m_shaderProgram); // 
	m_Shader->PSfade = model_fade;
	if (RENDER_PAGE >= 26)
		m_Shader->lightType = 2;
	else
		m_Shader->lightType = 1;

	// Set the color shader as the current shader program and set the matrices that it will use for rendering:
	if (ModelShaderType == SHADER_COLOR)
		m_Shader->SetShaderParameters(ModelShaderType, &m_worldMatrix, &m_viewMatrix, &projectionMatrix);

	else 
	{
		m_Shader->SetShaderParameters(ModelShaderType, &m_worldMatrix, &m_viewMatrix, &projectionMatrix, 0 /*m_Texture->m_textureID*/);

		// Step 3: Set shader texture resource(s)
		glActiveTexture(GL_TEXTURE0 + 0);	// Set the unique texture unit in which to store the data.
		glBindTexture(GL_TEXTURE_2D, m_Texture->m_textureID);
	}

	// Step 4: Render the model
	glBindVertexArray(m_vertexArrayId);		// SetBuffers(driver);

	RenderBuffers(driver);

	//unbind everything
	glBindVertexArray(NULL);				// Unbind our Vertex Array Object
	glBindTexture(GL_TEXTURE_2D, NULL);
	glUseProgram(0);						// shader->unbind(); // Unbind our shader
}

// TODO: ALSO SHARED FROM DX do it on model class!?
void GLmodelClass::GetIndices()
{
	if ( indexModelList == NULL || indexModelList->size() == 0) // BASIC object, without index? One index per vertice?
	{
		m_indexCount = m_vertexCount;			// Set the number of indices in the index array.
		indices = NEW UINT[m_indexCount];		// Create the index array.
		IF_NOT_THROW_EXCEPTION(indices);

		// getArrayIndices()
		for (UINT i = 0; i < m_indexCount; i++)
			indices[i] = i;						// Load the index array with data:

	} else {
		m_indexCount = (UINT)indexModelList->size();
		indices = NEW UINT[m_indexCount];		// Create the index array.
		IF_NOT_THROW_EXCEPTION (indices);

		// cloneArrayIndices()
		for (UINT i = 0; i < m_indexCount; i++)
			indices[i] = indexModelList->at(i);	// Load the index array with data:
	}
}


bool GLmodelClass::InitializeColorBuffers(/*GLopenGLclass*/ void* OpenGL)
{
	ModelColorVertexType* vertices;
	//UINT*	indices = NULL;

	m_vertexCount = (UINT) (*modelColorVertex).size();	// Set the number of vertices in the vertex array.

	GetIndices();

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
	#if _DEBUG
		WOMA_LOGManager_DebugMSG("vertices[i].x=%f vertices[i].y=%f vertices[i].z=%f\n", vertices[i].x, vertices[i].y, vertices[i].z);
	#endif
	}

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

	m_vertexCount = (UINT) (*modelTextureVertex).size();	// Set the number of vertices in the vertex array.

	GetIndices();

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

		#if _DEBUG
		WOMA_LOGManager_DebugMSG("vertices: %d %d %d - %f %f \n", vertices[i].x, vertices[i].y, vertices[i].z, vertices[i].tu, vertices[i].tv);
		#endif
	}

	glGenVertexArrays(1, &m_vertexArrayId);	// Allocate an OpenGL vertex array object.
	glBindVertexArray(m_vertexArrayId);		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	glGenBuffers(1, &m_vertexBufferId);		// Generate an ID for the vertex buffer.

	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(ModelTextureVertexType), vertices, GL_STATIC_DRAW);

	SetOpenGLBuffers(sizeof (ModelTextureVertexType), indices);

	// Now that the buffers have been loaded we can release the array data.
	SAFE_DELETE_ARRAY (vertices);
	SAFE_DELETE_ARRAY (indices);

	return true;
}

bool GLmodelClass::InitializeTextureLightBuffers(/*GLopenGLclass*/ void* OpenGL)
{
	ModelTextureLightVertexType* vertices;

	m_vertexCount = (UINT) (*modelTextureLightVertex).size();	// Set the number of vertices in the vertex array.
	GetIndices();

	// Create the vertex array.
	vertices = NEW ModelTextureLightVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION(vertices);

	// Load the vertex array with data:
	for (UINT i = 0; i < m_vertexCount; i++)
	{
		// Load the vertex array with data:
		vertices[i].x = (*modelTextureLightVertex)[i].x;
		vertices[i].y = (*modelTextureLightVertex)[i].y;
		vertices[i].z = (*modelTextureLightVertex)[i].z;

		vertices[i].tu = (*modelTextureLightVertex)[i].tu;
		vertices[i].tv = (*modelTextureLightVertex)[i].tv;

		vertices[i].nx = (*modelTextureLightVertex)[i].nx;
		vertices[i].ny = (*modelTextureLightVertex)[i].ny;
		vertices[i].nz = (*modelTextureLightVertex)[i].nz;
	}

	glGenVertexArrays(1, &m_vertexArrayId);	// Allocate an OpenGL vertex array object.
	glBindVertexArray(m_vertexArrayId);		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	glGenBuffers(1, &m_vertexBufferId);		// Generate an ID for the vertex buffer.

	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(ModelTextureLightVertexType), vertices, GL_STATIC_DRAW);

	SetOpenGLBuffers(sizeof (ModelTextureLightVertexType), indices);

	// Now that the buffers have been loaded we can release the array data.
	SAFE_DELETE_ARRAY (vertices);
	SAFE_DELETE_ARRAY (indices);

	return true;
}

void GLmodelClass::SetOpenGLBuffers(UINT sizeofMODELvertex, UINT* indices)
{
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

		case SHADER_TEXTURE_LIGHT:				
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:	// 0: Vertex position.
			glEnableVertexAttribArray(1);		// 1: Texture coordinates.
			glEnableVertexAttribArray(2);		// 2: Normals.
		break;
	}

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
	glVertexAttribPointer(0, sizeof(GLint), GL_FLOAT, false, sizeofMODELvertex, 0);	// POSITION

	switch (ModelShaderType)
	{
		case SHADER_COLOR: // COLOR
		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeofMODELvertex, (unsigned char*)NULL + (3 * sizeof(float)));
		break;

		case SHADER_TEXTURE: // TEXTURE
		case SHADER_TEXTURE_FONT: // TEXTURE
		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeofMODELvertex, (unsigned char*)NULL + (3 * sizeof(float)));
		break;

		case SHADER_TEXTURE_LIGHT: // TEXTURE + LIGHT
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeofMODELvertex, (unsigned char*)NULL + (3 * sizeof(float)));

		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeofMODELvertex, (unsigned char*)NULL + (5 * sizeof(float)));
		break;
	}

	// Generate an ID for the index buffer:
	glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it:
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
