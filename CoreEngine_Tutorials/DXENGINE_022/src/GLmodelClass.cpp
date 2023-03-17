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

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "GLmodelClass.h"
#include "GLshaderClass.h"
#include "mem_leak.h"
//#include "SystemPlatform.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID

#include "winSystemClass.h"

GLmodelClass::GLmodelClass(bool model3d) 
{
	CLASSLOADER();

	indices = NULL;

	// SUPER: ----------------------------------------------------------------------
	m_ObjId = 0;
	ModelShaderType			= SHADER_AUTO;

	Model3D = model3d;
	m_Shader = NULL;

	// Initialize the world/model matrix to the identity matrix:
	m_worldMatrix = m_worldMatrix.mat4identity();  //Identity();
	m_viewMatrix = m_viewMatrix.mat4identity();
}

GLmodelClass::~GLmodelClass(){
	CLASSDELETE();
}

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
		result = InitializeTextureBuffers(NULL);
		break;
	case SHADER_TEXTURE_LIGHT:
	case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
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
		// TODO FOR for all textures
		
		result = m_Texture->Initialize(WOMA::LoadFile((TCHAR*)(*textureFile)[0].c_str()), 0, /*wrap*/ Model3D);
		if(!result)
			{ WOMA::WomaMessageBox((TCHAR*)(*textureFile)[0].c_str(), TEXT("Texture File not found")); return false; }
	}

	if (!Model3D)
	{
		SpriteTextureWidth = m_Texture->width;
		SpriteTextureHeight = m_Texture->height;
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


void GLmodelClass::RenderWithFade(WomaDriverClass* driver, float fadeLight)
{
	//m_Shader->fade = fadeLight;
	Render(driver);
}

void GLmodelClass::Render(/*GLopenGLclass*/WomaDriverClass* Driver, UINT camera, UINT projection, UINT pass, void* lightViewMatrix, void* ShadowProjectionMatrix)
{
	GLopenGLclass* driver = (GLopenGLclass*)Driver;
	//mat4* viewMatrix = NULL;
	//mat4* projectionMatrix = NULL;
	// Get the world, view, and projection matrices from the opengl and camera objects:
	
	switch (projection)
	{
		case PROJECTION_PERSPECTIVE:
			 projectionMatrix = (driver->m_projectionMatrix);
			break;

	}

	switch (camera)
	{
		case CAMERA_NORMAL:
			if (projection == PROJECTION_PERSPECTIVE)
			{
				m_viewMatrix = ((GLcameraClass*)driver->m_Camera)->m_viewMatrix;
			}
			else
			{
				m_viewMatrix = m_viewMatrix;
				m_viewMatrix.mat4identity();
				m_viewMatrix.m[14] = 1;
			}
			
			break;

	}

	glUseProgram(m_Shader->m_shaderProgram); // m_Shader->SetShader();

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
	glBindVertexArray(m_vertexArrayId);				// SetBuffers(driver);

	RenderBuffers(driver);

/*
	//unbind everything
	glBindVertexArray(0);				// Unbind our Vertex Array Object
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);					// shader->unbind(); // Unbind our shader
*/
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
	//UINT* indices;

	m_vertexCount = (UINT) (*modelTextureVertex).size();	// Set the number of vertices in the vertex array.

	GetIndices();	//m_indexCount = m_vertexCount;	// Set the number of indices in the index array.

	// Create the vertex array.
	vertices = NEW ModelTextureVertexType[m_vertexCount];
	IF_NOT_THROW_EXCEPTION(vertices);

	// Create the index array.
	//indices = NEW UINT[m_indexCount];
	//IF_NOT_THROW_EXCEPTION(indices);

	// Load the vertex array with data:
	for (UINT i = 0; i <m_vertexCount; i++)
	{
		// Load the vertex array with data:
		vertices[i].x = (*modelTextureVertex)[i].x;
		vertices[i].y = (*modelTextureVertex)[i].y;
		vertices[i].z = (*modelTextureVertex)[i].z;

		vertices[i].tu = (*modelTextureVertex)[i].tu;
		vertices[i].tv = (*modelTextureVertex)[i].tv;

		#if true && _DEBUG
		WOMA_LOGManager_DebugMSG("vertices: %d %d %d - %f %f \n", vertices[i].x, vertices[i].y, vertices[i].z, vertices[i].tu, vertices[i].tv);
		#endif

		// Load the index array with data:
		//indices[i] = i;
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

void GLmodelClass::SetOpenGLBuffers(UINT sizeofMODELvertex, UINT* indices)
{
	glEnableVertexAttribArray(0);			// 0: Vertex position.

	switch (ModelShaderType)
	{
		case SHADER_COLOR:					// 0: Vertex position.
			glEnableVertexAttribArray(1);	// 1: Vertex color.	
		break;

		case SHADER_TEXTURE:				// 0: Vertex position.
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
