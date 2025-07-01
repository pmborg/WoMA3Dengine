// --------------------------------------------------------------------------------------------
// Filename: Rfontshaderclass.cpp
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
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut14.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525256;

#include "platform.h"
#if !defined WINDOWS_PLATFORM && defined USE_RASTERTEK_TEXT_FONTV2
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
#include "standard_platform.h"
#include "log.h"
#include "woma_macros.h"

#include "Rfontshaderclass.h"

RFontShaderClass::RFontShaderClass()
{
    m_OpenGLPtr = 0;
}


RFontShaderClass::RFontShaderClass(const RFontShaderClass& other)
{
}


RFontShaderClass::~RFontShaderClass()
{
}


bool RFontShaderClass::Initialize(OpenGLClass* OpenGL)
{
    bool result;


    // Store the pointer to the OpenGL object.
    m_OpenGLPtr = OpenGL;
#if !defined ANDROID_PLATFORM
    // Set the location and names of the shader files.
    char vsFilename[] = "/mnt/c/WoMAengine2023/LinuxWoma/LinuxWoma/font.vs";
    char psFilename[] = "/mnt/c/WoMAengine2023/LinuxWoma/LinuxWoma/font.ps";
#else
    char vsFilename[] = "GLengine/font.vs";
    char psFilename[] = "GLengine/font.ps";
#endif

    // Initialize the vertex and pixel shaders.
    result = InitializeShader(vsFilename, psFilename);
    if(!result)
    {
        return false;
    }

    return true;
}


void RFontShaderClass::Shutdown()
{
    // Shutdown the shader.
    ShutdownShader();

    // Release the pointer to the OpenGL object.
    m_OpenGLPtr = 0;

    return;
}


bool RFontShaderClass::InitializeShader(char* vsFilename, char* fsFilename)
{
    const char* vertexShaderBuffer;
    const char* fragmentShaderBuffer;
    int status;

	WOMA_LOGManager_DebugMSGAUTO(TEXT("RFontShaderClass::InitializeShader: %s\n"), vsFilename);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("RFontShaderClass::InitializeShader: %s\n"), fsFilename);

    // Load the vertex shader source file into a text buffer.
    vertexShaderBuffer = LoadShaderSourceFile(vsFilename);
    if(!vertexShaderBuffer)
    {
        return false;
    }

    // Load the fragment shader source file into a text buffer.
    fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
    if(!fragmentShaderBuffer)
    {
        return false;
    }

    // Create a vertex and fragment shader object.
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Copy the shader source code strings into the vertex and fragment shader objects.
    glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
    glShaderSource(m_fragmentShader, 1, &fragmentShaderBuffer, NULL);

    // Release the vertex and fragment shader buffers.
    delete [] vertexShaderBuffer;
    vertexShaderBuffer = 0;

    delete [] fragmentShaderBuffer;
    fragmentShaderBuffer = 0;

    // Compile the shaders.
    glCompileShader(m_vertexShader);
    glCompileShader(m_fragmentShader);

    // Check to see if the vertex shader compiled successfully.
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
    if(status != 1)
    {
        // If it did not compile then write the syntax error message out to a text file for review.
        OutputShaderErrorMessage(m_vertexShader, vsFilename);
        return false;
    }

    // Check to see if the fragment shader compiled successfully.
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
    if(status != 1)
    {
        // If it did not compile then write the syntax error message out to a text file for review.
        OutputShaderErrorMessage(m_fragmentShader, fsFilename);
        return false;
    }

    // Create a shader program object.
    m_shaderProgram = glCreateProgram();

    // Attach the vertex and fragment shader to the program object.
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);

    // Bind the shader input variables.
    glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
    glBindAttribLocation(m_shaderProgram, 1, "inputTexCoord");

    // Link the shader program.
    glLinkProgram(m_shaderProgram);

    // Check the status of the link.
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
    if(status != 1)
    {
        // If it did not link then write the syntax error message out to a text file for review.
        OutputLinkerErrorMessage(m_shaderProgram);
        return false;
    }

    return true;
}


void RFontShaderClass::ShutdownShader()
{
    // Detach the vertex and fragment shaders from the program.
    glDetachShader(m_shaderProgram, m_vertexShader);
    glDetachShader(m_shaderProgram, m_fragmentShader);

    // Delete the vertex and fragment shaders.
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);

    // Delete the shader program.
    glDeleteProgram(m_shaderProgram);

    return;
}


