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
#include "GLshaderClass.h"
#include "mem_leak.h"
#include "OSengine.h"
//#include "SystemPlatform.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID

GLshaderClass::GLshaderClass() {CLASSLOADER();}

GLshaderClass::~GLshaderClass() {CLASSDELETE();}

bool GLshaderClass::Initialize(SHADER_TYPE shaderType)
{
	bool result = false;

	// Initialize the vertex and pixel shaders.
	switch (shaderType)
	{
		case SHADER_COLOR:
		result = InitializeShader(shaderType, TEXT("GLengine/color.vs"), TEXT("GLengine/color.ps") );
		break;

	}

	IF_NOT_RETURN_FALSE(result);

	return true;
}

void GLshaderClass::Shutdown()
{
	ShutdownShader();				// Shutdown the vertex and pixel shaders as well as the related objects.
}

void GLshaderClass::SetShader()
{
	glUseProgram(m_shaderProgram);	// Install the shader program as part of the current rendering state.
}

static void validateShader(GLuint shader, TCHAR* file = 0) {
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];	// Have to be "char" (used by open gl)
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
    
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) 
	{
		glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer); // Ask OpenGL to give us the log associated with the shader

		/*
		STRINGSTREAM error;
		error << "Shader " << shader << " (" << (file?file:"") << ") compile error:" << endl; // Output the information
		if (length > 0) // If we have any information to display
			WomaMessageBox ((TCHAR*)error.str().c_str(), buffer); // Output the information);
		*/
		TCHAR Wbuffer[10*MAX_STR_LEN]; atow(Wbuffer, buffer, 10*MAX_STR_LEN);

		if (length > 0) // If we have any information to display
			WOMA::WomaMessageBox (Wbuffer, file); // Output the information);
	}
}


bool GLshaderClass::InitializeShader(SHADER_TYPE shaderType, TCHAR* vsFilename, TCHAR* fsFilename)
{
	const char* vertexShaderBuffer = NULL;
	const char* fragmentShaderBuffer = NULL;
	int status;

	// Load the vertex shader source file into a text buffer.
	CHAR AvsFilename[MAX_STR_LEN] = { 0 }; wtoa(AvsFilename, vsFilename, MAX_STR_LEN);
	vertexShaderBuffer = LoadShaderSourceFile(AvsFilename);
	IF_NOT_RETURN_FALSE (vertexShaderBuffer);

	// Load the fragment shader source file into a text buffer.
	CHAR AfsFilename[MAX_STR_LEN] = { 0 }; wtoa(AfsFilename, fsFilename, MAX_STR_LEN);
	fragmentShaderBuffer = LoadShaderSourceFile(AfsFilename);
	IF_NOT_RETURN_FALSE (fragmentShaderBuffer);

	// Create a vertex and fragment shader object.
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
	glCompileShader(m_vertexShader);
	validateShader(m_vertexShader, vsFilename); // Validate the vertex shader
	SAFE_DELETE(vertexShaderBuffer);

	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShader, 1, &fragmentShaderBuffer, NULL);
	glCompileShader(m_fragmentShader);
	validateShader(m_fragmentShader, fsFilename); // Validate the fragment shader
	SAFE_DELETE (fragmentShaderBuffer);

	/*
	// Check to see if the vertex shader compiled successfully.
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
	if(status != 1)
		{ OutputShaderErrorMessage(m_vertexShader, vsFilename); return false; }

	// Check to see if the fragment shader compiled successfully.
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
	if(status != 1)
		{ OutputShaderErrorMessage(m_fragmentShader, fsFilename); return false; }
	*/

	m_shaderProgram = glCreateProgram();				// Create a shader program object.
	glAttachShader(m_shaderProgram, m_vertexShader);	// Attach a vertex shader to the program
	glAttachShader(m_shaderProgram, m_fragmentShader);	// Attach the fragment shader to the program

	// Bind the shader input variables.
	switch (shaderType)
	{
		case SHADER_COLOR:
		glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
		glBindAttribLocation(m_shaderProgram, 1, "inputColor");
		break;

		case SHADER_TEXTURE:
		glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
		glBindAttribLocation(m_shaderProgram, 1, "inputTexCoord");
		break;

		case SHADER_TEXTURE_LIGHT:
		case SHADER_TEXTURE_LIGHT_RENDERSHADOW:
		glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
		glBindAttribLocation(m_shaderProgram, 1, "inputTexCoord");
		glBindAttribLocation(m_shaderProgram, 2, "inputNormal");
		break;

		default:
			return false;
	}

	glLinkProgram(m_shaderProgram);	// Link the shader program.

	// Check the status of the link.
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
	if(status != 1)
		{ OutputLinkerErrorMessage(m_shaderProgram); return false; }

	return true;
}


