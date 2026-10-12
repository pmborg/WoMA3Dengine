// --------------------------------------------------------------------------------------------
// Filename: GLmathClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2026 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234526920;

#include "main.h" // CLASSLOADER
#include "GLmathClass.h"
#if defined ANDROID_PLATFORM
#include "math.h"
#endif
GLmathClass::GLmathClass()
{
	CLASSLOADER();
}

GLmathClass::~GLmathClass() {CLASSDELETE();}

// Function Source: Rastertek
mat4 GLmathClass::BuildPerspectiveFovLHMatrix(float fieldOfView, float screenAspect, float screenNear, float screenDepth)
{
	return mat4(
		1.0f / (screenAspect * tan(fieldOfView * 0.5f)), 0, 0, 0,
		0, 1.0f / tan(fieldOfView * 0.5f), 0, 0,
		0, 0, screenDepth / (screenDepth - screenNear), 1,
		0, 0, (-screenNear * screenDepth) / (screenDepth - screenNear), 0);
}

// Function Source: Humus
mat4 GLmathClass::rotateX(const float angle)
{
	float cosA = cosf(-angle), sinA = sinf(-angle);
	
	return mat4(
		1, 0,     0,    0,
		0, cosA, -sinA, 0,
		0, sinA,  cosA, 0,
		0, 0,     0,    1);
}

mat4 GLmathClass::rotateY(float angle)
{
	float cosA = cosf(-angle), sinA = sinf(-angle);

	return mat4(
		cosA, 0, -sinA, 0,
		0,    1,  0,    0,
		sinA, 0,  cosA, 0,
		0,    0,  0,    1);
}

mat4 GLmathClass::rotateZ(const float angle){
	float cosA = cosf(-angle), sinA = sinf(-angle);

	return mat4(
		cosA, -sinA, 0, 0,
		sinA,  cosA, 0, 0,
		0,     0,    1, 0,
		0,     0,    0, 1);
}

mat4 rotateXY(const float angleX, const float angleY){
	float cosX = cosf(-angleX), sinX = sinf(-angleX), 
		  cosY = cosf(-angleY), sinY = sinf(-angleY);

	return mat4(
		 cosY,        0,    -sinY,        0,
		-sinX * sinY, cosX, -sinX * cosY, 0,
		 cosX * sinY, sinX,  cosX * cosY, 0,
		 0,           0,     0,           1);
}

mat4 rotateYX(const float angleX, const float angleY){
	float cosX = cosf(-angleX), sinX = sinf(-angleX), 
		  cosY = cosf(-angleY), sinY = sinf(-angleY);

	return mat4(
		cosY, -sinX * sinY, -cosX * sinY, 0,
		0,     cosX,        -sinX,        0,
		sinY,  sinX * cosY,  cosX * cosY, 0,
		0,     0,            0,           1);
}

mat4 rotateZXY(const float angleX, const float angleY, const float angleZ){
	float cosX = cosf(-angleX), sinX = sinf(-angleX), 
		  cosY = cosf(-angleY), sinY = sinf(-angleY),
		  cosZ = cosf(-angleZ), sinZ = sinf(-angleZ);

	return mat4(
		cosY * cosZ + sinX * sinY * sinZ,   -cosX * sinZ,    sinX * cosY * sinZ - sinY * cosZ,  0,
		cosY * sinZ - sinX * sinY * cosZ,    cosX * cosZ,   -sinY * sinZ - sinX * cosY * cosZ,  0,
		cosX * sinY,                         sinX,           cosX * cosY,                       0,
		0,                                   0,              0,                                 1);
}
mat4 operator * (const mat4 &mat1, const mat4 &mat2) 
{
	return mat4(
		(mat1.m[0] * mat2.m[0]) + (mat1.m[1] * mat2.m[4]) + (mat1.m[2] * mat2.m[8]) + (mat1.m[3] * mat2.m[12]),
		(mat1.m[0] * mat2.m[1]) + (mat1.m[1] * mat2.m[5]) + (mat1.m[2] * mat2.m[9]) + (mat1.m[3] * mat2.m[13]),
		(mat1.m[0] * mat2.m[2]) + (mat1.m[1] * mat2.m[6]) + (mat1.m[2] * mat2.m[10]) + (mat1.m[3] * mat2.m[14]),
		(mat1.m[0] * mat2.m[3]) + (mat1.m[1] * mat2.m[7]) + (mat1.m[2] * mat2.m[11]) + (mat1.m[3] * mat2.m[15]),

		(mat1.m[4] * mat2.m[0]) + (mat1.m[5] * mat2.m[4]) + (mat1.m[6] * mat2.m[8]) + (mat1.m[7] * mat2.m[12]),
		(mat1.m[4] * mat2.m[1]) + (mat1.m[5] * mat2.m[5]) + (mat1.m[6] * mat2.m[9]) + (mat1.m[7] * mat2.m[13]),
		(mat1.m[4] * mat2.m[2]) + (mat1.m[5] * mat2.m[6]) + (mat1.m[6] * mat2.m[10]) + (mat1.m[7] * mat2.m[14]),
		(mat1.m[4] * mat2.m[3]) + (mat1.m[5] * mat2.m[7]) + (mat1.m[6] * mat2.m[11]) + (mat1.m[7] * mat2.m[15]),

		(mat1.m[8] * mat2.m[0]) + (mat1.m[9] * mat2.m[4]) + (mat1.m[10] * mat2.m[8]) + (mat1.m[11] * mat2.m[12]),
		(mat1.m[8] * mat2.m[1]) + (mat1.m[9] * mat2.m[5]) + (mat1.m[10] * mat2.m[9]) + (mat1.m[11] * mat2.m[13]),
		(mat1.m[8] * mat2.m[2]) + (mat1.m[9] * mat2.m[6]) + (mat1.m[10] * mat2.m[10]) + (mat1.m[11] * mat2.m[14]),
		(mat1.m[8] * mat2.m[3]) + (mat1.m[9] * mat2.m[7]) + (mat1.m[10] * mat2.m[11]) + (mat1.m[11] * mat2.m[15]),

		(mat1.m[12] * mat2.m[0]) + (mat1.m[13] * mat2.m[4]) + (mat1.m[14] * mat2.m[8]) + (mat1.m[15] * mat2.m[12]),
		(mat1.m[12] * mat2.m[1]) + (mat1.m[13] * mat2.m[5]) + (mat1.m[14] * mat2.m[9]) + (mat1.m[15] * mat2.m[13]),
		(mat1.m[12] * mat2.m[2]) + (mat1.m[13] * mat2.m[6]) + (mat1.m[14] * mat2.m[10]) + (mat1.m[15] * mat2.m[14]),
		(mat1.m[12] * mat2.m[3]) + (mat1.m[13] * mat2.m[7]) + (mat1.m[14] * mat2.m[11]) + (mat1.m[15] * mat2.m[15])
	);
}

void MatrixTranspose(mat4* result, mat4* matrix)
{
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			result->m[r * 4 + c] = matrix->m[c * 4 + r];
}

