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
#pragma warning( disable : 4477 )
#if (defined OPENGL3 || defined OPENGL4) && DX_ENGINE_LEVEL >= 21
#if defined ANDROID_PLATFORM
#include "AndroidEngine.h"
#include <android\asset_manager.h>
#endif

#include "womadriverclass.h"
#include "GLmathClass.h"
#include "GLopenGLclass.h"
#include "GLshaderClass.h"

#include "OSengine.h"
#include "OSmain_dir.h"
#include "mem_leak.h"


GLshaderClass::GLshaderClass() {
	CLASSLOADER();
	WomaIntegrityCheck = 1234525217;
}

GLshaderClass::~GLshaderClass() {CLASSDELETE();}

bool GLshaderClass::Initialize(SHADER_TYPE shaderType)
{
	bool result = false;
	_tprintf(TEXT("GLshaderClass::Initialize (shaderType:%d)\n"), shaderType);

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
	//GLenum  err = glGetError(); if (err != GL_NO_ERROR) { _tprintf("ERROR! glUseProgram err: %04x", err); }
}

static void validateShader(GLuint shader, TCHAR* file = 0) {
    const unsigned int BUFFER_SIZE = 512;
    char infoLog[BUFFER_SIZE];	// Have to be "char" (used by open gl)
    memset(infoLog, 0, BUFFER_SIZE);
    GLsizei length = 0;
    
	GLint shaderCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
	if(!shaderCompiled) //if(shaderCompiled == GL_FALSE)
	{
		glGetShaderInfoLog(shader, BUFFER_SIZE, &length, infoLog); // Ask OpenGL to give us the log associated with the shader
		
		#if !defined ANDROID_PLATFORM
		TCHAR Wbuffer[10 * MAX_STR_LEN]; atow(Wbuffer, infoLog, 10 * MAX_STR_LEN);
		if (length > 0) {// If we have any information to display
			printf("FILE: %s", file);
			printf("Wbuffer: %s", Wbuffer);
		}
		#else
		_tprintf("ERROR: glGetShaderInfoLog: %s\n", infoLog);
		#endif
	}
}


bool GLshaderClass::InitializeShader(SHADER_TYPE shaderType, TCHAR* vsFilename, TCHAR* fsFilename)
{
	const char* vertexShaderBuffer = NULL;
	const char* fragmentShaderBuffer = NULL;
	GLenum  err;

	_tprintf(TEXT("[%d]: InitializeShader(VS:%s PS:%s)\n"), gettid(), vsFilename, fsFilename);

	// Load the vertex shader source file into a text buffer.
	CHAR AvsFilename[MAX_STR_LEN] = { 0 }; wtoa(AvsFilename, vsFilename, MAX_STR_LEN);
	vertexShaderBuffer = LoadShaderSourceFile(AvsFilename);
	IF_NOT_RETURN_FALSE (vertexShaderBuffer);

	// Create a vertex and fragment shader object.
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
	glCompileShader(m_vertexShader);
	validateShader(m_vertexShader, vsFilename); // Validate the vertex shader
	SAFE_DELETE(vertexShaderBuffer);

	// Load the fragment shader source file into a text buffer.
	CHAR AfsFilename[MAX_STR_LEN] = { 0 }; wtoa(AfsFilename, fsFilename, MAX_STR_LEN);
	fragmentShaderBuffer = LoadShaderSourceFile(AfsFilename);
	IF_NOT_RETURN_FALSE (fragmentShaderBuffer);

	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShader, 1, &fragmentShaderBuffer, NULL);
	glCompileShader(m_fragmentShader);
	validateShader(m_fragmentShader, fsFilename); // Validate the fragment shader
	SAFE_DELETE (fragmentShaderBuffer);

#if true
	int status;

	// Check to see if the vertex shader compiled successfully.
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
	if(status != 1)
		{ OutputShaderErrorMessage(m_vertexShader, vsFilename); return false; }

	// Check to see if the fragment shader compiled successfully.
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
	if(status != 1)
		{ OutputShaderErrorMessage(m_fragmentShader, fsFilename); return false; }
#endif

	m_shaderProgram = glCreateProgram();				// Create a shader program object.
	err = glGetError(); if (err != GL_NO_ERROR) { _tprintf(TEXT("ERROR! glCreateProgram err: %04x"), err); }
	ASSERT(m_shaderProgram);

	glAttachShader(m_shaderProgram, m_vertexShader);	// Attach a vertex shader to the program
	err = glGetError(); if (err != GL_NO_ERROR) { _tprintf(TEXT("ERROR! glAttachShader m_vertexShader: %d"), err); }

	glAttachShader(m_shaderProgram, m_fragmentShader);	// Attach the fragment shader to the program
	err = glGetError(); if (err != GL_NO_ERROR) { _tprintf(TEXT("ERROR! glAttachShader m_fragmentShader: %d"), err); }

	glLinkProgram(m_shaderProgram);	// Link the shader program.
	err = glGetError(); if (err != GL_NO_ERROR) { _tprintf(TEXT("ERROR! glLinkProgram err: %04x"), err); }

	// Check the program
	GLint Result = GL_FALSE;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		OutputLinkerErrorMessage(m_shaderProgram);
		return false;
	}

	return true;
}