char* GLshaderClass::LoadShaderSourceFile(char* filename)
{
	ifstream fin;
	int fileSize;
	char input;
	char* buffer;

	// Open the shader source file.
	fin.open(filename);

	// If it could not open the file then exit.
	if(fin.fail())
	{
		return 0;
	}

	// Initialize the size of the file.
	fileSize = 0;

	// Read the first element of the file.
	fin.get(input);

	// Count the number of elements in the text file.
	while(!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}

	// Close the file for now.
	fin.close();

	// Initialize the buffer to read the shader source file into.
	buffer = NEW char[fileSize+1];
	IF_NOT_THROW_EXCEPTION (buffer);

	// Open the shader source file again.
	fin.open(filename);

	// Read the shader text file into the buffer as a block.
	fin.read(buffer, fileSize);

	// Close the file.
	fin.close();

	// Null terminate the buffer.
	buffer[fileSize] = '\0';

	return buffer;
}


void GLshaderClass::OutputShaderErrorMessage(UINT shaderId, char* shaderFilename)
{
	int logSize=0;
	char* infoLog;
	ofstream fout;

	// Get the size of the string containing the information log for the failed shader compilation message.
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
	WOMA_LOGManager_DebugMSG (TEXT("logSize: %d\n"), logSize);

	if (logSize > 0) {
		// Increment the size by one to handle also the null terminator.
		logSize++;

		// Create a char buffer to hold the info log.
		infoLog = NEW char[logSize];
		IF_NOT_THROW_EXCEPTION (infoLog);

		// Now retrieve the info log.
		glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
		TCHAR WinfoLog[MAX_STR_LEN]={ 0 }; atow(WinfoLog, infoLog, MAX_STR_LEN);
		WOMA::WomaMessageBox (WinfoLog, TEXT("GL - SHADER ERROR"));
	}
}


void GLshaderClass::OutputLinkerErrorMessage(UINT programId)
{
	int logSize=0;
	char* infoLog;
	ofstream fout;

	// Get the size of the string containing the information log for the failed shader compilation message:
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
	WOMA_LOGManager_DebugMSG (TEXT("logSize: %d\n"), logSize);

	if (logSize > 0) 
	{
		// Increment the size by one to handle also the null terminator:
		logSize++;

		// Create a char buffer to hold the info log:
		infoLog = NEW char[logSize];
		IF_NOT_THROW_EXCEPTION (infoLog);

		// Now retrieve the info log:
		glGetProgramInfoLog(programId, logSize, NULL, infoLog);
		TCHAR WinfoLog[MAX_STR_LEN]={ 0 }; atow(WinfoLog, infoLog, MAX_STR_LEN);
		WOMA::WomaMessageBox (WinfoLog, TEXT("GL SHADER LINKER ERROR"));
	}
}


void GLshaderClass::ShutdownShader()
{
	// Detach the vertex and fragment shaders from the program:
	glDetachShader(m_shaderProgram, m_vertexShader);
	glDetachShader(m_shaderProgram, m_fragmentShader);

	// Delete the vertex and fragment shaders:
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);

	// Delete the shader program:
	glDeleteProgram(m_shaderProgram);
}


//bool GLshaderClass::SetShaderParameters(SHADER_TYPE shaderType, float* worldMatrix, float* viewMatrix, float* projectionMatrix, int textureUnit)
bool GLshaderClass::SetShaderParameters(SHADER_TYPE shaderType, mat4* worldMatrix, mat4* viewMatrix, mat4* projectionMatrix, int textureUnit)
{
	UINT location;

	//V2:
	mat4 WVP = (*worldMatrix) * (*viewMatrix) * (*projectionMatrix);

	//mat4 WVP;
	//mat4 WVP_ = (*worldMatrix) * (*viewMatrix) * (*projectionMatrix);
	//MatrixTranspose(&WVP, &WVP_);

	//V3:
	/*
	mat4 WVP_ = (*worldMatrix) * (*viewMatrix) * (*projectionMatrix);
	mat4 WVPT;
	MatrixTranspose(&WVPT, &WVP_);
	mat4* WVP = &WVPT;
	*/

	// Set the world matrix in the vertex shader.
	location = glGetUniformLocation(m_shaderProgram, "worldMatrix");
	glUniformMatrix4fv(location, 1, false, (float*)worldMatrix);

	// Set the view matrix in the vertex shader.
	//location = glGetUniformLocation(m_shaderProgram, "viewMatrix");
	//glUniformMatrix4fv(location, 1, false, (float*)viewMatrix);

	// Set the projection matrix in the vertex shader.
	//location = glGetUniformLocation(m_shaderProgram, "projectionMatrix");
	//glUniformMatrix4fv(location, 1, false, (float*)projectionMatrix);

	//V2:
	// Set the projection matrix in the vertex shader.
	location = glGetUniformLocation(m_shaderProgram, "WVP");
	glUniformMatrix4fv(location, 1, false, (float*)&WVP);

	return true;
}