char* RFontShaderClass::LoadShaderSourceFile(char* filename)
{
    char* buffer;

#if !defined ANDROID_PLATFORM
    long fileSize, count;
    int error;
    FILE* filePtr;

    // Open the shader file for reading in text modee.
	filePtr = fopen(filename, "r"); //AQUI-SHADER
	if(filePtr == NULL)
	{
		return 0;
	}

	// Go to the end of the file and get the size of the file.
    fseek(filePtr, 0, SEEK_END);
    fileSize = ftell(filePtr);

    // Initialize the buffer to read the shader source file into, adding 1 for an extra null terminator.
    buffer = new char[fileSize + 1];

    // Return the file pointer back to the beginning of the file.
    fseek(filePtr, 0, SEEK_SET);

    // Read the shader text file into the buffer.
    count = fread(buffer, 1, fileSize, filePtr);
	if(count != fileSize)
	{
		return 0;
	}

    // Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return 0;
	}

#else
    AAssetManager* manager = engine.app->activity->assetManager;
    AAsset* shaderFile = AAssetManager_open(manager, filename, AASSET_MODE_BUFFER);
    const void* shaderBuffer = AAsset_getBuffer(shaderFile);
    off_t fileSize = AAsset_getLength(shaderFile);

    // Initialize the buffer to read the shader source file into.
    buffer = NEW char[fileSize + 1];
    IF_NOT_THROW_EXCEPTION(buffer);

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


void RFontShaderClass::OutputShaderErrorMessage(unsigned int shaderId, char* shaderFilename)
{
    int logSize = 0;
    char* infoLog;
    ofstream fout;

    // Get the size of the string containing the information log for the failed shader compilation message.
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

    //WOMA_LOGManager_DebugMSG (TEXT("logSize: %d\n"), logSize);
    WOMA_LOGManager_DebugMSG(TEXT("FILE: %s\n"), shaderFilename);

    if (logSize > 0) {
        // Increment the size by one to handle also the null terminator.
        logSize++;

        // Create a char buffer to hold the info log.
        infoLog = NEW char[logSize];
        IF_NOT_THROW_EXCEPTION(infoLog);

        // Now retrieve the info log.
        glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
        TCHAR WinfoLog[MAX_STR_LEN] = { 0 }; atow(WinfoLog, infoLog, MAX_STR_LEN);
#if defined WINDOWS_PLATFORM
        WomaMessageBox(WinfoLog, TEXT("GL - SHADER ERROR"));
#else
        _tprintf("%s\n", infoLog);
#endif
    }
}


void RFontShaderClass::OutputLinkerErrorMessage(unsigned int programId)
{
    int InfoLogLength;

    glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
    glGetProgramInfoLog(m_shaderProgram, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    _tprintf("%s\n", &ProgramErrorMessage[0]);

    return;
}


bool RFontShaderClass::SetShaderParameters(float* worldMatrix, float* viewMatrix, float* projectionMatrix, float* pixelColor)
{
    float tpWorldMatrix[16], tpViewMatrix[16], tpProjectionMatrix[16];
    int location;

    // Transpose the matrices to prepare them for the shader.
    m_OpenGLPtr->MatrixTranspose(tpWorldMatrix, worldMatrix);
    m_OpenGLPtr->MatrixTranspose(tpViewMatrix, viewMatrix);
    m_OpenGLPtr->MatrixTranspose(tpProjectionMatrix, projectionMatrix);
    
    // Install the shader program as part of the current rendering state.
    glUseProgram(m_shaderProgram);

    // Set the world matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "worldMatrix");
    if(location == -1)
    {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, tpWorldMatrix);

    // Set the view matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "viewMatrix");
    if(location == -1)
    {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, tpViewMatrix);

    // Set the projection matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "projectionMatrix");
    if(location == -1)
    {
        return false;
    }
    glUniformMatrix4fv(location, 1, false, tpProjectionMatrix);

    // Set the texture in the pixel shader to use the data from the first texture unit.
    location = glGetUniformLocation(m_shaderProgram, "shaderTexture");
    if(location == -1)
    {
        return false;
    }
    glUniform1i(location, 0);

    // Set the font pixel color in the pixel shader.
    location = glGetUniformLocation(m_shaderProgram, "pixelColor");
    if(location == -1)
    {
        return false;
    }
    glUniform4fv(location, 1, pixelColor);

    return true;
}
#endif