char* GLshaderClass::LoadShaderSourceFile(char* filename)
{
	char* buffer;

#if !defined ANDROID_PLATFORM
	ifstream fin;
	int fileSize;
	char input;

	// Open the shader source file.
	fin.open(filename);
	womalogauto(TEXT("LoadShaderSourceFile: %s\n"), filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
#if defined LINUX_PLATFORM
		STRING dir = WOMA::getCurrentDir();
		STRING file = dir;
		file.append("/../../../CoreEngine_Tutorials/DXENGINE_0");
		file.append(std::to_string(DX_ENGINE_LEVEL));
		file.append("/");
		file.append(filename);
		womalogauto(TEXT("LoadShaderSourceFile: %s\n"), file);
		fin.open(file);
		if (fin.fail())
#endif
		{
		#if defined LINUX_PLATFORM
			STRING dir = WOMA::Home;
			STRING file = dir;
			file.append("/projects/LinuxWoma0");
			file.append(std::to_string(DX_ENGINE_LEVEL));
			file.append("/bin/x64/Debug/");
			file.append(filename);
			womalogauto(TEXT("LoadShaderSourceFile: %s\n"), file);
			fin.open(file);
			if (fin.fail())
		#endif
			{
				return 0;
			}
		}
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
#else
	AAssetManager* manager = engine.app->activity->assetManager;
	AAsset* shaderFile = AAssetManager_open(manager, filename, AASSET_MODE_BUFFER);
	const void* shaderBuffer = AAsset_getBuffer(shaderFile);
	off_t fileSize = AAsset_getLength(shaderFile);
#endif

	// Initialize the buffer to read the shader source file into.
	buffer = NEW char[fileSize+1];
	IF_NOT_THROW_EXCEPTION (buffer);

#if !defined ANDROID_PLATFORM
	// Open the shader source file again.
	fin.open(filename);
	womalogauto(TEXT("LoadShaderSourceFile: %s\n"), filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
#if defined LINUX_PLATFORM
		STRING file = "../../../CoreEngine_Tutorials/DXENGINE_0";
		file.append(std::to_string(DX_ENGINE_LEVEL));
		file.append("/");
		file.append(filename);
		fin.open(file);
		womalogauto(TEXT("LoadShaderSourceFile: %s\n"), file);
		if (fin.fail())
#endif
		{
		#if defined LINUX_PLATFORM
			STRING dir = WOMA::Home;
			STRING file = dir;
			file.append("/projects/LinuxWoma0");
			file.append(std::to_string(DX_ENGINE_LEVEL));
			file.append("/bin/x64/Debug/");
			file.append(filename);
			womalogauto(TEXT("LoadShaderSourceFile: %s\n"), file);
			fin.open(file);
			if (fin.fail())
		#endif
			{
				return 0;
			}
		}
	}

	// Read the shader text file into the buffer as a block.
	fin.read(buffer, fileSize);

	// Close the file.
	fin.close();
#else
	//dest - pointer to the memory location to copy to
	//src - pointer to the memory location to copy from
	//count - number of bytes to copy
	memcpy(buffer, shaderBuffer, fileSize * sizeof(char));
	AAsset_close(shaderFile);
#endif

	// Null terminate the buffer.
	buffer[fileSize] = '\0';

	return buffer;
}

#if defined UNICODE
void GLshaderClass::OutputShaderErrorMessage(UINT shaderId, WCHAR* shaderFilename) {
	CHAR file[MAX_STR_LEN] = { 0 }; wtoa(file, shaderFilename, MAX_STR_LEN); // wchar ==> char
	OutputShaderErrorMessage(shaderId, file);
}
#endif

void GLshaderClass::OutputShaderErrorMessage(UINT shaderId, char* shaderFilename)
{
	int logSize=0;
	char* infoLog;
	ofstream fout;

	// Get the size of the string containing the information log for the failed shader compilation message.
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
	womalog(TEXT("FILE: %s\n"), shaderFilename);

	if (logSize > 0) {
		// Increment the size by one to handle also the null terminator.
		logSize++;

		// Create a char buffer to hold the info log.
		infoLog = NEW char[logSize];
		IF_NOT_THROW_EXCEPTION (infoLog);

		// Now retrieve the info log.
		glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
		TCHAR WinfoLog[MAX_STR_LEN]={ 0 }; atow(WinfoLog, infoLog, MAX_STR_LEN);
#if defined WINDOWS_PLATFORM
		WomaMessageBox(WinfoLog, TEXT("GL - SHADER ERROR"));
#else
		_tprintf("%s\n", infoLog);
#endif
	}
}


void GLshaderClass::OutputLinkerErrorMessage(UINT m_shaderProgram)
{
	int InfoLogLength;

	glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
	glGetProgramInfoLog(m_shaderProgram, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	_tprintf(TEXT("%s\n"), &ProgramErrorMessage[0]);
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


bool GLshaderClass::SetShaderParameters(SHADER_TYPE shaderType, mat4* worldMatrix, mat4* viewMatrix, mat4* projectionMatrix, int textureUnit)
{
	INT location;

	//V2:
	mat4 WVP = (*worldMatrix) * (*viewMatrix) * (*projectionMatrix);

	// Set the world matrix in the vertex shader.
	location = glGetUniformLocation(m_shaderProgram, "worldMatrix");
	//ASSERT(location >= 0);
	glUniformMatrix4fv(location, 1, false, (float*)worldMatrix);

	//V2:
	// Set the projection matrix in the vertex shader.
	location = glGetUniformLocation(m_shaderProgram, "WVP");
	//ASSERT(location >= 0);
	glUniformMatrix4fv(location, 1, false, (float*)&WVP);

	// SET TUTURE!

	return true;
}

#endif
