////////////////////////////////////////////////////////////////////////////////
// Filename: GLmathClass.cpp
////////////////////////////////////////////////////////////////////////////////
//#if ENGINE_LEVEL >= 10

#include "main.h" // CLASSLOADER
#include "GLmathClass.h"

GLmathClass::GLmathClass()
{
	CLASSLOADER();
}

GLmathClass::~GLmathClass() {CLASSDELETE();}

// Rastertek:
mat4 GLmathClass::BuildPerspectiveFovLHMatrix(float fieldOfView, float screenAspect, float screenNear, float screenDepth)
{
	return mat4(
		1.0f / (screenAspect * tan(fieldOfView * 0.5f)), 0, 0, 0,
		0, 1.0f / tan(fieldOfView * 0.5f), 0, 0,
		0, 0, screenDepth / (screenDepth - screenNear), 1,
		0, 0, (-screenNear * screenDepth) / (screenDepth - screenNear), 0);
}

// SOURCE: Humus
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

/*
mat4 GLmathClass::translation(float x, float y, float z)
{
	return mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, x,y,z,1);
}
*/
/*
//OLD:
void GLmathClass::MatrixTranslation(float* matrix, float x, float y, float z)
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
}
*/

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
	result[0] = matrix[0];
	result[1] = matrix[4];
	result[2] = matrix[8];
	result[3] = matrix[12];

	result[4] = matrix[1];
	result[5] = matrix[5];
	result[6] = matrix[9];
	result[7] = matrix[13];

	result[8] = matrix[2];
	result[9] = matrix[6];
	result[10] = matrix[10];
	result[11] = matrix[14];

	result[12] = matrix[3];
	result[13] = matrix[7];
	result[14] = matrix[11];
	result[15] = matrix[15];

	return;
}

/*
//OLD:
void GLmathClass::MatrixMultiply(float* result, float* matrix1, float* matrix2)
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
}
*/


//#endif