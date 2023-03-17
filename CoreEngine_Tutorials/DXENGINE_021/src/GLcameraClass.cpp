// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: GLcameraClass.cpp
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

#include "GLcameraClass.h"

GLcameraClass::GLcameraClass()
{
	CLASSLOADER();

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

GLcameraClass::~GLcameraClass() {CLASSDELETE();}


void GLcameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void GLcameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


void GLcameraClass::Render()
{
	VectorType up, position, lookAt;
	float yaw, pitch, roll;
	float rotationMatrix[10];


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	TransformCoord(lookAt, rotationMatrix);
	TransformCoord(up, rotationMatrix);
	
	// Translate the rotated camera position to the location of the viewer.
	lookAt.x = position.x + lookAt.x;
	lookAt.y = position.y + lookAt.y;
	lookAt.z = position.z + lookAt.z;

	// Finally create the view matrix from the three updated vectors.
	BuildViewMatrix(position, lookAt, up);

	return;
}


void GLcameraClass::MatrixRotationYawPitchRoll(float* matrix, float yaw, float pitch, float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;


	// Get the cosine and sin of the yaw, pitch, and roll.
	cYaw = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll = cosf(roll);

	sYaw = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll = sinf(roll);

	// Calculate the yaw, pitch, roll rotation matrix.
	matrix[0] = (cRoll * cYaw) + (sRoll * sPitch * sYaw);
	matrix[1] = (sRoll * cPitch);
	matrix[2] = (cRoll * -sYaw) + (sRoll * sPitch * cYaw);
	
	matrix[3] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
	matrix[4] = (cRoll * cPitch);
	matrix[5] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);
	
	matrix[6] = (cPitch * sYaw);
	matrix[7] = -sPitch;
	matrix[8] = (cPitch * cYaw);

	return;
}


void GLcameraClass::TransformCoord(VectorType& vector, float* matrix)
{
	float x, y, z;


	// Transform the vector by the 3x3 matrix.
	x = (vector.x * matrix[0]) + (vector.y * matrix[3]) + (vector.z * matrix[6]);
	y = (vector.x * matrix[1]) + (vector.y * matrix[4]) + (vector.z * matrix[7]);
	z = (vector.x * matrix[2]) + (vector.y * matrix[5]) + (vector.z * matrix[8]);

	// Store the result in the reference.
	vector.x = x;
	vector.y = y;
	vector.z = z;

	return;
}


void GLcameraClass::BuildViewMatrix(VectorType position, VectorType lookAt, VectorType up)
{
	VectorType zAxis, xAxis, yAxis;
	float length, result1, result2, result3;


	// zAxis = normal(lookAt - position)
	zAxis.x = lookAt.x - position.x;
	zAxis.y = lookAt.y - position.y;
	zAxis.z = lookAt.z - position.z;
	length = sqrt((zAxis.x * zAxis.x) + (zAxis.y * zAxis.y) + (zAxis.z * zAxis.z));
	zAxis.x = zAxis.x / length;
	zAxis.y = zAxis.y / length;
	zAxis.z = zAxis.z / length;

	// xAxis = normal(cross(up, zAxis))
	xAxis.x = (up.y * zAxis.z) - (up.z * zAxis.y);
	xAxis.y = (up.z * zAxis.x) - (up.x * zAxis.z);
	xAxis.z = (up.x * zAxis.y) - (up.y * zAxis.x);
	length = sqrt((xAxis.x * xAxis.x) + (xAxis.y * xAxis.y) + (xAxis.z * xAxis.z));
	xAxis.x = xAxis.x / length;
	xAxis.y = xAxis.y / length;
	xAxis.z = xAxis.z / length;

	// yAxis = cross(zAxis, xAxis)
	yAxis.x = (zAxis.y * xAxis.z) - (zAxis.z * xAxis.y);
	yAxis.y = (zAxis.z * xAxis.x) - (zAxis.x * xAxis.z);
	yAxis.z = (zAxis.x * xAxis.y) - (zAxis.y * xAxis.x);

	// -dot(xAxis, position)
	result1 = ((xAxis.x * position.x) + (xAxis.y * position.y) + (xAxis.z * position.z)) * -1.0f;

	// -dot(yaxis, eye)
	result2 = ((yAxis.x * position.x) + (yAxis.y * position.y) + (yAxis.z * position.z)) * -1.0f;

	// -dot(zaxis, eye)
	result3 = ((zAxis.x * position.x) + (zAxis.y * position.y) + (zAxis.z * position.z)) * -1.0f;

	// Set the computed values in the view matrix.
	m_viewMatrix.m[0]  = xAxis.x;
	m_viewMatrix.m[1]  = yAxis.x;
	m_viewMatrix.m[2]  = zAxis.x;
	m_viewMatrix.m[3]  = 0.0f;

	m_viewMatrix.m[4]  = xAxis.y;
	m_viewMatrix.m[5]  = yAxis.y;
	m_viewMatrix.m[6]  = zAxis.y;
	m_viewMatrix.m[7]  = 0.0f;

	m_viewMatrix.m[8]  = xAxis.z;
	m_viewMatrix.m[9]  = yAxis.z;
	m_viewMatrix.m[10] = zAxis.z;
	m_viewMatrix.m[11] = 0.0f;

	m_viewMatrix.m[12] = result1;
	m_viewMatrix.m[13] = result2;
	m_viewMatrix.m[14] = result3;
	m_viewMatrix.m[15] = 1.0f;

	return;
}

void GLcameraClass::GetViewMatrix(float* matrix)
{
	matrix[0]  = m_viewMatrix.m[0];
	matrix[1]  = m_viewMatrix.m[1];
	matrix[2]  = m_viewMatrix.m[2];
	matrix[3]  = m_viewMatrix.m[3];

	matrix[4]  = m_viewMatrix.m[4];
	matrix[5]  = m_viewMatrix.m[5];
	matrix[6]  = m_viewMatrix.m[6];
	matrix[7]  = m_viewMatrix.m[7];

	matrix[8]  = m_viewMatrix.m[8];
	matrix[9]  = m_viewMatrix.m[9];
	matrix[10] = m_viewMatrix.m[10];
	matrix[11] = m_viewMatrix.m[11];

	matrix[12] = m_viewMatrix.m[12];
	matrix[13] = m_viewMatrix.m[13];
	matrix[14] = m_viewMatrix.m[14];
	matrix[15] = m_viewMatrix.m[15];
}

