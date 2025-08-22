// --------------------------------------------------------------------------------------------
// Filename: Ropenglclass.cpp
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
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut14.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

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

#include "Ropenglclass.h"


OpenGLClass::OpenGLClass()
{
}

OpenGLClass::~OpenGLClass()
{
}

#if !defined ANDROID_PLATFORM
bool OpenGLClass::Initialize(int screenWidth, int screenHeight, float screenNear, float screenDepth, bool vsync)
{
    GLXDrawable drawable;
    float fieldOfView, screenAspect;

    // Store the screen size.
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Initialize the world/model matrix to the identity matrix.
    BuildIdentityMatrix(m_worldMatrix);

    // Set the field of view and screen aspect ratio.
    fieldOfView = 3.14159265358979323846f / 4.0f;
    screenAspect = (float)screenWidth / (float)screenHeight;

    // Build the perspective projection matrix.
    BuildPerspectiveFovMatrix(m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

    // Create an orthographic projection matrix for 2D rendering.
	BuildOrthoMatrix(m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

    return true;
}
#else
bool OpenGLClass::Initialize(int screenWidth, int screenHeight, float screenNear, float screenDepth, bool vsync)
{
    float fieldOfView, screenAspect;

    // Store the screen size.
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Initialize the world/model matrix to the identity matrix.
    BuildIdentityMatrix(m_worldMatrix);

    // Set the field of view and screen aspect ratio.
    fieldOfView = 3.14159265358979323846f / 4.0f;
    screenAspect = (float)screenWidth / (float)screenHeight;

    // Build the perspective projection matrix.
    BuildPerspectiveFovMatrix(m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

    // Create an orthographic projection matrix for 2D rendering.
    BuildOrthoMatrix(m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

    return true;
}
#endif

void OpenGLClass::Shutdown()
{

    return;
}

void OpenGLClass::BuildIdentityMatrix(float* matrix)
{
    matrix[0]  = 1.0f;
    matrix[1]  = 0.0f;
    matrix[2]  = 0.0f;
    matrix[3]  = 0.0f;

    matrix[4]  = 0.0f;
    matrix[5]  = 1.0f;
    matrix[6]  = 0.0f;
    matrix[7]  = 0.0f;

    matrix[8]  = 0.0f;
    matrix[9]  = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;

    return;
}


void OpenGLClass::BuildPerspectiveFovMatrix(float* matrix, float fieldOfView, float screenAspect, float screenNear, float screenDepth)
{
    matrix[0]  = 1.0f / (screenAspect * tan(fieldOfView * 0.5f));
    matrix[1]  = 0.0f;
    matrix[2]  = 0.0f;
    matrix[3]  = 0.0f;

    matrix[4]  = 0.0f;
    matrix[5]  = 1.0f / tan(fieldOfView * 0.5f);
    matrix[6]  = 0.0f;
    matrix[7]  = 0.0f;

    matrix[8]  = 0.0f;
    matrix[9]  = 0.0f;
    matrix[10] = screenDepth / (screenDepth - screenNear);
    matrix[11] = 1.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = (-screenNear * screenDepth) / (screenDepth - screenNear);
    matrix[15] = 0.0f;

    return;
}


void OpenGLClass::BuildOrthoMatrix(float* matrix, float screenWidth,  float screenHeight, float screenNear, float screenDepth)
{
    matrix[0]  = 2.0f / screenWidth;
    matrix[1]  = 0.0f;
    matrix[2]  = 0.0f;
    matrix[3]  = 0.0f;

    matrix[4]  = 0.0f;
    matrix[5]  = 2.0f / screenHeight;
    matrix[6]  = 0.0f;
    matrix[7]  = 0.0f;

    matrix[8]  = 0.0f;
    matrix[9]  = 0.0f;
    matrix[10] = 1.0f / (screenDepth - screenNear);
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = screenNear / (screenNear - screenDepth);
    matrix[15] = 1.0f;

    return;
}


void OpenGLClass::GetWorldMatrix(float* matrix)
{
    matrix[0]  = m_worldMatrix[0];
    matrix[1]  = m_worldMatrix[1];
    matrix[2]  = m_worldMatrix[2];
    matrix[3]  = m_worldMatrix[3];

    matrix[4]  = m_worldMatrix[4];
    matrix[5]  = m_worldMatrix[5];
    matrix[6]  = m_worldMatrix[6];
    matrix[7]  = m_worldMatrix[7];

    matrix[8]  = m_worldMatrix[8];
    matrix[9]  = m_worldMatrix[9];
    matrix[10] = m_worldMatrix[10];
    matrix[11] = m_worldMatrix[11];

    matrix[12] = m_worldMatrix[12];
    matrix[13] = m_worldMatrix[13];
    matrix[14] = m_worldMatrix[14];
    matrix[15] = m_worldMatrix[15];

    return;
}


void OpenGLClass::GetProjectionMatrix(float* matrix)
{
    matrix[0]  = m_projectionMatrix[0];
    matrix[1]  = m_projectionMatrix[1];
    matrix[2]  = m_projectionMatrix[2];
    matrix[3]  = m_projectionMatrix[3];

    matrix[4]  = m_projectionMatrix[4];
    matrix[5]  = m_projectionMatrix[5];
    matrix[6]  = m_projectionMatrix[6];
    matrix[7]  = m_projectionMatrix[7];

    matrix[8]  = m_projectionMatrix[8];
    matrix[9]  = m_projectionMatrix[9];
    matrix[10] = m_projectionMatrix[10];
    matrix[11] = m_projectionMatrix[11];

    matrix[12] = m_projectionMatrix[12];
    matrix[13] = m_projectionMatrix[13];
    matrix[14] = m_projectionMatrix[14];
    matrix[15] = m_projectionMatrix[15];

    return;
}


void OpenGLClass::GetOrthoMatrix(float* matrix)
{
    matrix[0]  = m_orthoMatrix[0];
    matrix[1]  = m_orthoMatrix[1];
    matrix[2]  = m_orthoMatrix[2];
    matrix[3]  = m_orthoMatrix[3];

    matrix[4]  = m_orthoMatrix[4];
    matrix[5]  = m_orthoMatrix[5];
    matrix[6]  = m_orthoMatrix[6];
    matrix[7]  = m_orthoMatrix[7];

    matrix[8]  = m_orthoMatrix[8];
    matrix[9]  = m_orthoMatrix[9];
    matrix[10] = m_orthoMatrix[10];
    matrix[11] = m_orthoMatrix[11];

    matrix[12] = m_orthoMatrix[12];
    matrix[13] = m_orthoMatrix[13];
    matrix[14] = m_orthoMatrix[14];
    matrix[15] = m_orthoMatrix[15];

    return;
}

void OpenGLClass::MatrixRotationX(float* matrix, float angle)
{
	matrix[0]  = 1.0f;
	matrix[1]  = 0.0f;
	matrix[2]  = 0.0f;
	matrix[3]  = 0.0f;

	matrix[4]  = 0.0f;
	matrix[5]  = cosf(angle);
	matrix[6]  = sinf(angle);
	matrix[7]  = 0.0f;

	matrix[8]  = 0.0f;
	matrix[9]  = -sinf(angle);
	matrix[10] = cosf(angle);
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	return;
}


void OpenGLClass::MatrixRotationY(float* matrix, float angle)
{
	matrix[0]  = cosf(angle);
	matrix[1]  = 0.0f;
	matrix[2]  = -sinf(angle);
	matrix[3]  = 0.0f;

	matrix[4]  = 0.0f;
	matrix[5]  = 1.0f;
	matrix[6]  = 0.0f;
	matrix[7]  = 0.0f;

	matrix[8]  = sinf(angle);
	matrix[9]  = 0.0f;
	matrix[10] = cosf(angle);
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	return;
}


void OpenGLClass::MatrixRotationZ(float* matrix, float angle)
{
	matrix[0]  = cosf(angle);
	matrix[1]  = sinf(angle);
	matrix[2]  = 0.0f;
	matrix[3]  = 0.0f;

	matrix[4]  = -sinf(angle);
	matrix[5]  = cosf(angle);
	matrix[6]  = 0.0f;
	matrix[7]  = 0.0f;

	matrix[8]  = 0.0f;
	matrix[9]  = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	return;
}


void OpenGLClass::MatrixTranslation(float* matrix, float x, float y, float z)
{
	matrix[0]  = 1.0f;
	matrix[1]  = 0.0f;
	matrix[2]  = 0.0f;
	matrix[3]  = 0.0f;

	matrix[4]  = 0.0f;
	matrix[5]  = 1.0f;
	matrix[6]  = 0.0f;
	matrix[7]  = 0.0f;

	matrix[8]  = 0.0f;
	matrix[9]  = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;

	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
	matrix[15] = 1.0f;

	return;
}


void OpenGLClass::MatrixScale(float* matrix, float x, float y, float z)
{
	matrix[0]  = x;
	matrix[1]  = 0.0f;
	matrix[2]  = 0.0f;
	matrix[3]  = 0.0f;

	matrix[4]  = 0.0f;
	matrix[5]  = y;
	matrix[6]  = 0.0f;
	matrix[7]  = 0.0f;

	matrix[8]  = 0.0f;
	matrix[9]  = 0.0f;
	matrix[10] = z;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	return;
}


void OpenGLClass::MatrixTranspose(float* result, float* matrix)
{
 	result[0]  = matrix[0];
	result[1]  = matrix[4];
	result[2]  = matrix[8];
	result[3]  = matrix[12];

	result[4]  = matrix[1];
	result[5]  = matrix[5];
	result[6]  = matrix[9];
	result[7]  = matrix[13];

	result[8]  = matrix[2];
	result[9]  = matrix[6];
	result[10] = matrix[10];
	result[11] = matrix[14];

	result[12] = matrix[3];
	result[13] = matrix[7];
	result[14] = matrix[11];
	result[15] = matrix[15];

    return;
}


void OpenGLClass::MatrixMultiply(float* result, float* matrix1, float* matrix2)
{
	result[0]  = (matrix1[0] * matrix2[0]) + (matrix1[1] * matrix2[4]) + (matrix1[2] * matrix2[8]) + (matrix1[3] * matrix2[12]);
	result[1]  = (matrix1[0] * matrix2[1]) + (matrix1[1] * matrix2[5]) + (matrix1[2] * matrix2[9]) + (matrix1[3] * matrix2[13]);
	result[2]  = (matrix1[0] * matrix2[2]) + (matrix1[1] * matrix2[6]) + (matrix1[2] * matrix2[10]) + (matrix1[3] * matrix2[14]);
	result[3]  = (matrix1[0] * matrix2[3]) + (matrix1[1] * matrix2[7]) + (matrix1[2] * matrix2[11]) + (matrix1[3] * matrix2[15]);

	result[4]  = (matrix1[4] * matrix2[0]) + (matrix1[5] * matrix2[4]) + (matrix1[6] * matrix2[8]) + (matrix1[7] * matrix2[12]);
	result[5]  = (matrix1[4] * matrix2[1]) + (matrix1[5] * matrix2[5]) + (matrix1[6] * matrix2[9]) + (matrix1[7] * matrix2[13]);
	result[6]  = (matrix1[4] * matrix2[2]) + (matrix1[5] * matrix2[6]) + (matrix1[6] * matrix2[10]) + (matrix1[7] * matrix2[14]);
	result[7]  = (matrix1[4] * matrix2[3]) + (matrix1[5] * matrix2[7]) + (matrix1[6] * matrix2[11]) + (matrix1[7] * matrix2[15]);

	result[8]  = (matrix1[8] * matrix2[0]) + (matrix1[9] * matrix2[4]) + (matrix1[10] * matrix2[8]) + (matrix1[11] * matrix2[12]);
	result[9]  = (matrix1[8] * matrix2[1]) + (matrix1[9] * matrix2[5]) + (matrix1[10] * matrix2[9]) + (matrix1[11] * matrix2[13]);
	result[10] = (matrix1[8] * matrix2[2]) + (matrix1[9] * matrix2[6]) + (matrix1[10] * matrix2[10]) + (matrix1[11] * matrix2[14]);
	result[11] = (matrix1[8] * matrix2[3]) + (matrix1[9] * matrix2[7]) + (matrix1[10] * matrix2[11]) + (matrix1[11] * matrix2[15]);

	result[12] = (matrix1[12] * matrix2[0]) + (matrix1[13] * matrix2[4]) + (matrix1[14] * matrix2[8]) + (matrix1[15] * matrix2[12]);
	result[13] = (matrix1[12] * matrix2[1]) + (matrix1[13] * matrix2[5]) + (matrix1[14] * matrix2[9]) + (matrix1[15] * matrix2[13]);
	result[14] = (matrix1[12] * matrix2[2]) + (matrix1[13] * matrix2[6]) + (matrix1[14] * matrix2[10]) + (matrix1[15] * matrix2[14]);
	result[15] = (matrix1[12] * matrix2[3]) + (matrix1[13] * matrix2[7]) + (matrix1[14] * matrix2[11]) + (matrix1[15] * matrix2[15]);

	return;
}


void OpenGLClass::TurnZBufferOn()
{
    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);

    return;
}


void OpenGLClass::TurnZBufferOff()
{
    // Disable depth testing.
    glDisable(GL_DEPTH_TEST);

    return;
}


void OpenGLClass::EnableAlphaBlending()
{
    // Enable alpha blending.
    glEnable(GL_BLEND);

    // Set the blending equation.
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    return;
}


void OpenGLClass::DisableAlphaBlending()
{
    // Disable alpha blending.
    glDisable(GL_BLEND);

    return;
}


void OpenGLClass::SetBackBufferRenderTarget()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
}


void OpenGLClass::ResetViewport()
{
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    return;
}
#endif